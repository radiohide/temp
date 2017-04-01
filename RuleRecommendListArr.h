// RuleRecommendListArr.h: interface for the RuleRecommendListArr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERECOMMENDLISTARR_H__91513FBD_BB35_412A_9957_AED27EC31F93__INCLUDED_)
#define AFX_RULERECOMMENDLISTARR_H__91513FBD_BB35_412A_9957_AED27EC31F93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RuleRecommendList.h"
#include "PoteDll.h"
class RuleRecommendListArr  
{
public:
	CArray<ConDataTag*,ConDataTag*&> ConDataTagHistArr;
	CArray<RuleFB*,RuleFB*&> RuleFBAllArr;
	CArray<RuleRecommendList*,RuleRecommendList*&> RuleArr;

	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> RuleRecomFunTagArrForView;//在规则展示界面显示的规则。

	RuleRecommendList *pCurrentRuleRecom;

	CPtrArray ListPoteDll;//存放计算目标值的外部函数指针
private:
	int histCurrentListID;//当前选中的规则链ID
	CString histCurrentCon;//当前选中的工况

private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
	int m_iRuleID;

	CString m_strConCode;	

	CArray<int,int> *m_piCGArr;//表达式中复合组ID
	CStringArray *m_pStrFunExpArr;//表达式
	CStringArray *m_pStrFunDepArr;//表达式描述
public:
	void SetConCode(CString strConCode);
	void SetRuleListID(int ruleID);
	void ReInitRuleArr();//重新初始化RuleArr，其中重新读取工况，读取推荐公式
	void WriteRuleFunArrForView();
	void RunRecom();
	void SaveRunRecom();//保存推荐规则
	RuleRecommendListArr();
	virtual ~RuleRecommendListArr();
	void InitRuleRecommendListArr();//初始化
	void SetCurrHistCon(CString conStr);//设置当前选中的历史工况
	void SetCurrHistRuleListID(int listID);//设置当前选中的规则链
	CString GetCurrHistCon();
	int GetCurrHistRuleListID();
	void BuildCurrHistSelRule();//根据选择信息创建当前选择的历史规则
	bool LookUpHistConByConStr(CString strCon,ConDataTag *&pConDataTag);
	void DelRuleRecomFunArrForView();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void SetFunExpData(CArray<int,int> *m_piCGArr,CStringArray *m_pStrFunExpArr,CStringArray *m_pStrFunDepArr);
private:
	void delConData();//删除工况
	void initDllMapping();
	void readConData();
	void readModeRuleList();
	void readModeRuleFB();
	void buildRuleFB();
	void buildRuleList();
};

#endif // !defined(AFX_RULERECOMMENDLISTARR_H__91513FBD_BB35_412A_9957_AED27EC31F93__INCLUDED_)
