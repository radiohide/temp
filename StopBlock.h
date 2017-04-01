// StopBlock.h: interface for the CStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_)
#define AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStopBlock : public CCalcBlock  
{
public:
	CStopBlock();
	virtual ~CStopBlock();
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

#endif // !defined(AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_)
