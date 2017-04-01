// WASPInTSBlock.cpp: implementation of the CWASPInTSBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInTSBlock.h"
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
CWASPInTSBlock::CWASPInTSBlock()
{
	SetBlockType(_T( "WASPInTS" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ��ѹ��(P)(��ѹ)");m_strArrFunName.Add("PLP_TS");m_strArrOutPortName.Add("PLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ�ı���(H)(��ѹ)");m_strArrFunName.Add("HLP_TS");m_strArrOutPortName.Add("HLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ�ı���(V)(��ѹ)");m_strArrFunName.Add("VLP_TS");m_strArrOutPortName.Add("VLP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ��ѹ��(P)(��ѹ)");m_strArrFunName.Add("PHP_TS");m_strArrOutPortName.Add("PHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ�ı���(H)(��ѹ)");m_strArrFunName.Add("HHP_TS");m_strArrOutPortName.Add("HHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ�ı���(V)(��ѹ)");m_strArrFunName.Add("VHP_TS");m_strArrOutPortName.Add("VHP");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(S),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_TS");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪�¶ȱ���������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInTSBlock::~CWASPInTSBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInTSBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("S"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInTSBlock::CreateFromString( const CString& strType )
{
	CWASPInTSBlock* obj = new CWASPInTSBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
