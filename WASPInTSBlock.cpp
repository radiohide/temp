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
	SetBlockType(_T( "WASPInTS" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的压力(P)(低压)");m_strArrFunName.Add("PLP_TS");m_strArrOutPortName.Add("PLP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的比焓(H)(低压)");m_strArrFunName.Add("HLP_TS");m_strArrOutPortName.Add("HLP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的比容(V)(低压)");m_strArrFunName.Add("VLP_TS");m_strArrOutPortName.Add("VLP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的压力(P)(高压)");m_strArrFunName.Add("PHP_TS");m_strArrOutPortName.Add("PHP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的比焓(H)(高压)");m_strArrFunName.Add("HHP_TS");m_strArrOutPortName.Add("HHP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的比容(V)(高压)");m_strArrFunName.Add("VHP_TS");m_strArrOutPortName.Add("VHP");
	m_strArrFunDisp.Add("已知温度(T)和比熵(S),求取对应的干度(X)");m_strArrFunName.Add("X_TS");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知温度比熵求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInTSBlock::~CWASPInTSBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInTSBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("S"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
