// PLossRateBlock.cpp: implementation of the CPLossRateBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PLossRateBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLossRateBlock::CPLossRateBlock()
{
	SetBlockType(_T( "PLossRate" ) );//设置类型
	ConstructBlockInputOutput ();
}

CPLossRateBlock::~CPLossRateBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CPLossRateBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("Extr"), VALUE_DOUBLE);
	AddInputPort(_T("InStm"), VALUE_DOUBLE);
	AddOutputPort("PLossR",VALUE_DOUBLE);
}
//根据类型新建对象的函数
CCalcBlock* CPLossRateBlock::CreateFromString( const CString& strType )
{
	CPLossRateBlock* obj = new CPLossRateBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CPLossRateBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CPLossRateBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double extValue,inValue;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&extValue);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue);
	
	CUniValue UniVar;
	UniVar.AddData((extValue-inValue)/extValue);
	pOutPortObj->SetPortUniValue(UniVar);
	//输出到工作空间中
	OutputResultToGlobalWS();
}
