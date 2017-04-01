// WASPInPVBlock.cpp: implementation of the CWASPInPVBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPVBlock.h"
#include "PropertyDlgWASP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPtrArray ListPoteDll;

CWASPInPVBlock::CWASPInPVBlock()
{
	SetBlockType(_T( "WASPInPV" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(V),��ȡ��Ӧ���¶�(T)");m_strArrFunName.Add("T_PV");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(V),��ȡ��Ӧ�ı���(H)");m_strArrFunName.Add("H_PV");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(V),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_PV");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(V),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_PV");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪ѹ������������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInPVBlock::~CWASPInPVBlock()
{
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInPVBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("V"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInPVBlock::CreateFromString( const CString& strType )
{
	CWASPInPVBlock* obj = new CWASPInPVBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
