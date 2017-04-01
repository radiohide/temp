// ModuleStopBlock.cpp: implementation of the CModuleStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ModuleStopBlock.h"
#include "PropertyDlgStop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModuleStopBlock::CModuleStopBlock()
{
	SetBlockType(_T( "ModuleStop" ) );//设置类型
	ConstructBlockInputOutput();
}

CModuleStopBlock::~CModuleStopBlock()
{
	
}
void CModuleStopBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in"), _T("in"));
}
//根据类型新建对象的函数
CCalcBlock* CModuleStopBlock::CreateFromString( const CString& strType )
{
	CModuleStopBlock* obj = new CModuleStopBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CModuleStopBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CModuleStopBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObj = GetInputPortObj(0);
	double dVar;
	pPortObj->GetPortUniValue().GetDoubleVal(&dVar);
	if(dVar!=0)
	{
		this->m_pModule->StopCalc();//模块停止
	}
}
//重载以显示不同的属性对象对话框
void CModuleStopBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStop dlg;
	dlg.m_StrBlockDespText="当输入为非零值时该模块运行停止。";
	dlg.m_StrWinText="模块停止块";
	dlg.DoModal();
}