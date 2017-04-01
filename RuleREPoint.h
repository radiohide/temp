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
	CString LongID;//点名，包含子组号和变量名
	CPtrArray *pListPoteDll;//存放函数指针
	RTFUNC rtFunc;//外部函数指针
	TAGINFO* pG;
	TAGINFO* PSourceValue;//针对外部函数要传入一组数据

	CBTree* m_BTree;	//公式表达式的二叉树
	int	PType;			//点类型，0为采集点，1为计算点，2为系数点
	CString PFormula;	//点的计算公式
	double PValue;		//点的结果（包括采集数据、和根据公式计算数据）
	int PState;			//标识是否已经得到正确点值
	time_t PTime;		//点值的时间
private:
	int	m_Position;		//表达式字符分析指针
	bool IsFormula;		//标识公式是否正确
	bool b_Calced;//是否被计算过
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
	RuleREPoint(CString name,CString expr,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap);//构造计算点
	RuleREPoint(CString name,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap);//构造数据点
	void SetDllMap(CPtrArray *ListPoteDll);
	void SetRuleFBPtr(RuleFB* pRuleFB);
	void SetCGMark(CString cGMark);//设置复合组分类结果

	int ParseFormula();//解析公式到二叉树
	void ReadEValue();//读取特征值
	int CalcFormula();//计算点值(计算点)
	double GetValue();
	double ReportValue (CBTree* pnod);
	virtual ~RuleREPoint();

};

#endif // !defined(AFX_RULEREPOINT_H__A1D69B1C_6C2A_4F0D_82FC_85FC57E9D4C3__INCLUDED_)
