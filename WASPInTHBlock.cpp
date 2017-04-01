// WASPInTHBlock.cpp: implementation of the CWASPInTHBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInTHBlock.h"
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
CWASPInTHBlock::CWASPInTHBlock()
{
	SetBlockType(_T( "WASPInTH" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ��ѹ��(P)(��ѹ)");m_strArrFunName.Add("PLP_TH");m_strArrOutPortName.Add("PLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ��ѹ��(P)(��ѹ)");m_strArrFunName.Add("PHP_TH");m_strArrOutPortName.Add("PHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ�ı���(S)(��ѹ)");m_strArrFunName.Add("SLP_TH");m_strArrOutPortName.Add("SLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ�ı���(S)(��ѹ)");m_strArrFunName.Add("SHP_TH");m_strArrOutPortName.Add("SHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ�ı���(V)(��ѹ)");m_strArrFunName.Add("VLP_TH");m_strArrOutPortName.Add("VLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ�ı���(V)(��ѹ)");m_strArrFunName.Add("VHP_TH");m_strArrOutPortName.Add("VHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(H),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_TH");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪�¶ȱ���������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInTHBlock::~CWASPInTHBlock()
{
	
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInTHBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("H"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInTHBlock::CreateFromString( const CString& strType )
{
	CWASPInTHBlock* obj = new CWASPInTHBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
