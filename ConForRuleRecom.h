// ConForRuleRecom.h: interface for the ConForRuleRecom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFORRULERECOM_H__1EE0DB9A_586E_4AB1_9883_DF73423D9CFF__INCLUDED_)
#define AFX_CONFORRULERECOM_H__1EE0DB9A_586E_4AB1_9883_DF73423D9CFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RuleRecomFunTag.h"
#include "RuleRTargetValue.h"
#include "RuleRecomResult.h"

class ConForRuleRecom  
{
public:
	ConForRuleRecom();
	virtual ~ConForRuleRecom();
	void SetConDataTag(ConDataTag *pConDataTag);
	void SetListID(int listID);
	void SetRuleFBArr(CArray<RuleFB*,RuleFB*&> *pRuleFBArr);
	void SetRuleRecomFunArr(CArray<RuleRecomFunTag*,RuleRecomFunTag*&> *pRuleRecomFunArr);
	void SetDllPtrArr(CPtrArray *pListPoteDll);

	void BuildRule();
	void InitTargetValueArr();//初始化目标值序列，读取目标复合组分类。
	void RunRecom();
	void SaveRunRecom();//保存推荐规则
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	CString GetSubGMarkBySGID(int subGID);//根据权重和自然排序找到最优规则中的对应的子组分类
public:
	CArray<RuleFB*,RuleFB*&> *pRuleFBArr;//当前规则所包含的前后件集合
	ConDataTag *pConDataTag;//当前工况数据
	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> *pRuleRecomFunArr;//规则推荐方法列表。
	CArray<RuleRTargetValue*,RuleRTargetValue*&> RuleRTargetValueArr;//推荐规则链条目标值
	int ListID;
	CArray<RuleRecomResult*,RuleRecomResult*&> RuleRecomResultArr;//推荐规则的结果集合
private:
	CPtrArray *pListPoteDll;//存放计算目标值的外部函数指针

	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
private:
	bool beInRuleRTValueArrByCGMark(CString cGMark);
	void delRuleRemResultArr();
	void readTargetCGMark();//读取目标复合组分类
	void removeTargetValueArr();//清空目标值序列
	void runOneByRecomFun(RuleRecomFunTag *pRuleRecomFunTag);
	void orderRRTargetObj();//对目标值进行排序
};

#endif // !defined(AFX_CONFORRULERECOM_H__1EE0DB9A_586E_4AB1_9883_DF73423D9CFF__INCLUDED_)
