// PExtStreamBlock.cpp: implementation of the CPExtStreamBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PExtStreamBlock.h"
#include "PropertyDlgPExtStm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPExtStreamBlock::CPExtStreamBlock()
{
	SetBlockType(_T( "PExtStream" ) );//��������
	ConstructBlockInputOutput ();
}

CPExtStreamBlock::~CPExtStreamBlock()
{
	
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CPExtStreamBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("InStm"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("PLossR"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("Extr",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CPExtStreamBlock::CreateFromString( const CString& strType )
{
	CPExtStreamBlock* obj = new CPExtStreamBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CPExtStreamBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CPExtStreamBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue,pLossR;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&pLossR);
	
	CUniValue UniVar;
	if(!m_bBeIgnore)
	{
		UniVar.AddData(inValue/(1-pLossR));
	}
	else
		UniVar.AddData(inValue);
	pOutPortObj->SetPortUniValue(UniVar);
	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CPExtStreamBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("beignore")==0)
	{
		if(!strItem1.IsEmpty())  m_bBeIgnore=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CPExtStreamBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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
void CPExtStreamBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
void CPExtStreamBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_bBeIgnore=((CPExtStreamBlock*)pBlock)->m_bBeIgnore;
}
//�����Է��ô���������
void CPExtStreamBlock::SetBlockPropertyToTransferData(
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
