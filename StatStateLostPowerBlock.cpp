// StatStateLostPowerBlock.cpp: implementation of the CStatStateLostPowerBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatStateLostPowerBlock.h"
#include "PropertyDlgStateLostPower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatStateLostPowerBlock::CStatStateLostPowerBlock()
{
	iInputDataStart=0;
	this->iTimeSelfOrInput=1;
	ConstructBlockInputOutput();
	SetBlockType(_T( "StateLostPower" ) );
}

CStatStateLostPowerBlock::~CStatStateLostPowerBlock()
{

}

//根据类型新建对象的函数
CCalcBlock* CStatStateLostPowerBlock::CreateFromString( const CString& strType )
{
	CStatStateLostPowerBlock* obj = new CStatStateLostPowerBlock();
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatStateLostPowerBlock::ConstructBlockInputOutput ()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
	AddInputPort("WindSpeed",VALUE_RTVALUE_SERIES);

	AddOutputPort("OutPut",VALUE_STAT);
}
//初始化计算的函数做特殊的初始化
void CStatStateLostPowerBlock::InitCalc()
{
	//将功率曲线进行排序，风速递增
	for(int i=0;i<dWindSpeedArr.GetSize();i++)
	{
		for(int j=i+1;j<dWindSpeedArr.GetSize();j++)
		{
			if(dWindSpeedArr[i]>dWindSpeedArr[j])
			{
				double dTemp=0;
				
				dTemp=dWindSpeedArr[i];
				dWindSpeedArr[i]=dWindSpeedArr[j];
				dWindSpeedArr[j]=dTemp;

				dTemp=dPowerArr[i];
				dPowerArr[i]=dPowerArr[j];
				dPowerArr[j]=dTemp;
			}
		}
	}
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CStatStateLostPowerBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatStateLostPowerBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}

void CStatStateLostPowerBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	int lSTime=0;
	int lETime=0;
	
	if(iTimeSelfOrInput==1)
	{
		double dInValue=0;
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
		lSTime=(long)dInValue;

		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
		lETime=(long)dInValue;
		iInputDataStart=2;
	}
	long lRealSTime=0;
	long lRealETime=0;
	double dValue=0;
	long lTimeOld=0;
	long lTimeSum=0;

	CCalcPort *pPortObjWindSpeed = GetInputPortObj(iInputDataStart);	
	CUniValue  &CUVWindSpeed=pPortObjWindSpeed->GetPortUniValue();
	
	lTimeOld=0;
	for(int j=0; j<CUVWindSpeed.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUVWindSpeed.GetDoubleValAt(j,&value);
		CUVWindSpeed.GetTimeValAt(j,&lTime);	
		double dPower=getPowerFromWindSpeed(value);
		bool bValidLost=isValidLostByTime(lTime);
		if(j==0)
		{
			lTimeOld=lTime;
			lRealSTime=lTime;
			lRealETime=lTime;
		}
		else
		{
			if(bValidLost)
			{
				dValue+=dPower*(lTime-lTimeOld);
			}
		}
		if(j+1==CUVWindSpeed.GetDataSize())
		{
			lRealETime=lTime;
		}
		lTimeOld=lTime;
	}
	double dMin=0;
	double dMax=0;		
	if(iTimeSelfOrInput==1)//修正为外部时间段。
	{
		lRealSTime=lSTime;
		lRealETime=lETime;
	}
	StatData oneStatData;
	oneStatData.lSTime=lRealSTime;
	oneStatData.lETime=lRealETime;
	oneStatData.lInsertTime=0;
	oneStatData.dMin=dMin;
	oneStatData.dMax=dMax;
	oneStatData.dValue=dValue/3600.0;//转换为千瓦时
	oneStatData.iType=0;
	oneStatData.iCount=CUVWindSpeed.GetDataSize();
	CUniValue UniVarOut;
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	UniVarOut.AddStatData(oneStatData);
	pPortObjOut->SetPortUniValue(UniVarOut);
	//输出计算
	OutputResultToGlobalWS();
}

