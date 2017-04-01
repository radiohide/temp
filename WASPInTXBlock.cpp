// WASPInTXBlock.cpp: implementation of the CWASPInTXBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInTXBlock.h"
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
CWASPInTXBlock::CWASPInTXBlock()
{
	SetBlockType(_T( "WASPInTX" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪�¶�(T)�͸ɶ�(X),��ȡ��Ӧ��ѹ��(P)");m_strArrFunName.Add("P_TX");m_strArrOutPortName.Add("P");
	m_strArrFunDisp.Add("��֪�¶�(T)�͸ɶ�(X),��ȡ��Ӧ�ı���(H)");m_strArrFunName.Add("H_TX");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪�¶�(T)�͸ɶ�(X),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_TX");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪�¶�(T)�͸ɶ�(X),��ȡ��Ӧ�ı���(V)");m_strArrFunName.Add("V_TX");m_strArrOutPortName.Add("V");
	
	m_strBlockDisp="��֪�¶ȸɶ�������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInTXBlock::~CWASPInTXBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInTXBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("X"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInTXBlock::CreateFromString( const CString& strType )
{
	CWASPInTXBlock* obj = new CWASPInTXBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
