// ProcessToStatisBlock.cpp: implementation of the CProcessToStatisBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ProcessToStatis.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcessToStatisBlock::CProcessToStatisBlock()
{
	m_nStatType = 1;//仅做初始值
	SetBlockType(_T( "ProcessToStatis" ) );
	ConstructBlockInputOutput();
}

CProcessToStatisBlock::~CProcessToStatisBlock()
{

}
//构造初始的输入输出个数
void CProcessToStatisBlock::ConstructBlockInputOutput()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);

	//AddInputPort(_T("A1"), VALUE_DOUBLE);
	//AddOutputPort("out",VALUE_STAT,"out");
}
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CProcessToStatisBlock::CreateFromString( const CString& strType )
{
	CProcessToStatisBlock* obj = new CProcessToStatisBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//初始化计算的函数做特殊的初始化
void CProcessToStatisBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//计算函数，实现本模块的计算
void CProcessToStatisBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();

	//获取输入

	//起始终止时间值
	int lSTime=0;
	int lETime=0;

	double dInValue=0;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
	lSTime=(long)dInValue;

	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
	lETime=(long)dInValue;

	//过程值获取
	for(int i=0;i<strInputArr.GetSize();i++)
	{
		double inValue = 0;
		CCalcPort *pPortObj = GetInputPortObj(i+2);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		
		//转换
		StatData oneStatData;
		oneStatData.lSTime = lSTime;
		oneStatData.lETime = lETime;
		oneStatData.dValue = inValue;
		oneStatData.dMax = inValue;
		oneStatData.dMin = inValue;
		oneStatData.iCount = 1;
		oneStatData.iType = m_nStatType;
		CTime curTime=CTime::GetCurrentTime();
		oneStatData.lInsertTime=curTime.GetTime();
		
		
		//////////////////////////////////
		//输出计算
		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}

	OutputResultToGlobalWS();
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CProcessToStatisBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_nStatType=atoi(strItem1);}
	}
	else
	{
		int propIndex;
		int dIndex=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);//括号中值为字符串右数的个数
		CString strName=strPropName.Left(dIndex);
	    propIndex=atoi(strIndex);

		if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
		if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
		if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
		if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
	}
}

//重载以显示不同的属性对象对话框
void CProcessToStatisBlock::ShowPropertyDialog( CWnd* parent, BOOL show)
{
	//显示窗口
	CPropertyDlgProcToStatis dlg;

	//计算块有连接时并在属性窗口对接口有修改时的预处理
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> IDArrLeftOutType;//计算块原有的输入端口类型
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CArray<int,int>  IDArrLeftInType;//计算块内部的输入端口类型
	
	//最近时间的输入端口组
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}
	
	//最近时间的输出端口组
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
	dlg.m_nStatType = m_nStatType;


	//将属性页的数据加入当前计算快变量
	if(IDOK==dlg.DoModal())
	{
		//将属性窗口中更新的数据项拷贝到计算块对应的数据中
		m_nStatType = dlg.m_nStatType;
		strInputArr.Copy(dlg.strInputArr);//输入变量
		strOutputArr.Copy(dlg.strOutputArr);//输出变量
		strTagDespArr.Copy(dlg.strTagDespArr);//描述点名
		strUnitArr.Copy(dlg.strUnitArr);//单位

		//1、对输入端口的修改
		IDArrLeftIn.Add("STime");
		IDArrLeftInType.Add(VALUE_DOUBLE);
		IDArrLeftIn.Add("ETime");
		IDArrLeftInType.Add(VALUE_DOUBLE);

		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_DOUBLE);//VALUE_RTVALUE_SERIES
		}
		
		//查找最近输入项删除属性窗口已删除的项
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
		//查找最近输入项增加属性窗口已增加的项
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
void CProcessToStatisBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	//默认固定项
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name,"block_name"为prop表字段名
	strFieldValue="StrInputCfg";//属性名,同类分行使用
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_nStatType); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	//动态加载项
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
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CProcessToStatisBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_nStatType=((CProcessToStatisBlock*)pBlock)->m_nStatType;
}
//将属性放置传输数据中
void CProcessToStatisBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}