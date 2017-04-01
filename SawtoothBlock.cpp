// SawtoothBlock.cpp: implementation of the CSawtoothBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SawtoothBlock.h"
#include "PropertyDlgSawtooth.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSawtoothBlock::CSawtoothBlock()
{
	m_iMin =0;
	m_iMax =30000;
	m_BFirstCalc=TRUE;
	m_iResult=0;
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "Sawtooth" ) );
	ConstructBlockInputOutput();
}

CSawtoothBlock::~CSawtoothBlock()
{
	
}
//构造初始的输入输出个数
void CSawtoothBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CSawtoothBlock::CreateFromString( const CString& strType )
{
	CSawtoothBlock* obj = new CSawtoothBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//初始化计算的函数做特殊的初始化
void CSawtoothBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//计算函数，实现本模块的计算
void CSawtoothBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	if(m_BFirstCalc)
	{	
		m_iResult=m_iMin;
	}
	else
	{
		m_iResult++;
		if(m_iResult>m_iMax)
		{
			m_iResult=m_iMin;
		}
	}
	m_BFirstCalc=FALSE;
  	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	UniVar.AddData(m_iResult);
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CSawtoothBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Sawtooth")==0)
	{
		if(!strItem1.IsEmpty())
			m_iMin=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_iMax=atoi(strItem2);	
	}
}

//重载以显示不同的属性对象对话框
void CSawtoothBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgSawtooth  dlg;
	
	dlg.m_iMin =m_iMin;
	dlg.m_iMax=m_iMax;
    ///
	if(dlg.DoModal()==IDOK)
	{
		m_iMin=dlg.m_iMin;
		m_iMax=dlg.m_iMax;
	}
}
//重载以将本模块写入到数据库中
void CSawtoothBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("Sawtooth"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	CString str;
	str.Format("%d",m_iMin);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(str));
	str.Format("%d",m_iMax);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(str));
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CSawtoothBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iMin=((CSawtoothBlock*)pBlock)->m_iMin;
	m_iMax=((CSawtoothBlock*)pBlock)->m_iMax;
}
//将属性放置传输数据中
void CSawtoothBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Sawtooth");
	str.Format("%d",m_iMin);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iMax);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}