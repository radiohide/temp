// StopBlock.cpp: implementation of the CStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StopBlock.h"
#include "PropertyDlgStop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStopBlock::CStopBlock()
{
	SetBlockType(_T( "Stop" ) );//��������
	ConstructBlockInputOutput();
}

CStopBlock::~CStopBlock()
{

}
void CStopBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in"), _T("in"));
}
//���������½�����ĺ���
CCalcBlock* CStopBlock::CreateFromString( const CString& strType )
{
	CStopBlock* obj = new CStopBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStopBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStopBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObj = GetInputPortObj(0);
	double dVar;
	pPortObj->GetPortUniValue().GetDoubleVal(&dVar);
	if(dVar!=0)
	{
		this->m_pModule->StopCalc();//ģ��ֹͣ
		this->m_pModel->StopCalc();//ģ��ֹͣ
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStopBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStop dlg;
	dlg.DoModal();
}