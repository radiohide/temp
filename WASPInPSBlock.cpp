// WASPInPSBlock.cpp: implementation of the CWASPInPSBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPSBlock.h"
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
CWASPInPSBlock::CWASPInPSBlock()
{
	SetBlockType(_T( "WASPInPS" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(S),��ȡ��Ӧ���¶�(T)");m_strArrFunName.Add("T_PS");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(S),��ȡ��Ӧ�ı���(H)");m_strArrFunName.Add("H_PS");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(S),��ȡ��Ӧ�ı���(V)");m_strArrFunName.Add("V_PS");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(S),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_PS");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪ѹ������������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInPSBlock::~CWASPInPSBlock()
{
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInPSBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("S"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInPSBlock::CreateFromString( const CString& strType )
{
	CWASPInPSBlock* obj = new CWASPInPSBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
