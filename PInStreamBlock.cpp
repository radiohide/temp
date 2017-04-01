// PInStreamBlock.cpp: implementation of the CPInStreamBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PInStreamBlock.h"
#include "PropertyDlgPExtStm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPInStreamBlock::CPInStreamBlock()
{
	SetBlockType(_T( "PInStream" ) );//设置类型
	ConstructBlockInputOutput ();
}

CPInStreamBlock::~CPInStreamBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CPInStreamBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("Extr"), VALUE_DOUBLE);
	AddInputPort(_T("PLossR"), VALUE_DOUBLE);
	AddOutputPort("InStm",VALUE_DOUBLE);
}
//根据类型新建对象的函数
CCalcBlock* CPInStreamBlock::CreateFromString( const CString& strType )
{
	CPInStreamBlock* obj = new CPInStreamBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CPInStreamBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CPInStreamBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double extrValue,pLossR;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&extrValue);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&pLossR);
	
	CUniValue UniVar;
	if(!m_bBeIgnore)
	{
		UniVar.AddData(extrValue*(1-pLossR));
	}
	else
		UniVar.AddData(extrValue);
	pOutPortObj->SetPortUniValue(UniVar);
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CPInStreamBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("beignore")==0)
	{
		if(!strItem1.IsEmpty())  m_bBeIgnore=atoi(strItem1);
	}
}
//重载以显示不同的属性对象对话框
void CPInStreamBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgPExtStm dlg;
	CString strTagName;
    //给对话框变量赋值
	if(m_bBeIgnore==1)
		dlg.m_bBeIgnore = true;
	else
		dlg.m_bBeIgnore = false;
	dlg.SetDlgText("进气压力计算块");
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_bBeIgnore)
			m_bBeIgnore=1;
		else
			m_bBeIgnore=0;
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
void CPInStreamBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="beignore";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_bBeIgnore); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CPInStreamBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_bBeIgnore=((CPInStreamBlock*)pBlock)->m_bBeIgnore;
}
//将属性放置传输数据中
void CPInStreamBlock::SetBlockPropertyToTransferData(
					CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"beignore");
	
	CString str;
	str.Format("%d",m_bBeIgnore);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
