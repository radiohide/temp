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
	SetBlockType(_T( "WASPInPS" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知压力(P)和比熵(S),求取对应的温度(T)");m_strArrFunName.Add("T_PS");m_strArrOutPortName.Add("T");
	m_strArrFunDisp.Add("已知压力(P)和比熵(S),求取对应的比焓(H)");m_strArrFunName.Add("H_PS");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("已知压力(P)和比熵(S),求取对应的比容(V)");m_strArrFunName.Add("V_PS");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("已知压力(P)和比熵(S),求取对应的干度(X)");m_strArrFunName.Add("X_PS");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知压力比熵求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInPSBlock::~CWASPInPSBlock()
{
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInPSBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("S"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
