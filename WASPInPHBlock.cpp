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
	SetBlockType(_T( "WASPInPH" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知压力(P)和比焓(H),求取对应的温度(T)");m_strArrFunName.Add("T_PH");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("已知压力(P)和比焓(H),求取对应的比熵(S)");m_strArrFunName.Add("S_PH");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("已知压力(P)和比焓(H),求取对应的比容(V)");m_strArrFunName.Add("V_PH");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("已知压力(P)和比焓(H),求取对应的干度(X)");m_strArrFunName.Add("X_PH");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知压力比焓求其他水和水蒸气性质.";//计算块描述
	ConstructBlockInputOutput ();
}

CWASPInPHBlock::~CWASPInPHBlock()
{
	
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInPHBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("H"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("Out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
