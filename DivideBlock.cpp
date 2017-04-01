// DivideBlock.cpp: implementation of the CDivideBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DivideBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDivideBlock::CDivideBlock()
{
	SetBlockType(_T( "Divide" ) );
	ConstructBlockInputOutput();
}

CDivideBlock::~CDivideBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CDivideBlock::CreateFromString( const CString& strType )
{
	CDivideBlock* obj = new CDivideBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CDivideBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("D1"), VALUE_DOUBLE);
	AddInputPort(_T("D2"), VALUE_DOUBLE);
	//该模块无输入，当计算完毕后，输出为一个时间序列的数据，
	//可自由定义输入输出的变量名称。
	AddOutputPort("Out",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CDivideBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CDivideBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue1,inValue2;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
	
	CUniValue UniVar;
	if(0!=inValue2)
	{
		UniVar.AddData(inValue1/(inValue2));
	}
	else
		UniVar.AddData(0);
	pOutPortObj->SetPortUniValue(UniVar);
	//输出到工作空间中
	OutputResultToGlobalWS();
}
