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
void RuleRTargetValue::AnalyExp()//解析公式
{
	initRREPointMap();
	POSITION pos=RuleREPointMap.GetStartPosition();//解析map内的计算点，建立点内的二叉树
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			if(1==pRuleRePoint->PType)
				pRuleRePoint->ParseFormula();//解析约束公式对应二叉树
		}
	}
}

void RuleRTargetValue::Reset()//复位
{
	POSITION pos=RuleREPointMap.GetStartPosition();//解析map内的计算点，建立点内的二叉树
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

void RuleRTargetValue::ReadEValue()//读取特征值
{
	POSITION pos=RuleREPointMap.GetStartPosition();//解析map内的计算点，建立点内的二叉树
	while(pos)
	{
		CString key;
		RuleREPoint *pRuleRePoint;
		RuleREPointMap.GetNextAssoc(pos,key,pRuleRePoint);
		if(pRuleRePoint!=NULL)
		{
			if(0==pRuleRePoint->PType)
				pRuleRePoint->ReadEValue();//数据点读取特征值
		}
	}
}

void RuleRTargetValue::Calc()//计算目标值
{
	
	RuleREPoint *pRuleRePoint;
	pRuleRePoint=RuleREPointMap[TargetVarName];
	if(pRuleRePoint!=NULL)
	{
		if(1==pRuleRePoint->PType)
		{
			pRuleRePoint->CalcFormula();//计算计算点的值
			TargetValue=pRuleRePoint->PValue;
		}
	}
}

void RuleRTargetValue::initRREPointMap()//初始化特征点Map
{
	clearRREPointMap();
	CStringArray pointNameArr;//计算目标值公式中的点为采集点，采集该点的特征值。
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
	for(int i=0;i<pointNameArr.GetSize();i++)//初始化数值点
	{
		CString strName=pointNameArr[i];
		pRuleRePoint=new RuleREPoint(strName,&RuleREPointMap);
		pRuleRePoint->SetRuleFBPtr(this->pRuleFB);
		pRuleRePoint->SetCGMark(CGMark);
		pRuleRePoint->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
		RuleREPointMap.SetAt(strName,pRuleRePoint);
	}
	pRuleRePoint=new RuleREPoint(TargetVarName,TargetFunExp+"@",&RuleREPointMap);//初始化计算点
	pRuleRePoint->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
	pRuleRePoint->SetDllMap(pListPoteDll);
	RuleREPointMap.SetAt(TargetVarName,pRuleRePoint);
}

void RuleRTargetValue::clearRREPointMap()
{
	POSITION pos=RuleREPointMap.GetStartPosition();//解析map内的计算点，建立点内的二叉树
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
