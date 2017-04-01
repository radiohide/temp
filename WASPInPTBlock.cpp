// WASPInPTBlock.cpp: implementation of the CWASPInPTBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPTBlock.h"
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
CWASPInPTBlock::CWASPInPTBlock()
{
	SetBlockType(_T( "WASPInPT" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪ѹ��(MPa)���¶�(��)�������(H)");m_strArrFunName.Add("H_PT");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_PT");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�ı���(V)");m_strArrFunName.Add("V_PT");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�Ķ�ѹ����������(Cp)");m_strArrFunName.Add("Cp_PT");m_strArrOutPortName.Add("Cp");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�Ķ�������������(Cv)");m_strArrFunName.Add("Cv_PT ");m_strArrOutPortName.Add("Cv");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ������(E)");m_strArrFunName.Add("E_PT");m_strArrOutPortName.Add("E");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ������(SSp)");m_strArrFunName.Add("SSp_PT");m_strArrOutPortName.Add("SSp");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�Ķ���ָ��(Ks)");m_strArrFunName.Add("Ks_PT");m_strArrOutPortName.Add("Ks");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�Ķ���ճ��(Eta)");m_strArrFunName.Add("Eta_PT");m_strArrOutPortName.Add("Eta");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ���˶�ճ��(U)");m_strArrFunName.Add("U_PT");m_strArrOutPortName.Add("U");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�ĵ���ϵ��(Ramd)");m_strArrFunName.Add("Ramd_PT");m_strArrOutPortName.Add("Ramd");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ����������(Prn)");m_strArrFunName.Add("Prn_PT");m_strArrOutPortName.Add("Prn");
	m_strArrFunDisp.Add("��֪ѹ��(P)���¶�(T),��ȡ��Ӧ�Ľ�糣��(Eps)");m_strArrFunName.Add("Eps_PT");m_strArrOutPortName.Add("Eps");
	
	m_strBlockDisp="��֪ѹ���¶�������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInPTBlock::~CWASPInPTBlock()
{
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInPTBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInPTBlock::CreateFromString( const CString& strType )
{
	CWASPInPTBlock* obj = new CWASPInPTBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
