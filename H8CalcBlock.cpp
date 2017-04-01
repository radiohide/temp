// H8CalcBlock.cpp: implementation of the CH8CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "H8CalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CH8CalcBlock::CH8CalcBlock()
{
	SetBlockType(_T("H8Calc"));
	ConstructBlockInputOutput();
}

CH8CalcBlock::~CH8CalcBlock()
{

}


//构造初始的输入输出个数
void CH8CalcBlock::ConstructBlockInputOutput()
{
	//具有3个输出
	AddOutputPort("PerX_h8thExtr",VALUE_DOUBLE,"");//8级焓值
	AddOutputPort("PerX_x8thExtrStm",VALUE_DOUBLE,"");//八段抽汽干度
	AddOutputPort("PerX_effExtr78",VALUE_DOUBLE,"");//78抽级段效率

	//具有5个输入 
	AddInputPort(_T("PudX_x8Extr"), VALUE_DOUBLE, _T("八段干度设计值"), _T(""));	//八段干度设计值
	AddInputPort(_T("PudX_eff78Extr"), VALUE_DOUBLE, _T("78段级段效率设计值"), _T(""));	//78段级段效率设计值
	AddInputPort(_T("PerX_hi8thExtr"), VALUE_DOUBLE, _T("八段抽汽等熵焓"), _T(""));	//八段抽汽等熵焓
	AddInputPort(_T("PerX_h7thExtr"), VALUE_DOUBLE, _T("七段抽气焓值"), _T(""));	//七段抽气焓值
	AddInputPort(_T("PubX_p8thExtr"), VALUE_DOUBLE, _T("#8抽汽压力"), _T(""));	//#8抽汽压力

}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CH8CalcBlock::CreateFromString( const CString& strType )
{
	CH8CalcBlock* obj = new CH8CalcBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CH8CalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

double bph_x(double para1,double para2)
{
	typedef void (_stdcall *PH2X97)(double P,double H,double & X,int & Range);
	PH2X97 proc;
	HINSTANCE hst = ::LoadLibrary("WASP.dll");
	if(!hst)
		return 0;
	proc = (PH2X97)::GetProcAddress(hst, "PH2X97");
	if(!proc)
		return 0;
	//第1个测点值表示压力
	double P=para1;
	//第2个测点值表示比焓
	double T=para2;
	double H=0.0;
	int R=0;
	
	//直接调用97标准的函数
	proc(P,T,H,R);
	//printf("直接调用97标准的函数，h=%10.5f\n",H);
	return H;
}
//计算函数，实现本模块的计算
void CH8CalcBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	/////////////////////////////////////
	//计算输出

	CCalcPort *pPortObj1 = GetOutputPortObj("PerX_h8thExtr"); //8级焓值
	CCalcPort *pPortObj2 = GetOutputPortObj("PerX_x8thExtrStm"); //八段抽汽干度
	CCalcPort *pPortObj3 = GetOutputPortObj("PerX_effExtr78"); //78抽级段效率
	if(pPortObj1 && pPortObj2 && pPortObj3)
	{
		double result=0;
		CUniValue a,b,c,d,e;
		BOOL ret1 = GetInputValue("PudX_x8Extr",&a);//八段干度设计值
		BOOL ret2 = GetInputValue("PudX_eff78Extr",&b);//78段级段效率设计值
		BOOL ret3 = GetInputValue("PerX_hi8thExtr",&c);//八段抽汽等熵焓
		BOOL ret4 = GetInputValue("PubX_p8thExtr",&d);//#8抽汽压力
		BOOL ret5 = GetInputValue("PerX_h7thExtr",&e);//7级焓值

		if(ret1 && ret2 && ret3 && ret4 && ret5)
		{
			double x8_0,Eta78_0,hi8,p8,h7;
			x8_0 = a.GetValueDouble()/100;	//八段干度设计值
			Eta78_0 = b.GetValueDouble()/100;//78段级段效率设计值
			hi8 = c.GetValueDouble();//八段抽汽等熵焓
		    p8 = d.GetValueDouble(); //#8抽汽压力
			h7 = e.GetValueDouble();//7级焓值

			//第8级焓值
			double Eta78 = Eta78_0/((1+x8_0)/2);
			double h80 = h7-Eta78*(h7-hi8);	//h80初始值
			double x8,h8;

			int numCycle = 0;
			do
			{
				numCycle++;
				x8 = bph_x(p8,h80);

				h8 = h80+(1-x8)*(h7-hi8);

				if(abs(h8-h80) < 0.001)
					break;
				h80 = h8;
				if(numCycle > 10)	//循环超限
					break;
			}
			while(true);//10的负3次方
			//1)	η78为当前的78抽级段效率；
			//2)	x8为八段抽汽干度；
			//3)	h8为八段抽汽焓值。

			CUniValue UniVar1;
			UniVar1.AddData(h8);
			pPortObj1->SetPortUniValue(UniVar1);

			CUniValue UniVar2;
			UniVar1.AddData(x8);
			pPortObj2->SetPortUniValue(UniVar2);

			CUniValue UniVar3;
			UniVar1.AddData(Eta78);
			pPortObj3->SetPortUniValue(UniVar3);
		}
	}

	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}