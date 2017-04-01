// RuleREPoint.h: interface for the RuleREPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULEREPOINT_H__A1D69B1C_6C2A_4F0D_82FC_85FC57E9D4C3__INCLUDED_)
#define AFX_RULEREPOINT_H__A1D69B1C_6C2A_4F0D_82FC_85FC57E9D4C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RuleFB.h"

class RuleREPoint  
{
public:
	RuleFB* pRuleFB;
	CString CGMark;
	CString LongID;//��������������źͱ�����
	CPtrArray *pListPoteDll;//��ź���ָ��
	RTFUNC rtFunc;//�ⲿ����ָ��
	TAGINFO* pG;
	TAGINFO* PSourceValue;//����ⲿ����Ҫ����һ������

	CBTree* m_BTree;	//��ʽ���ʽ�Ķ�����
	int	PType;			//�����ͣ�0Ϊ�ɼ��㣬1Ϊ����㣬2Ϊϵ����
	CString PFormula;	//��ļ��㹫ʽ
	double PValue;		//��Ľ���������ɼ����ݡ��͸��ݹ�ʽ�������ݣ�
	int PState;			//��ʶ�Ƿ��Ѿ��õ���ȷ��ֵ
	time_t PTime;		//��ֵ��ʱ��
private:
	int	m_Position;		//���ʽ�ַ�����ָ��
	bool IsFormula;		//��ʶ��ʽ�Ƿ���ȷ
	bool b_Calced;//�Ƿ񱻼����
	CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap;

private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
private:
	bool IsDigit(char byte);
	CString Substring(CString str,int pos1,int count);
	bool IsLetter(char byte);
	
	void SkipSpaces();
	CBTree* pmOP();
	CBTree* mdOP();
	CBTree* involOP();
	CBTree* logicOP();
	CBTree* logsecOP();
	CBTree* factorOP();
	CBTree* identiOP();
	
public:
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	RuleREPoint(CString name,CString expr,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap);//��������
	RuleREPoint(CString name,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap);//�������ݵ�
	void SetDllMap(CPtrArray *ListPoteDll);
	void SetRuleFBPtr(RuleFB* pRuleFB);
	void SetCGMark(CString cGMark);//���ø����������

	int ParseFormula();//������ʽ��������
	void ReadEValue();//��ȡ����ֵ
	int CalcFormula();//�����ֵ(�����)
	double GetValue();
	double ReportValue (CBTree* pnod);
	virtual ~RuleREPoint();

};

#endif // !defined(AFX_RULEREPOINT_H__A1D69B1C_6C2A_4F0D_82FC_85FC57E9D4C3__INCLUDED_)
