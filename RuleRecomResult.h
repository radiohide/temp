// RuleRecomResult.h: interface for the RuleRecomResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERECOMRESULT_H__66644F44_BA4B_4398_8858_CFE6BB745732__INCLUDED_)
#define AFX_RULERECOMRESULT_H__66644F44_BA4B_4398_8858_CFE6BB745732__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConDataTag.h"
class RuleFB;
class SubGroupInfTag;
class RuleRecomResult  
{
public:
	int ListID;
	RuleFB *pRuleFB;
	ConDataTag *pConDataTag;
	CStringArray *pBTargetMarkArr;//设置以该分类为后件，将根据此分类来找前件，合成规则
	int RuleSelIndex;

	CStringArray FMarkArr;
	CStringArray BMarkArr;
	CArray<int,int> WeightArr;
	CArray<int,int> IDArr;//t_uXsY_f2b_rule中的ID

	CString FMark;
	CString BMark;
	int Weight;

private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
	//CArray<RuleBGParaEValue*,RuleBGParaEValue*&> FEValueArr;//对应FMarkView的特征数据，用于对比显示特征值
	//CArray<RuleBGParaEValue*,RuleBGParaEValue*&> BEValueArr;//对应BMarkView的特征数据，用于对比显示特征值
public:
	void SaveToRunRecom();
	void ReadRuleByBTargetMarkArr();
	RuleRecomResult();
	virtual ~RuleRecomResult();
	void SetConDataTag(ConDataTag *pConDataTag);
	void SetRuleFB(RuleFB *pRuleFB);
	void ReadRecomRule();//读取历史规则
	void SetListID(int listID);//设置规则链ID
	void CopyRuleByRIndex(RuleRecomResult *pRRResult,int ruleIndex);
	void ReadHistEValueArr();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void SetBTargetMarkArr(CStringArray *pBTargetMarkArr);
	CString GetSubGMarkBySGID(int comGID,int subGID);//根据权重和自然排序找到最优规则中的对应的子组分类
private:
	void writeToTheRuleRecom(int ruleID,int ruleListID);
	bool beHaveTheRule(int ruleID,int ruleListID);
	void delRuleRecomBeAdvice();
	void buildEValueArr();
	void delEValueArr();
	CString getSubGMarkByCGMark(int subGID,CString cGmark,int cGID);
};

#endif // !defined(AFX_RULERECOMRESULT_H__66644F44_BA4B_4398_8858_CFE6BB745732__INCLUDED_)
