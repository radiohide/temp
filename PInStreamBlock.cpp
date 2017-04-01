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
	SetBlockType(_T( "PInStream" ) );//��������
	ConstructBlockInputOutput ();
}

CPInStreamBlock::~CPInStreamBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CPInStreamBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("Extr"), VALUE_DOUBLE);
	AddInputPort(_T("PLossR"), VALUE_DOUBLE);
	AddOutputPort("InStm",VALUE_DOUBLE);
}
//���������½�����ĺ���
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
//��ʼ������ĺ���������ĳ�ʼ��
void CPInStreamBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CPInStreamBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
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
	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CPInStreamBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("beignore")==0)
	{
		if(!strItem1.IsEmpty())  m_bBeIgnore=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CPInStreamBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgPExtStm dlg;
	CString strTagName;
    //���Ի��������ֵ
	if(m_bBeIgnore==1)
		dlg.m_bBeIgnore = true;
	else
		dlg.m_bBeIgnore = false;
	dlg.SetDlgText("����ѹ�������");
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_bBeIgnore)
			m_bBeIgnore=1;
		else
			m_bBeIgnore=0;
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
	}
	//////////////////////////////////////////////////
}
//�����Խ���ģ��д�뵽���ݿ���
void CPInStreamBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="beignore";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_bBeIgnore); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CPInStreamBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_bBeIgnore=((CPInStreamBlock*)pBlock)->m_bBeIgnore;
}
//�����Է��ô���������
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
