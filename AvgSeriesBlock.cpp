// AvgSeriesBlock.cpp: implementation of the CAvgSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "AvgSeriesBlock.h"
#include "PropertyDlgAvgSeries.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAvgSeriesBlock::CAvgSeriesBlock()
{
	SetBlockType(_T( "AvgSeries" ) );
	ConstructBlockInputOutput();
}

CAvgSeriesBlock::~CAvgSeriesBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CAvgSeriesBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Value"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//根据类型新建对象的函数
CCalcBlock* CAvgSeriesBlock::CreateFromString( const CString& strType )
{
	CAvgSeriesBlock* obj = new CAvgSeriesBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CAvgSeriesBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CAvgSeriesBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程

	CCalcPort *pPortObjIn3 = GetInputPortObj(0);	
	CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
	double dOutput=0;//输出
	if(CUV.GetDataSize()>0)//当数据个数大于零个的时候开始进入计算否则，输出均为0
	{
		//得到均值
		double sum=0;
		double avg=0;
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
			sum+=value;
		}
		avg=sum/CUV.GetDataSize();
		dOutput=avg;
	}
	CUniValue UniVar1;
	CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
	UniVar1.AddData(dOutput);
	pPortObjOut1->SetPortUniValue(UniVar1);
	//输出计算
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CAvgSeriesBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}
//重载以显示不同的属性对象对话框
void CAvgSeriesBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgAvgSeries dlg;
	dlg.DoModal();
}
//重载以将本模块写入到数据库中
void CAvgSeriesBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}
//复制图形时，需要拷贝属性。
void CAvgSeriesBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//将属性放置传输数据中
void CAvgSeriesBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}