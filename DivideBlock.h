// DivideBlock.h: interface for the CDivideBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_)
#define AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDivideBlock : public CCalcBlock  
{
public:
	CDivideBlock();
	virtual ~CDivideBlock();
//添加必要的虚函数：
//-----------------------------------------------------------------------------------------------------------
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
};

#endif // !defined(AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_)
