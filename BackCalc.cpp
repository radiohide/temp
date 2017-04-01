// BackCalc.cpp: implementation of the CBackCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "BackCalc.h"
#include "PropertyDlgBackCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackCalc::CBackCalc()
{
	SetBlockType(_T( "BackCalc" ) );
	ConstructBlockInputOutput();
}

CBackCalc::~CBackCalc()
{

}

//���������½�����ĺ���
CCalcBlock* CBackCalc::CreateFromString( const CString& strType )
{
	CBackCalc* obj = new CBackCalc;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CBackCalc::ConstructBlockInputOutput ()
{
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//��ʼ������ĺ���������ĳ�ʼ��
void CBackCalc::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CBackCalc::Calc()//���м���ĺ���,��Ҫ����
{
	//��ȫ�ֻ�ȡ���ֵ�������������׼��
	GetInputValueFromGlobalWS();
	
	CUniValue UniVar;
	UniVar.AddData(this->m_pModel->m_iBackCalcTag,0,0);
	
    CCalcPort *pOutPortObj = GetOutputPortObj(0);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CBackCalc::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{

}
//��������ʾ��ͬ�����Զ���Ի���
void CBackCalc::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgBackCalc dlg;
	dlg.m_iBackCalc = this->m_pModel->m_iBackCalcTag;
	dlg.DoModal();
}
//�����Խ���ģ��д�뵽���ݿ���
void CBackCalc::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}

//����ͼ��ʱ����Ҫ�������ԡ�
void CBackCalc::CopyPropertyData(CCalcBlock* pBlock)
{
}
//�����Է��ô���������
void CBackCalc::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
}
