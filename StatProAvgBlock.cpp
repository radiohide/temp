// StatProAvgBlock.cpp: implementation of the CStatProAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatProAvgBlock.h"
#include "PropertyDlgStatProAvg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CStatProAvgBlock::CStatProAvgBlock(int iTimeSelfOrInput)
{
	this->iTimeSelfOrInput=iTimeSelfOrInput;
	if(iTimeSelfOrInput==1)
	{
		ConstructBlockInputOutput();
	}
	SetBlockType(_T( "StatProAvg" ) );
}
CStatProAvgBlock::~CStatProAvgBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CStatProAvgBlock::CreateFromString( const CString& strType )
{
	CStatProAvgBlock* obj = new CStatProAvgBlock(1);
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatProAvgBlock::ConstructBlockInputOutput ()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CStatProAvgBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CStatProAvgBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatProAvgBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatProAvgBlock::Calc()//进行计算的函数,需要重载
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
	for(int i=0;i<strInputArr.GetSize();i++)
	{
		CCalcPort *pPortObjIn3 = GetInputPortObj(iInputDataStart+i);	
		CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
		
		double dAvg=0;
		double dMin=0;
		double dMax=0;	
		long lRealSTime=0;
		long lRealETime=0;
		int iCount=0; 
		statData(CUV,dAvg,dMin,dMax,lRealSTime,lRealETime,iCount);
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
		oneStatData.dValue=dAvg;
		oneStatData.iType=0;
		oneStatData.iCount=iCount;

		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}
	//输出计算
	OutputResultToGlobalWS();
}
void CStatProAvgBlock::statData(CUniValue  &CUV,
							 double &dAvg,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dAvg=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetDataSize();
	double dSum=0;
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
		}
		else
		{
			if(value<dMin)
			{
				dMin=value;
			}
			if(value>dMax)
			{
				dMax=value;
			}
		}
		dSum+=value;
		if(j+1==CUV.GetDataSize())
		{
			lRealETime=lTime;
		}
	}
	if(CUV.GetDataSize()>0)
	{
		dAvg=dSum/CUV.GetDataSize();
	}
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStatProAvgBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SelfOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {iTimeSelfOrInput=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);//处理多行
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CStatProAvgBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);//括号中值为字符串右数的个数
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
void CStatProAvgBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatProAvg dlg;
	
	//计算块有连接时并在属性窗口对接口有修改时的预处理
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> IDArrLeftOutType;//计算块原有的输入端口类型
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CArray<int,int>  IDArrLeftInType;//计算块内部的输入端口类型
	
	//最近输入端口组
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}

	//最近的输出端口组
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	//将计算块类中的变量值更新到窗口类对应的变量
	dlg.strInputArr.Copy(strInputArr);//输入变量
	dlg.strOutputArr.Copy(strOutputArr);//输出变量
	dlg.strTagDespArr.Copy(strTagDespArr);//描述点名
	dlg.strUnitArr.Copy(strUnitArr);//单位
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入

	//显示属性窗口 等待确认
	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);//输入变量
		strOutputArr.Copy(dlg.strOutputArr);//输出变量
		strTagDespArr.Copy(dlg.strTagDespArr);//描述点名
		strUnitArr.Copy(dlg.strUnitArr);//单位
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入


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
void CStatProAvgBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;

	//动态添加项
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

	//默认固定项
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SelfOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",iTimeSelfOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStatProAvgBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strInputArr.Copy(((CStatProAvgBlock*)pBlock)->strInputArr);
	this->strOutputArr.Copy(((CStatProAvgBlock*)pBlock)->strOutputArr);
	this->strTagDespArr.Copy(((CStatProAvgBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CStatProAvgBlock*)pBlock)->strUnitArr);
	this->iTimeSelfOrInput=((CStatProAvgBlock*)pBlock)->iTimeSelfOrInput;
}
//将属性放置传输数据中
void CStatProAvgBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
