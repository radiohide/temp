// WASPInTVBlock.cpp: implementation of the CWASPInTVBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInTVBlock.h"
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
CWASPInTVBlock::CWASPInTVBlock()
{
	SetBlockType(_T( "WASPInTV" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(V),��ȡ��Ӧ��ѹ��(P)");m_strArrFunName.Add("P_TV");m_strArrOutPortName.Add("P");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(V),��ȡ��Ӧ�ı���(H)");m_strArrFunName.Add("H_TV");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(V),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_TV");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪�¶�(T)�ͱ���(V),��ȡ��Ӧ�ĸɶ�(X)");m_strArrFunName.Add("X_TV");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="��֪�¶ȱ���������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInTVBlock::~CWASPInTVBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInTVBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("V"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInTVBlock::CreateFromString( const CString& strType )
{
	CWASPInTVBlock* obj = new CWASPInTVBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
