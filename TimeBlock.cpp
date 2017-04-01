// TimeBlock.cpp: implementation of the CTimeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "TimeBlock.h"
#include "PropertyDlgTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeBlock::CTimeBlock()
{
	SetBlockType(_T("Time") );
	m_strDateTime="";
	m_iTheTimeOrCurrent=0;
	ConstructBlockInputOutput();
}

CTimeBlock::~CTimeBlock()
{

}

void CTimeBlock::ConstructBlockInputOutput ()
{
	//�����ɶ�����������ı������ơ�
	AddOutputPort("T", VALUE_DOUBLE);
}
//���������½�����ĺ���
CCalcBlock* CTimeBlock::CreateFromString( const CString& strType )
{
	CTimeBlock* obj = new CTimeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CTimeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CTimeBlock::Calc()//���м���ĺ���,��Ҫ����
{
	CUniValue UniVar1;
	CCalcPort *pOutPortObj=GetOutputPortObj(0);
	if(0==m_iTheTimeOrCurrent)//��ǰʱ��
	{
		UniVar1.AddData(CTime::GetCurrentTime().GetTime());
	}
	else
	{
		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;    
		sscanf(m_strDateTime, "%d-%d-%d %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		CTime tDateTime(nYear,   nMonth,   nDate,nHour,nMin,nSec);
		UniVar1.AddData(tDateTime.GetTime());
	}
	pOutPortObj->SetPortUniValue(UniVar1);
   //����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CTimeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("BeTheTimeOrCurrent")==0)
	{
		if(!strItem1.IsEmpty())
			m_iTheTimeOrCurrent=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_strDateTime=strItem2;
		else
			m_strDateTime="";
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CTimeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgTime dlg;
	dlg.m_iTheTimeOrCurrent=this->m_iTheTimeOrCurrent;
	dlg.m_strDateTime=m_strDateTime;
	if(IDOK==dlg.DoModal())
	{
		this->m_iTheTimeOrCurrent=dlg.m_iTheTimeOrCurrent;
		m_strDateTime=dlg.m_strDateTime;
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}

	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CTimeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("BeTheTimeOrCurrent"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	strField.Format("%d",m_iTheTimeOrCurrent); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );
	strField.Format("%s",m_strDateTime); 
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strField) );
		
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CTimeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iTheTimeOrCurrent=((CTimeBlock*)pBlock)->m_iTheTimeOrCurrent;
	m_strDateTime=((CTimeBlock*)pBlock)->m_strDateTime;
}
//�����Է��ô���������
void CTimeBlock::SetBlockPropertyToTransferData(
											CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeTheTimeOrCurrent");
	
	CString str;
	str.Format("%d",m_iTheTimeOrCurrent);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strDateTime);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}
