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
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "Random" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===
}

CRandomBlock::~CRandomBlock()
{

}
//�����ʼ�������������
void CRandomBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
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
//��ʼ������ĺ���������ĳ�ʼ��
void CRandomBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	srand(time(0));
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CRandomBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
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
	//�������
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
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
//��������ʾ��ͬ�����Զ���Ի���
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
//�����Խ���ģ��д�뵽���ݿ���
void CRandomBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("Random"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_dMin) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t((double)m_dMax) );

	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CRandomBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dMin=((CRandomBlock*)pBlock)->m_dMin;
	m_dMax=((CRandomBlock*)pBlock)->m_dMax;
}
//�����Է��ô���������
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