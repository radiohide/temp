// PLossRateBlock.h: interface for the CPLossRateBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_)
#define AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CPLossRateBlock : public CCalcBlock  
{
public:
	CPLossRateBlock();
	virtual ~CPLossRateBlock();
public:
//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
};

#endif // !defined(AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_)
