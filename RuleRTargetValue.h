// RuleRTargetValue.h: interface for the RuleRTargetValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERTARGETVALUE_H__AB223D63_07D5_4007_90E6_167A36A3DC80__INCLUDED_)
#define AFX_RULERTARGETVALUE_H__AB223D63_07D5_4007_90E6_167A36A3DC80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RuleREPoint.h"

class RuleRTargetValue  
{
public:
	CString TargetVarName;//�ñ�����ʾ����Ŀ��ֵʱ��Ŀ������ĵ���
	CString CGMark;//���������
	CString TargetFunExp;//Ŀ��ֵ��ʽ
	double TargetValue;//Ŀ��ֵ

	CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> RuleREPointMap;//������ֵ����
	CPtrArray *pListPoteDll;
	RuleFB* pRuleFB;//Ŀ�긴�������ڵ�ǰ���
public:
	void SetDllPtrArr(CPtrArray *pListPoteDll);
	void SetRuleFBPtr(RuleFB* pRuleFB);
	void Calc();//����Ŀ��ֵ
	void ReadEValue();//��ȡ����ֵ
	void Reset();//��λ
	void AnalyExp();//������ʽ
	RuleRTargetValue();
	virtual ~RuleRTargetValue();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
private:
	void clearRREPointMap();
	void initRREPointMap();//��ʼ��������Map
};

#endif // !defined(AFX_RULERTARGETVALUE_H__AB223D63_07D5_4007_90E6_167A36A3DC80__INCLUDED_)
