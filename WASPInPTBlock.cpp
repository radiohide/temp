// WASPInPTBlock.cpp: implementation of the CWASPInPTBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPTBlock.h"
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
CWASPInPTBlock::CWASPInPTBlock()
{
	SetBlockType(_T( "WASPInPT" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知压力(MPa)和温度(℃)，求比焓(H)");m_strArrFunName.Add("H_PT");m_strArrOutPortName.Add("H");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的比熵(S)");m_strArrFunName.Add("S_PT");m_strArrOutPortName.Add("S");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的比容(V)");m_strArrFunName.Add("V_PT");m_strArrOutPortName.Add("V");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的定压质量比热容(Cp)");m_strArrFunName.Add("Cp_PT");m_strArrOutPortName.Add("Cp");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的定容质量比热容(Cv)");m_strArrFunName.Add("Cv_PT ");m_strArrOutPortName.Add("Cv");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的内能(E)");m_strArrFunName.Add("E_PT");m_strArrOutPortName.Add("E");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的音速(SSp)");m_strArrFunName.Add("SSp_PT");m_strArrOutPortName.Add("SSp");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的定熵指数(Ks)");m_strArrFunName.Add("Ks_PT");m_strArrOutPortName.Add("Ks");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的动力粘度(Eta)");m_strArrFunName.Add("Eta_PT");m_strArrOutPortName.Add("Eta");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的运动粘度(U)");m_strArrFunName.Add("U_PT");m_strArrOutPortName.Add("U");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的导热系数(Ramd)");m_strArrFunName.Add("Ramd_PT");m_strArrOutPortName.Add("Ramd");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的普朗特数(Prn)");m_strArrFunName.Add("Prn_PT");m_strArrOutPortName.Add("Prn");
	m_strArrFunDisp.Add("已知压力(P)和温度(T),求取对应的介电常数(Eps)");m_strArrFunName.Add("Eps_PT");m_strArrOutPortName.Add("Eps");
	
	m_strBlockDisp="已知压力温度求其他水和水蒸气性质.";//计算块描述

	ConstructBlockInputOutput ();
}

CWASPInPTBlock::~CWASPInPTBlock()
{
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInPTBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
CCalcBlock* CWASPInPTBlock::CreateFromString( const CString& strType )
{
	CWASPInPTBlock* obj = new CWASPInPTBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
