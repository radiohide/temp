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
	SetBlockType(_T( "WASPInTX" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知温度(T)和干度(X),求取对应的压力(P)");m_strArrFunName.Add("P_TX");m_strArrOutPortName.Add("P");
	m_strArrFunDisp.Add("已知温度(T)和干度(X),求取对应的比焓(H)");m_strArrFunName.Add("H_TX");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("已知温度(T)和干度(X),求取对应的比熵(S)");m_strArrFunName.Add("S_TX");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("已知温度(T)和干度(X),求取对应的比容(V)");m_strArrFunName.Add("V_TX");m_strArrOutPortName.Add("V");
	
	m_strBlockDisp="已知温度干度求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInTXBlock::~CWASPInTXBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInTXBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("X"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
