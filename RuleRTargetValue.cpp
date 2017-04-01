// RuleRTargetValue.cpp: implementation of the RuleRTargetValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRTargetValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RuleRTargetValue::RuleRTargetValue()
{
	TargetVarName="TargetVar123456789";
	CGMark="";
	TargetFunExp="";
	TargetValue=0;
}

RuleRTargetValue::~RuleRTargetValue()
{
	clearRREPointMap();
}
void RuleRTargetValue::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void RuleRTargetValue::SetDllPtrArr(CPtrArray *pListPoteDll)
{
	this->pListPoteDll=pListPoteDll;
}
void RuleRTargetValue::SetRuleFBPtr(RuleFB* pRuleFB)
{
	this->pRuleFB=pRuleFB;
}
void RuleRTargetValue::AnalyExp()//������ʽ
{
	initRREPointMap();
	POSITION pos=RuleREPointMap.GetStartPosition();//����map�ڵļ���㣬�������ڵĶ�����
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			if(1==pRuleRePoint->PType)
				pRuleRePoint->ParseFormula();//����Լ����ʽ��Ӧ������
		}
	}
}

void RuleRTargetValue::Reset()//��λ
{
	POSITION pos=RuleREPointMap.GetStartPosition();//����map�ڵļ���㣬�������ڵĶ�����
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			delete pRuleRePoint;
			pRuleRePoint=NULL;
		}
	}
	RuleREPointMap.RemoveAll();
}

void RuleRTargetValue::ReadEValue()//��ȡ����ֵ
{
	POSITION pos=RuleREPointMap.GetStartPosition();//����map�ڵļ���㣬�������ڵĶ�����
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			if(0==pRuleRePoint->PType)
				pRuleRePoint->ReadEValue();//���ݵ��ȡ����ֵ
		}
	}
}

void RuleRTargetValue::Calc()//����Ŀ��ֵ
{
	
	RuleREPoint *pRuleRePoint;
	pRuleRePoint=RuleREPointMap[TargetVarName];
	if(pRuleRePoint!=NULL)
	{
		if(1==pRuleRePoint->PType)
		{
			pRuleRePoint->CalcFormula();//���������ֵ
			TargetValue=pRuleRePoint->PValue;
		}
	}
}

void RuleRTargetValue::initRREPointMap()//��ʼ��������Map
{
	clearRREPointMap();
	CStringArray pointNameArr;//����Ŀ��ֵ��ʽ�еĵ�Ϊ�ɼ��㣬�ɼ��õ������ֵ��
	int indexFindStart=0;
	while(TargetFunExp.Find('[',indexFindStart)>=0)
	{
		int sIndex=TargetFunExp.Find('[',indexFindStart);
		int eIndex=TargetFunExp.Find(']',indexFindStart);
		CString strName=TargetFunExp.Mid(sIndex+1,eIndex-sIndex-1);
		pointNameArr.Add(strName);
		indexFindStart=eIndex+1;
	}

	RuleREPoint *pRuleRePoint;
	for(int i=0;i<pointNameArr.GetSize();i++)//��ʼ����ֵ��
	{
		CString strName=pointNameArr[i];
		pRuleRePoint=new RuleREPoint(strName,&RuleREPointMap);
		pRuleRePoint->SetRuleFBPtr(this->pRuleFB);
		pRuleRePoint->SetCGMark(CGMark);
		pRuleRePoint->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
		RuleREPointMap.SetAt(strName,pRuleRePoint);
	}
	pRuleRePoint=new RuleREPoint(TargetVarName,TargetFunExp+"@",&RuleREPointMap);//��ʼ�������
	pRuleRePoint->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
	pRuleRePoint->SetDllMap(pListPoteDll);
	RuleREPointMap.SetAt(TargetVarName,pRuleRePoint);
}

void RuleRTargetValue::clearRREPointMap()
{
	POSITION pos=RuleREPointMap.GetStartPosition();//����map�ڵļ���㣬�������ڵĶ�����
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			delete pRuleRePoint;
			pRuleRePoint=NULL;
		}
	}
	RuleREPointMap.RemoveAll();
}
