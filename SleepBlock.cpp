// SleepBlock.cpp: implementation of the CSleepBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SleepBlock.h"
#include "PropertyDlgSleep.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSleepBlock::CSleepBlock()
{
	SetBlockType(_T("Sleep") );
	ConstructBlockInputOutput();
	m_iSleepMS=1000;

	m_bShowDesp=TRUE;//�Ƿ���ʾDESP
	m_bShowInPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
	m_bShowOutPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
}

CSleepBlock::~CSleepBlock()
{

}
void CSleepBlock::ConstructBlockInputOutput ()
{

}
	//���������½�����ĺ���
CCalcBlock* CSleepBlock::CreateFromString( const CString& strType )
{
	CSleepBlock* obj = new CSleepBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CSleepBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iSleepMS=((CSleepBlock*)pBlock)->m_iSleepMS;
}
	//��ʼ������ĺ���������ĳ�ʼ��
void CSleepBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//���㺯����ʵ�ֱ�ģ��ļ���
void CSleepBlock::Calc() //���м���ĺ���,��Ҫ����
{
	if(m_iSleepMS>0)
		Sleep(m_iSleepMS);
}
//������ͼ��
void CSleepBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect Ϊblock������ο�,���в�д����ֵ
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	str.Format(_T("%d ms"), m_iSleepMS);
	//str.TrimRight(_T("0")); 
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
void CSleepBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//�����Է��ô���������
{
	
}
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CSleepBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("SleepValue")==0)
	{
		if(!strItem1.IsEmpty())
			m_iSleepMS=atoi(strItem1);
	}	
}
	//��������ʾ��ͬ�����Զ���Ի���
void CSleepBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgSleep dlg;
    //���Ի��������ֵ
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_iSleep=m_iSleepMS;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_iSleepMS = dlg.m_iSleep;
		
	}
	//////////////////////////////////////////////////
}
	//�����Խ���ģ��д�뵽���ݿ���
void CSleepBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("SleepValue"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_iSleepMS) );
	pRecord->Update();
}
