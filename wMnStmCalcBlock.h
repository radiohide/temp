// wMnStmCalcBlock.h: interface for the CwMnStmCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_)
#define AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CycleBlock.h"

class CwMnStmCalcBlock : public CCycleBlock  
{
public:
	CwMnStmCalcBlock();
	virtual ~CwMnStmCalcBlock();
public:
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//进行循环计算
	virtual void CalcIter();
private:
	CStringArray m_VarArr;//迭代前变量
	CStringArray m_FormulaArr;
	CArray<int,int> m_TypeArr;//0,数值；1,序列
private:
	void buildInsideEle();

};

#endif // !defined(AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_)
