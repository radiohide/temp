// H7CalcBlock.cpp: implementation of the CH7CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "H7CalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CH7CalcBlock::CH7CalcBlock()
{
	SetBlockType(_T("H7Calc"));
	ConstructBlockInputOutput();
}

CH7CalcBlock::~CH7CalcBlock()
{

}


//构造初始的输入输出个数
void CH7CalcBlock::ConstructBlockInputOutput()
{
	//具有1个输出
	AddOutputPort("PerX_h7thExtr",VALUE_DOUBLE,"");
	//具有3个输入 P,T 
	AddInputPort(_T("PerX_h6thExtr"), VALUE_DOUBLE, _T("六段抽汽焓值"), _T(""));	//六段抽汽焓值
	AddInputPort(_T("PerX_hi7thExtr"), VALUE_DOUBLE, _T("七段抽汽等熵焓"), _T(""));	//七段抽汽等熵焓
	AddInputPort(_T("PudX_eff67Extr"), VALUE_DOUBLE, _T("六七段级段效率设计值"), _T(""));	//六七段级段效率设计值
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CH7CalcBlock::CreateFromString( const CString& strType )
{
	CH7CalcBlock* obj = new CH7CalcBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CH7CalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CH7CalcBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	/////////////////////////////////////
	//计算输出

	//第7级焓值	h7 = h6-(h6-(hi7)*Eta67_0)

	CCalcPort *pPortObj = GetOutputPortObj("PerX_h7thExtr"); 
	if(pPortObj)
	{
		double result=0;
		CUniValue a;
		CUniValue b;
		CUniValue c;

		BOOL ret1 = GetInputValue("PerX_h6thExtr",&a);	//六段抽汽焓值
		BOOL ret2 = GetInputValue("PerX_hi7thExtr",&b);	//七段抽汽等熵焓
		BOOL ret3 = GetInputValue("PudX_eff67Extr",&c);	//六七段级段效率设计值
		if(ret1 && ret2 && ret3)
		{
			double h6 =	a.GetValueDouble();	//六段抽汽焓值
			double hi7 = b.GetValueDouble() ;	//七段抽汽等熵焓
			double Eta67_0 = c.GetValueDouble() ;//六七段级段效率设计值

			double h7 = h6-(h6-(hi7)*Eta67_0);
			CUniValue UniVar;
			UniVar.AddData(h7);
			pPortObj->SetPortUniValue(UniVar);
		}
	}
	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}