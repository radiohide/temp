// IfBlock.cpp: implementation of the CIfBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "IfBlock.h"
#include "PropertyDlgIF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIfBlock::CIfBlock()
{
	SetBlockType(_T( "IF" ) );//设置类型
	m_iDataType=0;//默认
	ConstructBlockInputOutput();
}

CIfBlock::~CIfBlock()
{
	
}
void CIfBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("IF"), VALUE_DOUBLE);
	AddInputPort(_T("InValue1"), VALUE_DOUBLE);
	AddInputPort(_T("InValue2"), VALUE_DOUBLE);
	AddOutputPort("Out",VALUE_DOUBLE);
} 
//根据类型新建对象的函数
CCalcBlock* CIfBlock::CreateFromString( const CString& strType )
{
	CIfBlock* obj = new CIfBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CIfBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CIfBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInputIfPortObj=GetInputPortObj(0);
	double ifDValue;
	pInputIfPortObj->GetPortUniValue().GetDoubleVal(&ifDValue);
	if(ifDValue>0)
	{
		pOutPortObj->SetPortUniValue(GetInputPortObj(1)->GetPortUniValue());//取输入1
	}
	else
	{
		pOutPortObj->SetPortUniValue(GetInputPortObj(2)->GetPortUniValue());//取输入2
	}
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CIfBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("dateType")==0)
	{
		if(!strItem1.IsEmpty())  m_iDataType=atoi(strItem1);
	}
}
//重载以显示不同的属性对象对话框
void CIfBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgIF dlg;
	CString strTagName;
    //给对话框变量赋值
    dlg.SetType(this->m_iDataType);
	int SetBefore=this->m_iDataType;
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		this->m_iDataType = dlg.GetType();
		int SetBehind=dlg.GetType();
		
		/*
		ClearInputPorts();
		ClearOutputPorts();
		AddInputPort(_T("If"), VALUE_DOUBLE, _T("If"), _T("If"));
		AddInputPort(_T("InValue1"), m_iDataType, _T("in1"), _T("in1"));
		AddInputPort(_T("InValue2"), m_iDataType, _T("in2"), _T("in2"));
		AddOutputPort("out",m_iDataType,"out");
		*/
		if(SetBefore==SetBehind){}
		else
		{
			this->ClearInputPortByIndex(2);
			this->ClearInputPortByIndex(1);
			ClearOutputPorts();

			AddInputPort(_T("InValue1"), m_iDataType, _T("in1"), _T("in1"));
			AddInputPort(_T("InValue2"), m_iDataType, _T("in2"), _T("in2"));
			AddOutputPort("out",m_iDataType,"out");
		}
		parent->RedrawWindow();
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
	//////////////////////////////////////////////////
}
//重载以将本模块写入到数据库中
void CIfBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="dateType";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iDataType); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}
void CIfBlock::CopyPropertyData(CCalcBlock* pBlock)
{	
	m_iDataType=((CIfBlock*)pBlock)->m_iDataType;
}
	//将属性放置传输数据中
void CIfBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"dateType");
	
	CString str;
	str.Format("%d",m_iDataType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}