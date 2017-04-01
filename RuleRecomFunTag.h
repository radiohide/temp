// RuleRecomFunTag.h: interface for the RuleRecomFunTag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERECOMFUNTAG_H__BA2F3928_BDF0_4841_AFA4_5620C7AC63F3__INCLUDED_)
#define AFX_RULERECOMFUNTAG_H__BA2F3928_BDF0_4841_AFA4_5620C7AC63F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RuleRecomFunTag  
{
public:
	int CGroup_ID;
	CString Expression;
	CString Desp;
	int ListID;

	CString ConstraintsExp;//公式中的约束部分
	CArray<double,double> ParaItemArr;//约束公式中的参数
	CString TargetFunExp;//计算目标值的公式
	
public:
	void CopySelf(RuleRecomFunTag *OriRuleFun);
	void AnalyExp();
	RuleRecomFunTag();
	virtual ~RuleRecomFunTag();

};

#endif // !defined(AFX_RULERECOMFUNTAG_H__BA2F3928_BDF0_4841_AFA4_5620C7AC63F3__INCLUDED_)
