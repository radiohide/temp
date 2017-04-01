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
	SetBlockType(_T( "WASPInTV" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知温度(T)和比容(V),求取对应的压力(P)");m_strArrFunName.Add("P_TV");m_strArrOutPortName.Add("P");
	m_strArrFunDisp.Add("已知温度(T)和比容(V),求取对应的比焓(H)");m_strArrFunName.Add("H_TV");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("已知温度(T)和比容(V),求取对应的比熵(S)");m_strArrFunName.Add("S_TV");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("已知温度(T)和比容(V),求取对应的干度(X)");m_strArrFunName.Add("X_TV");m_strArrOutPortName.Add("X");
	
	m_strBlockDisp="已知温度比容求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInTVBlock::~CWASPInTVBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInTVBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("V"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
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
