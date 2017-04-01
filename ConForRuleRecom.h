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
	void InitTargetValueArr();//��ʼ��Ŀ��ֵ���У���ȡĿ�긴������ࡣ
	void RunRecom();
	void SaveRunRecom();//�����Ƽ�����
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	CString GetSubGMarkBySGID(int subGID);//����Ȩ�غ���Ȼ�����ҵ����Ź����еĶ�Ӧ���������
public:
	CArray<RuleFB*,RuleFB*&> *pRuleFBArr;//��ǰ������������ǰ�������
	ConDataTag *pConDataTag;//��ǰ��������
	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> *pRuleRecomFunArr;//�����Ƽ������б�
	CArray<RuleRTargetValue*,RuleRTargetValue*&> RuleRTargetValueArr;//�Ƽ���������Ŀ��ֵ
	int ListID;
	CArray<RuleRecomResult*,RuleRecomResult*&> RuleRecomResultArr;//�Ƽ�����Ľ������
private:
	CPtrArray *pListPoteDll;//��ż���Ŀ��ֵ���ⲿ����ָ��

	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
private:
	bool beInRuleRTValueArrByCGMark(CString cGMark);
	void delRuleRemResultArr();
	void readTargetCGMark();//��ȡĿ�긴�������
	void removeTargetValueArr();//���Ŀ��ֵ����
	void runOneByRecomFun(RuleRecomFunTag *pRuleRecomFunTag);
	void orderRRTargetObj();//��Ŀ��ֵ��������
};

#endif // !defined(AFX_CONFORRULERECOM_H__1EE0DB9A_586E_4AB1_9883_DF73423D9CFF__INCLUDED_)
