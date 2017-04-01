// RandomBlock.cpp: implementation of the CRandomBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RandomBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandomBlock::CRandomBlock()
{
	m_dMin =0;
	m_dMax =10;
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "Random" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===
}

CRandomBlock::~CRandomBlock()
{

}
//构造初始的输入输出个数
void CRandomBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CRandomBlock::CreateFromString( const CString& strType )
{
	CRandomBlock* obj = new CRandomBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CRandomBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	srand(time(0));
}
//计算函数，实现本模块的计算
void CRandomBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	double m_Result=0; 
	//m_Result=m_dMin+rand()*(m_dMax-m_dMin);
	//m_Result=m_dMin+rand()%(m_dMax-m_dMin+1);
	m_Result=m_dMin+rand()/(double)(m_dMax/10-m_dMin+1);
	///////////////////////////////////
  	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	UniVar.AddData(m_Result);
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRandomBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Random")==0)
	{
		if(!strItem1.IsEmpty())
			m_dMin=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_dMax=atoi(strItem2);	
	}
}
//重载以显示不同的属性对象对话框
void CRandomBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRandom  dlg;
	
	dlg.m_dMin =m_dMin;
	dlg.m_dMax=m_dMax;
    ///
	if(dlg.DoModal()==IDOK)
	{
		m_dMin=dlg.m_dMin;
		m_dMax=dlg.m_dMax;
	}
}
//重载以将本模块写入到数据库中
void CRandomBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("Random"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_dMin) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t((double)m_dMax) );

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CRandomBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dMin=((CRandomBlock*)pBlock)->m_dMin;
	m_dMax=((CRandomBlock*)pBlock)->m_dMax;
}
//将属性放置传输数据中
void CRandomBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Random");
	str.Format("%f",m_dMin);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dMax);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}