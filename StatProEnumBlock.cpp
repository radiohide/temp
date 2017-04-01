// StatProEnumBlock.cpp: implementation of the CStatProEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatProEnumBlock.h"
#include "PropertyDlgStatProEnum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatProEnumBlock::CStatProEnumBlock(int iTimeSelfOrInput)
{
	m_dLeft=0.5;//区间左（闭区间）
	m_dRight=1.5;//区间右（闭区间）
	m_iStatType=1;//状态值
	m_iFunType=0;//统计方法，次数统计
	m_dGain=1;//1秒=0.0166666666666667小时

	this->iTimeSelfOrInput=iTimeSelfOrInput;
	if(iTimeSelfOrInput==1)
	{
		ConstructBlockInputOutput();
	}
	SetBlockType(_T( "StatProEnum" ) );
}

CStatProEnumBlock::~CStatProEnumBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CStatProEnumBlock::CreateFromString( const CString& strType )
{
	CStatProEnumBlock* obj = new CStatProEnumBlock(1);
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatProEnumBlock::ConstructBlockInputOutput ()
{
	//初始化时从外部获取时间起始，所以初始时添加两个端口
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CStatProEnumBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CStatProEnumBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatProEnumBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatProEnumBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	int lSTime=0;
	int lETime=0;
	int iInputDataStart=0;
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

	//添加外部数据区间参数方式添加，数据由vestore提供，原采用界面配置方式，该部分为提高参数变换时的适应性

	for(int i=0;i<strInputArr.GetSize();i++)
	{
		CCalcPort *pPortObjIn3 = GetInputPortObj(iInputDataStart+i);	
		CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
		
		double dValue=0;
		double dMin=0;
		double dMax=0;	
		long lRealSTime=0;
		long lRealETime=0;
		int iCount=0; 
		if(m_iFunType==0)//统计次数
		{
			statDataCount(CUV,dValue,dMin,dMax,lRealSTime,lRealETime,iCount);
		}
		else//统计时长
		{
			statDataTimeLength(CUV,dValue,dMin,dMax,lRealSTime,lRealETime,iCount);
		}
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
		oneStatData.dValue=dValue*m_dGain;//放大m_dGain倍数
		oneStatData.iType=m_iStatType;//设定状态值
		oneStatData.iCount=iCount;

		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}
	//输出计算
	OutputResultToGlobalWS();
}
void CStatProEnumBlock::statDataTimeLength(CUniValue  &CUV,//统计时长
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dValue=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetDataSize();
	double dOldValue=0;
	long lOldTime=0;
	for(int j=0; j<CUV.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			dMin=value;
			dMax=dMin;
			lRealSTime=lTime;
			lRealETime=lTime;
			dOldValue=value;
			lOldTime=lTime;
		}
		else
		{
			//前后两个数据点均在区间内则认为在区间内持续
			if((isInRange(dOldValue,m_dLeft,m_dRight))&&(isInRange(value,m_dLeft,m_dRight)))
			{
				dValue+=(double)(lTime-lOldTime);//时间累加
			}
			if(value<dMin)
			{
				dMin=value;
			}
			if(value>dMax)
			{
				dMax=value;
			}
		}
		if(j+1==CUV.GetDataSize())
		{
			lRealETime=lTime;
		}
		dOldValue=value;
		lOldTime=lTime;
	}
}
void CStatProEnumBlock::statDataCount(CUniValue  &CUV,
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dValue=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetDataSize();
	double dOldValue=0;
	for(int j=0; j<CUV.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			dMin=value;
			dMax=dMin;
			lRealSTime=lTime;
			lRealETime=lTime;
			dOldValue=value;
		}
		else
		{
			//前一个点不在该范围内，当前点在该范围内，则认为进入一次（跳变一次）
			if((!isInRange(dOldValue,m_dLeft,m_dRight))&&(isInRange(value,m_dLeft,m_dRight)))
			{
				dValue+=1;//次数加1
			}
			if(value<dMin)
			{
				dMin=value;
			}
			if(value>dMax)
			{
				dMax=value;
			}
		}
		if(j+1==CUV.GetDataSize())
		{
			lRealETime=lTime;
		}
		dOldValue=value;
	}
}
bool CStatProEnumBlock::isInRange(const double &dValue,const double &dLeft,const double &dRight)
{
	bool bRet=false;
	if((dValue>=dLeft)&&(dValue<=dRight))
	{
		bRet=true;
	}
	return bRet;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStatProEnumBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SelfOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {iTimeSelfOrInput=atoi(strItem1);}
	}
	else if(strPropName.Compare("StatFun")==0)
	{
		if(!strItem1.IsEmpty())  {m_dLeft=atof(strItem1);}
		if(!strItem2.IsEmpty())  {m_dRight=atof(strItem2);}
		if(!strItem3.IsEmpty())  {m_iStatType=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iFunType=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_dGain=atof(strItem5);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CStatProEnumBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="formula")//公式为3
	{
		propType=3;
	}
}
//重载以显示不同的属性对象对话框
void CStatProEnumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatProEnum dlg;

	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	
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

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strInputArr.Copy(strInputArr);//输入变量
	dlg.strOutputArr.Copy(strOutputArr);//输出变量
	dlg.strTagDespArr.Copy(strTagDespArr);//描述点名
	dlg.strUnitArr.Copy(strUnitArr);//单位
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
	dlg.m_dLeft=m_dLeft;//区间左（闭区间）
	dlg.m_dRight=m_dRight;//区间右（闭区间）
	dlg.m_iStatType=m_iStatType;//状态值
	dlg.m_iFunType=m_iFunType;//统计方法
	dlg.m_dGain=m_dGain;//放大倍数
	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);//输入变量
		strOutputArr.Copy(dlg.strOutputArr);//输出变量
		strTagDespArr.Copy(dlg.strTagDespArr);//描述点名
		strUnitArr.Copy(dlg.strUnitArr);//单位
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入

		m_dLeft=dlg.m_dLeft;//区间左（闭区间）
		m_dRight=dlg.m_dRight;//区间右（闭区间）
		m_iStatType=dlg.m_iStatType;//状态值
		m_iFunType=dlg.m_iFunType;//统计方法
		m_dGain=dlg.m_dGain;//放大倍数
		//1、对输入端口的修改
		if(iTimeSelfOrInput==1)
		{
			IDArrLeftIn.Add("STime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
			IDArrLeftIn.Add("ETime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
		}
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

		//对输出端口的修改
		for(i=0;i<strOutputArr.GetSize();i++)
		{
			IDArrRightIn.Add(strOutputArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj]))	
				{runs++;}
			}
			if(runs==IDArrRightIn.GetSize())
			{
				this->ClearOutputPortByIndex(jj,parent);
			}
		}
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
		{
			int  runs=0;//遍历次数
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_STAT, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CStatProEnumBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",strInputArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strOutputArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strUnitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SelfOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",iTimeSelfOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="StatFun";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dLeft); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%f",m_dRight); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%i",m_iStatType); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%i",m_iFunType); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dGain); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStatProEnumBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strInputArr.Copy(((CStatProEnumBlock*)pBlock)->strInputArr);
	this->strOutputArr.Copy(((CStatProEnumBlock*)pBlock)->strOutputArr);
	this->strTagDespArr.Copy(((CStatProEnumBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CStatProEnumBlock*)pBlock)->strUnitArr);
	this->iTimeSelfOrInput=((CStatProEnumBlock*)pBlock)->iTimeSelfOrInput;

	this->m_dLeft=((CStatProEnumBlock*)pBlock)->m_dLeft;
	this->m_dRight=((CStatProEnumBlock*)pBlock)->m_dRight;
	this->m_iStatType=((CStatProEnumBlock*)pBlock)->m_iStatType;
	this->m_iFunType=((CStatProEnumBlock*)pBlock)->m_iFunType;
	this->m_dGain=((CStatProEnumBlock*)pBlock)->m_dGain;
}
//将属性放置传输数据中
void CStatProEnumBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
