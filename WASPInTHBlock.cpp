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
	SetBlockType(_T( "WASPInTH" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的压力(P)(低压)");m_strArrFunName.Add("PLP_TH");m_strArrOutPortName.Add("PLP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的压力(P)(高压)");m_strArrFunName.Add("PHP_TH");m_strArrOutPortName.Add("PHP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的比熵(S)(低压)");m_strArrFunName.Add("SLP_TH");m_strArrOutPortName.Add("SLP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的比熵(S)(高压)");m_strArrFunName.Add("SHP_TH");m_strArrOutPortName.Add("SHP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的比容(V)(低压)");m_strArrFunName.Add("VLP_TH");m_strArrOutPortName.Add("VLP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的比容(V)(高压)");m_strArrFunName.Add("VHP_TH");m_strArrOutPortName.Add("VHP");
	m_strArrFunDisp.Add("已知温度(T)和比焓(H),求取对应的干度(X)");m_strArrFunName.Add("X_TH");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知温度比焓求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInTHBlock::~CWASPInTHBlock()
{
	
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInTHBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("H"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