bool CStatStateLostPowerBlock::isValidLostByTime(long lParTime)
{
	bool bRet=false;
	int iInputNum=this->GetInputNum();
	int iCountValid=0;

	//循环故障、正常、维护停机，任意一个符合正常值范围 返回true 
	//任意一个情况出现 当前时段需要计算为弃风电量
	for(int i=iInputDataStart+1;i<iInputNum;i++)
	{
		CCalcPort *pPortObj = GetInputPortObj(i);	
		CUniValue  &CUV=pPortObj->GetPortUniValue();
		int iValidMinTag=iValidMin[iCountValid];
		int iValidMaxTag=iValidMax[iCountValid];
		iCountValid++;

		//循环故障、正常、维护停机中任意一个的序列值，任意一个符合范围返回 true
		//正常应该某一值符合即将那一刻值的附近时段人为故障、正常、维护停机，但当前默认按此方式处理，即整个时段均需要计算为弃风电量
		for(int j=CUV.GetDataSize()-1;j>=0;j--)
		{
			double value=0;
			long lTime=0;
			CUV.GetDoubleValAt(j,&value);
			CUV.GetTimeValAt(j,&lTime);
			if(lParTime>=lTime)
			{
				if((((int)value)>=iValidMinTag)&&(((int)value)<=iValidMaxTag))
				{
					bRet=true;
					break;
				}
				break;//在对应的时间范围内改状态量要么有效要么无效
			}
		}

		if(bRet==true)
		{
			break;
		}
	}
	return  bRet;
}
double CStatStateLostPowerBlock::getPowerFromWindSpeed(double dWindSpeed)
{
	double dPower=0;
	for(int i=dWindSpeedArr.GetSize()-1;i>=0;i--)
	{
		if(dWindSpeed>=dWindSpeedArr[i])
		{
			if(i==dWindSpeedArr.GetSize()-1)
			{
				dPower=dPowerArr[i];
				break;
			}
			else
			{
				dPower=dPowerArr[i]+(dPowerArr[i+1]-dPowerArr[i])*(dWindSpeed-dWindSpeedArr[i])/(dWindSpeedArr[i+1]-dWindSpeedArr[i]);
			}
		}
	}
	return dPower;
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CStatStateLostPowerBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SelfOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {iTimeSelfOrInput=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==1)
		{
			if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {iValidMin.SetAtGrow(propIndex,atoi(strItem2));}
			if(!strItem3.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {iValidMax.SetAtGrow(propIndex,atoi(strItem4));}
		}
		else if(propType==2)
		{
			if(!strItem1.IsEmpty())  {dWindSpeedArr.SetAtGrow(propIndex,atof(strItem1));}
			if(!strItem2.IsEmpty())  {dPowerArr.SetAtGrow(propIndex,atof(strItem2));}
		}
	}
}
void CStatStateLostPowerBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="WindPower")//输出为2
	{
		propType=2;
	}
}
//重载以显示不同的属性对象对话框
void CStatStateLostPowerBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStateLostPower dlg;

	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> IDArrLeftOutType;//计算块原有的输入端口类型
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CArray<int,int>  IDArrLeftInType;//计算块内部的输入端口类型
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}

	dlg.strInputArr.Copy(strInputArr);
	dlg.iValidMin.Copy(iValidMin);
	dlg.iValidMax.Copy(iValidMax);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.dWindSpeedArr.Copy(dWindSpeedArr);
	dlg.dPowerArr.Copy(dPowerArr);
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;

	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);
		strTagDespArr.Copy(dlg.strTagDespArr);	
		dWindSpeedArr.Copy(dlg.dWindSpeedArr);
		dPowerArr.Copy(dlg.dPowerArr);	
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;
		
		iValidMin.Copy(dlg.iValidMin);	
		iValidMax.Copy(dlg.iValidMax);

		//1、对输入端口的修改
		if(iTimeSelfOrInput==1)
		{
			IDArrLeftIn.Add("STime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
			IDArrLeftIn.Add("ETime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
		}
		IDArrLeftIn.Add("WindSpeed");
		IDArrLeftInType.Add(VALUE_RTVALUE_SERIES);
	
		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_RTVALUE_SERIES);
		}
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j])||(IDArrLeftInType[k]!=IDArrLeftOutType[j]))	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}	
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
		{
			int  runs=0;//遍历次数
			for(int n=0; n<IDArrLeftOut.GetSize(); n++)
			{
				if((IDArrLeftIn[m]!=IDArrLeftOut[n])||(IDArrLeftInType[m]!=IDArrLeftOutType[n]))
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, IDArrLeftInType[m], _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CStatStateLostPowerBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strInputArr.GetSize();i++)//输出
	{
		CString strInPut=strInputArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",strInputArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",iValidMin[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名
		strFieldValue.Format("%d",iValidMax[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	}
	for(i=0;i<dWindSpeedArr.GetSize();i++)//输出
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("WindPower.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%f",dWindSpeedArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%f",dPowerArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SelfOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",iTimeSelfOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStatStateLostPowerBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strInputArr.Copy(((CStatStateLostPowerBlock*)pBlock)->strInputArr);
	this->iValidMin.Copy(((CStatStateLostPowerBlock*)pBlock)->iValidMin);
	this->iValidMax.Copy(((CStatStateLostPowerBlock*)pBlock)->iValidMax);
	this->strTagDespArr.Copy(((CStatStateLostPowerBlock*)pBlock)->strTagDespArr);

	this->iTimeSelfOrInput=((CStatStateLostPowerBlock*)pBlock)->iTimeSelfOrInput;

	this->dWindSpeedArr.Copy(((CStatStateLostPowerBlock*)pBlock)->dWindSpeedArr);
	this->dPowerArr.Copy(((CStatStateLostPowerBlock*)pBlock)->dPowerArr);
}
//将属性放置传输数据中
void CStatStateLostPowerBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
