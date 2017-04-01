// WASPInPHBlock.cpp: implementation of the CWASPInPHBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPHBlock.h"
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

CWASPInPHBlock::CWASPInPHBlock()
{
	SetBlockType(_T( "WASPInPH" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(H),��ȡ��Ӧ���¶�(T)");m_strArrFunName.Add("T_PH");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(H),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_PH");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(H),��ȡ��Ӧ�ı���(V)");m_strArrFunName.Add("V_PH");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("��֪ѹ��(P)�ͱ���(H),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_PH");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪ѹ������������ˮ��ˮ��������.";//���������
	ConstructBlockInputOutput ();
}

CWASPInPHBlock::~CWASPInPHBlock()
{
	
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInPHBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("H"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("Out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInPHBlock::CreateFromString( const CString& strType )
{
	CWASPInPHBlock* obj = new CWASPInPHBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
