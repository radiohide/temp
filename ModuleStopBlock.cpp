// ModuleStopBlock.cpp: implementation of the CModuleStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ModuleStopBlock.h"
#include "PropertyDlgStop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModuleStopBlock::CModuleStopBlock()
{
	SetBlockType(_T( "ModuleStop" ) );//��������
	ConstructBlockInputOutput();
}

CModuleStopBlock::~CModuleStopBlock()
{
	
}
void CModuleStopBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in"), _T("in"));
}
//���������½�����ĺ���
CCalcBlock* CModuleStopBlock::CreateFromString( const CString& strType )
{
	CModuleStopBlock* obj = new CModuleStopBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CModuleStopBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CModuleStopBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObj = GetInputPortObj(0);
	double dVar;
	pPortObj->GetPortUniValue().GetDoubleVal(&dVar);
	if(dVar!=0)
	{
		this->m_pModule->StopCalc();//ģ��ֹͣ
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CModuleStopBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStop dlg;
	dlg.m_StrBlockDespText="������Ϊ����ֵʱ��ģ������ֹͣ��";
	dlg.m_StrWinText="ģ��ֹͣ��";
	dlg.DoModal();
}