// WASPInPXBlock.cpp: implementation of the CWASPInPXBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPXBlock.h"
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
CWASPInPXBlock::CWASPInPXBlock()
{
	SetBlockType(_T( "WASPInPX" ) );//��������
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("��֪ѹ��(P)�͸ɶ�(X),��ȡ��Ӧ���¶�(T)");m_strArrFunName.Add("T_PX");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("��֪ѹ��(P)�͸ɶ�(X),��ȡ��Ӧ�ı���(H)");m_strArrFunName.Add("H_PX");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("��֪ѹ��(P)�͸ɶ�(X),��ȡ��Ӧ�ı���(S)");m_strArrFunName.Add("S_PX");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("��֪ѹ��(P)�͸ɶ�(X),��ȡ��Ӧ�ı���(V)");m_strArrFunName.Add("V_PX");m_strArrOutPortName.Add("V");
	
	m_strBlockDisp="��֪ѹ���ɶ�������ˮ��ˮ��������.";//���������

	ConstructBlockInputOutput ();
}

CWASPInPXBlock::~CWASPInPXBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWASPInPXBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("X"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CWASPInPXBlock::CreateFromString( const CString& strType )
{
	CWASPInPXBlock* obj = new CWASPInPXBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
