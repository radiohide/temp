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
	SetBlockType(_T( "WASPInPV" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知压力(P)和比容(V),求取对应的温度(T)");m_strArrFunName.Add("T_PV");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("已知压力(P)和比容(V),求取对应的比焓(H)");m_strArrFunName.Add("H_PV");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("已知压力(P)和比容(V),求取对应的比熵(S)");m_strArrFunName.Add("S_PV");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("已知压力(P)和比容(V),求取对应的干度(X)");m_strArrFunName.Add("X_PV");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知压力比容求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInPVBlock::~CWASPInPVBlock()
{
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInPVBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("V"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
