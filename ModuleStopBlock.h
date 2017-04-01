// ModuleStopBlock.h: interface for the CModuleStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_)
#define AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CModuleStopBlock : public CCalcBlock  
{
public:
	CModuleStopBlock();
	virtual ~CModuleStopBlock();
public:
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
	//重载以显示不同的属性对象对话框
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );

};

#endif // !defined(AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_)
