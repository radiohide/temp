// StopBlock.cpp: implementation of the CStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StopBlock.h"
#include "PropertyDlgStop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStopBlock::CStopBlock()
{
	SetBlockType(_T( "Stop" ) );//设置类型
	ConstructBlockInputOutput();
}

CStopBlock::~CStopBlock()
{

}
void CStopBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in"), _T("in"));
}
//根据类型新建对象的函数
CCalcBlock* CStopBlock::CreateFromString( const CString& strType )
{
	CStopBlock* obj = new CStopBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CStopBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CStopBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObj = GetInputPortObj(0);
	double dVar;
	pPortObj->GetPortUniValue().GetDoubleVal(&dVar);
	if(dVar!=0)
	{
		this->m_pModule->StopCalc();//模块停止
		this->m_pModel->StopCalc();//模型停止
	}
}
//重载以显示不同的属性对象对话框
void CStopBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStop dlg;
	dlg.DoModal();
}