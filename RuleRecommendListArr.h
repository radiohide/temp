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

	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> RuleRecomFunTagArrForView;//�ڹ���չʾ������ʾ�Ĺ���

	RuleRecommendList *pCurrentRuleRecom;

	CPtrArray ListPoteDll;//��ż���Ŀ��ֵ���ⲿ����ָ��
private:
	int histCurrentListID;//��ǰѡ�еĹ�����ID
	CString histCurrentCon;//��ǰѡ�еĹ���

private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
	int m_iRuleID;

	CString m_strConCode;	

	CArray<int,int> *m_piCGArr;//���ʽ�и�����ID
	CStringArray *m_pStrFunExpArr;//���ʽ
	CStringArray *m_pStrFunDepArr;//���ʽ����
public:
	void SetConCode(CString strConCode);
	void SetRuleListID(int ruleID);
	void ReInitRuleArr();//���³�ʼ��RuleArr���������¶�ȡ��������ȡ�Ƽ���ʽ
	void WriteRuleFunArrForView();
	void RunRecom();
	void SaveRunRecom();//�����Ƽ�����
	RuleRecommendListArr();
	virtual ~RuleRecommendListArr();
	void InitRuleRecommendListArr();//��ʼ��
	void SetCurrHistCon(CString conStr);//���õ�ǰѡ�е���ʷ����
	void SetCurrHistRuleListID(int listID);//���õ�ǰѡ�еĹ�����
	CString GetCurrHistCon();
	int GetCurrHistRuleListID();
	void BuildCurrHistSelRule();//����ѡ����Ϣ������ǰѡ�����ʷ����
	bool LookUpHistConByConStr(CString strCon,ConDataTag *&pConDataTag);
	void DelRuleRecomFunArrForView();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void SetFunExpData(CArray<int,int> *m_piCGArr,CStringArray *m_pStrFunExpArr,CStringArray *m_pStrFunDepArr);
private:
	void delConData();//ɾ������
	void initDllMapping();
	void readConData();
	void readModeRuleList();
	void readModeRuleFB();
	void buildRuleFB();
	void buildRuleList();
};

#endif // !defined(AFX_RULERECOMMENDLISTARR_H__91513FBD_BB35_412A_9957_AED27EC31F93__INCLUDED_)
