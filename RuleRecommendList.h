// RuleRecommendList.h: interface for the RuleRecommendList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERECOMMENDLIST_H__BAAC0DB6_F58C_48D4_93B4_610877C98630__INCLUDED_)
#define AFX_RULERECOMMENDLIST_H__BAAC0DB6_F58C_48D4_93B4_610877C98630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConForRuleRecom.h"
#include "RuleRecomResult.h"
#include "RuleRecomFunTag.h"

class RuleRecommendList
{
public:
	int ListID;
	CString RuleName;
	CString RuleDesp;
	CArray<RuleFB*,RuleFB*&> *pRuleFBAllArr;
	CArray<ConDataTag*,ConDataTag*&> *pConDataTagHistArr;
	CArray<RuleFB*,RuleFB*&> RuleFBArr;//当前规则所包含的前后件集合

	CArray<ConForRuleRecom*,ConForRuleRecom*&> ConArr;//以每个工况为单位进行挖掘
	CArray<RuleRecomResult*,RuleRecomResult*&> RuleRecomReslutArr;//用来存放要查看的历史规则

	CArray<RuleRecomResult*,RuleRecomResult*&> CompRuleReslutArr;//用来存放规则对比界面的规则记录
	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> RuleRecomFunArr;//规则推荐方法列表。
private:
	CArray<int,int> ruleFBIDArr;
	CArray<int,int> ruleVarIDArr;
	CArray<int,int> ruleFrontRuleNumArr;
	
	CString histCurrentCon;

	CPtrArray *pListPoteDll;//存放计算目标值的外部函数指针

	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;

public:
	void RunRecom();
	void SaveRunRecom();//保存推荐规则
	RuleRecommendList();
	virtual ~RuleRecommendList();
	void SetRuleFBAllArr(CArray<RuleFB*,RuleFB*&> *pRuleFBAllArr);
	void SetConDataArr(CArray<ConDataTag*,ConDataTag*&> *pConDataTagHistArr);
	void SetDllPtrArr(CPtrArray *pListPoteDll);
	void InitRuleRecomList();
	void ReInitConArr();//重新加载推荐方法，根据新读取的工况重新初始化
	void GetRuleCount(int &ruleAllCount);
	void CopyData(RuleRecommendList *pSource);//拷贝数据，在创建当前选择的历史规则时使用
	void SetCurrSelHistCon(CString strCon);//设置当前选中的工况
	void BuildSelHistConData();//创建当前选择的历史工况数据
	void DelCompRRDataArr();
	void AddCompRRDataArr(RuleRecomResult *pRRResult,int ruleIndex);
	void ReadCompHistRREValueArr();
	void SetFunExpData(CArray<int,int> *m_piCGArr,CStringArray *m_pStrFunExpArr,CStringArray *m_pStrFunDepArr);
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	CString GetSubGMarkBySGID(int subGID);
private:
	void delRunRecomFunArr();
	void delConArr();
	void readRuleRecomFun();
	void readRuleListVar();
	void buildSelfRuleList();
	void buildConArr();
	bool getIndexInArr(const CArray<int,int> &intArr,int num,int &index);//查找在intArr中查找值为num所在的索引
};

#endif // !defined(AFX_RULERECOMMENDLIST_H__BAAC0DB6_F58C_48D4_93B4_610877C98630__INCLUDED_)
