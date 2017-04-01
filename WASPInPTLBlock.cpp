// WASPInPTLBlock.cpp: implementation of the CWASPInPTLBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPInPTLBlock.h"
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
CWASPInPTLBlock::CWASPInPTLBlock()
{
	SetBlockType(_T( "WASPInPTL" ) );//设置类型
	m_strFuncName="";
	PSourceValue=new TAGINFO[3];
	pG = new TAGINFO;

	m_strArrFunDisp.Add("已知压力(P)、温度(T)和波长(Lamd),求取对应的折射率(N)");m_strArrFunName.Add("N_PT");m_strArrOutPortName.Add("N");
	
	m_strBlockDisp="已知压力温度和波长求其他水和水蒸气性质.";//计算块描述
	ConstructBlockInputOutput ();
}

CWASPInPTLBlock::~CWASPInPTLBlock()
{
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWASPInPTLBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("P"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("T"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddInputPort(_T("L"), VALUE_DOUBLE, _T("in3"), _T("in3"));
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
CCalcBlock* CWASPInPTLBlock::CreateFromString( const CString& strType )
{
	CWASPInPTLBlock* obj = new CWASPInPTLBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//计算函数，实现本模块的计算
void CWASPInPTLBlock::Calc() //进行计算的函数,需要重载
{
	rtFunc = NULL;
	for (int i=0; i<(ListPoteDll).GetSize(); i++)
	{
		CPoteDll* PoteDll = (CPoteDll*)((ListPoteDll)[i]);
		for (int flag=0; flag<PoteDll->lstPoteFuncName.GetSize();flag++)
		{
			if (PoteDll->lstPoteFuncName[flag] == m_strFuncName)
			{
				rtFunc = (RTFUNC)PoteDll->lstPoteFuncAddr[flag];
				if (rtFunc)	break;
			}
		}
		if (rtFunc)	break;
	}
	if(rtFunc)
	{
		GetInputValueFromGlobalWS();
		//计算过程
		CCalcPort *pOutPortObj = GetOutputPortObj(0);
		CCalcPort *pInput1Obj=GetInputPortObj(0);
		CCalcPort *pInput2Obj=GetInputPortObj(1);
		CCalcPort *pInput3Obj=GetInputPortObj(2);
		
		double inValue1,inValue2,inValue3;
		pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
		pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
		pInput3Obj->GetPortUniValue().GetDoubleVal(&inValue3);
		
		PSourceValue[0].dValue = inValue1;
		PSourceValue[0].wState = 1;
		PSourceValue[0].tTime=CTime::GetCurrentTime().GetTime();
		
		PSourceValue[1].dValue = inValue2;
		PSourceValue[1].wState = 1;
		PSourceValue[1].tTime=CTime::GetCurrentTime().GetTime();

		PSourceValue[2].dValue = inValue3;
		PSourceValue[2].wState = 1;
		PSourceValue[2].tTime=CTime::GetCurrentTime().GetTime();
		
		*pG = rtFunc(PSourceValue, 3);
		CUniValue UniVar;
		UniVar.AddData(pG->dValue);
		pOutPortObj->SetPortUniValue(UniVar);
		//输出到工作空间中
		OutputResultToGlobalWS();
	}
}
