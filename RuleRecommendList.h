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
	CArray<RuleFB*,RuleFB*&> RuleFBArr;//��ǰ������������ǰ�������

	CArray<ConForRuleRecom*,ConForRuleRecom*&> ConArr;//��ÿ������Ϊ��λ�����ھ�
	CArray<RuleRecomResult*,RuleRecomResult*&> RuleRecomReslutArr;//�������Ҫ�鿴����ʷ����

	CArray<RuleRecomResult*,RuleRecomResult*&> CompRuleReslutArr;//������Ź���ԱȽ���Ĺ����¼
	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> RuleRecomFunArr;//�����Ƽ������б�
private:
	CArray<int,int> ruleFBIDArr;
	CArray<int,int> ruleVarIDArr;
	CArray<int,int> ruleFrontRuleNumArr;
	
	CString histCurrentCon;

	CPtrArray *pListPoteDll;//��ż���Ŀ��ֵ���ⲿ����ָ��

	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;

public:
	void RunRecom();
	void SaveRunRecom();//�����Ƽ�����
	RuleRecommendList();
	virtual ~RuleRecommendList();
	void SetRuleFBAllArr(CArray<RuleFB*,RuleFB*&> *pRuleFBAllArr);
	void SetConDataArr(CArray<ConDataTag*,ConDataTag*&> *pConDataTagHistArr);
	void SetDllPtrArr(CPtrArray *pListPoteDll);
	void InitRuleRecomList();
	void ReInitConArr();//���¼����Ƽ������������¶�ȡ�Ĺ������³�ʼ��
	void GetRuleCount(int &ruleAllCount);
	void CopyData(RuleRecommendList *pSource);//�������ݣ��ڴ�����ǰѡ�����ʷ����ʱʹ��
	void SetCurrSelHistCon(CString strCon);//���õ�ǰѡ�еĹ���
	void BuildSelHistConData();//������ǰѡ�����ʷ��������
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
	bool getIndexInArr(const CArray<int,int> &intArr,int num,int &index);//������intArr�в���ֵΪnum���ڵ�����
};

#endif // !defined(AFX_RULERECOMMENDLIST_H__BAAC0DB6_F58C_48D4_93B4_610877C98630__INCLUDED_)
