// NumConverToStrBlock.cpp: implementation of the CNumConverToStrBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "NumConverToStrBlock.h"
#include "PropertyDlgNumToStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumConverToStrBlock::CNumConverToStrBlock()
{
	SetBlockType(_T( "NumConverToStr" ) );
	ConstructBlockInputOutput();
}

CNumConverToStrBlock::~CNumConverToStrBlock()
{

}
//构造初始的输入输出个数
void CNumConverToStrBlock::ConstructBlockInputOutput()
{
	AddInputPort("In",VALUE_DOUBLE, _T("out1"), _T("out1"));	
	AddOutputPort("Out",VALUE_RTVALUE_SERIES,"out1");	
}
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CNumConverToStrBlock::CreateFromString( const CString& strType )
{
	CNumConverToStrBlock* obj = new CNumConverToStrBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//初始化计算的函数做特殊的初始化
void CNumConverToStrBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//计算函数，实现本模块的计算
void CNumConverToStrBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	CCalcPort *pPortObjIn = GetInputPortObj(0);
	double dInValue;
	pPortObjIn->GetPortUniValue().GetDoubleVal(&dInValue);
	int iInValue=(int)dInValue;// 取其整数部分
	CString strIn;
	strIn.Format("%d",iInValue);

	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	for(int i=0;i<strIn.GetLength();i++)
	{
		int iChar=strIn.GetAt(i);
		UniVar.AddData((double)iChar);
	}
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CNumConverToStrBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}

//重载以显示不同的属性对象对话框
void CNumConverToStrBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgNumToStr dlg;
	dlg.DoModal();
}
//重载以将本模块写入到数据库中
void CNumConverToStrBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	
}
//复制图形时，需要拷贝属性。
void CNumConverToStrBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//将属性放置传输数据中
void CNumConverToStrBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}