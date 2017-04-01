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
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "Sawtooth" ) );
	ConstructBlockInputOutput();
}

CSawtoothBlock::~CSawtoothBlock()
{
	
}
//�����ʼ�������������
void CSawtoothBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
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

//��ʼ������ĺ���������ĳ�ʼ��
void CSawtoothBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CSawtoothBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
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
	//�������
	OutputResultToGlobalWS();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
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

//��������ʾ��ͬ�����Զ���Ի���
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
//�����Խ���ģ��д�뵽���ݿ���
void CSawtoothBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("Sawtooth"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	CString str;
	str.Format("%d",m_iMin);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(str));
	str.Format("%d",m_iMax);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(str));
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CSawtoothBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iMin=((CSawtoothBlock*)pBlock)->m_iMin;
	m_iMax=((CSawtoothBlock*)pBlock)->m_iMax;
}
//�����Է��ô���������
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