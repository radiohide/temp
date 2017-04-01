// H8CalcBlock.h: interface for the CH8CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_)
#define AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CH8CalcBlock : public CCalcBlock  
{
public:
	CH8CalcBlock();
	virtual ~CH8CalcBlock();

	virtual void ConstructBlockInputOutput(); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
};

#endif // !defined(AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_)
