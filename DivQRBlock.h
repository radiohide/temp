// DivQRBlock.h: interface for the CDivQRBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_)
#define AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDivQRBlock : public CCalcBlock  
{
public:
	CDivQRBlock();
	virtual ~CDivQRBlock();
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

#endif // !defined(AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_)
