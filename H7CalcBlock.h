// H7CalcBlock.h: interface for the CH7CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_)
#define AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CH7CalcBlock : public CCalcBlock  
{
public:
	CH7CalcBlock();
	virtual ~CH7CalcBlock();

	virtual void ConstructBlockInputOutput(); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
};

#endif // !defined(AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_)
