// BTree.h: interface for the CBTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_)
#define AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ElementInOut.h"
class CElementInOut;
class RuleREPoint;
class CBTree  
{
public:
	CBTree(void);
	~CBTree(void);

	int type;//0单值点;1统计点;
	CPtrArray Leaf;
	unsigned short oprt;//运算符
	CString oprtName;	//用于函数名称
	double valueD;		//当前二叉树(表达式)的值
	CElementInOut* RtPt;	//用于测点变量

	RuleREPoint* pRuleREPoint;//用于推荐规则中，计算目标值
};

#endif // !defined(AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_)
