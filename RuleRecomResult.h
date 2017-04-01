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
	CStringArray *pBTargetMarkArr;//�����Ը÷���Ϊ����������ݴ˷�������ǰ�����ϳɹ���
	int RuleSelIndex;

	CStringArray FMarkArr;
	CStringArray BMarkArr;
	CArray<int,int> WeightArr;
	CArray<int,int> IDArr;//t_uXsY_f2b_rule�е�ID

	CString FMark;
	CString BMark;
	int Weight;

private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
	//CArray<RuleBGParaEValue*,RuleBGParaEValue*&> FEValueArr;//��ӦFMarkView���������ݣ����ڶԱ���ʾ����ֵ
	//CArray<RuleBGParaEValue*,RuleBGParaEValue*&> BEValueArr;//��ӦBMarkView���������ݣ����ڶԱ���ʾ����ֵ
public:
	void SaveToRunRecom();
	void ReadRuleByBTargetMarkArr();
	RuleRecomResult();
	virtual ~RuleRecomResult();
	void SetConDataTag(ConDataTag *pConDataTag);
	void SetRuleFB(RuleFB *pRuleFB);
	void ReadRecomRule();//��ȡ��ʷ����
	void SetListID(int listID);//���ù�����ID
	void CopyRuleByRIndex(RuleRecomResult *pRRResult,int ruleIndex);
	void ReadHistEValueArr();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void SetBTargetMarkArr(CStringArray *pBTargetMarkArr);
	CString GetSubGMarkBySGID(int comGID,int subGID);//����Ȩ�غ���Ȼ�����ҵ����Ź����еĶ�Ӧ���������
private:
	void writeToTheRuleRecom(int ruleID,int ruleListID);
	bool beHaveTheRule(int ruleID,int ruleListID);
	void delRuleRecomBeAdvice();
	void buildEValueArr();
	void delEValueArr();
	CString getSubGMarkByCGMark(int subGID,CString cGmark,int cGID);
};

#endif // !defined(AFX_RULERECOMRESULT_H__66644F44_BA4B_4398_8858_CFE6BB745732__INCLUDED_)
