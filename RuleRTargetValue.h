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
	CString TargetVarName;//该变量表示计算目标值时的目标变量的点名
	CString CGMark;//复合组分类
	CString TargetFunExp;//目标值公式
	double TargetValue;//目标值

	CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> RuleREPointMap;//存特征值数据
	CPtrArray *pListPoteDll;
	RuleFB* pRuleFB;//目标复合组所在的前后件
public:
	void SetDllPtrArr(CPtrArray *pListPoteDll);
	void SetRuleFBPtr(RuleFB* pRuleFB);
	void Calc();//计算目标值
	void ReadEValue();//读取特征值
	void Reset();//复位
	void AnalyExp();//解析公式
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
	void initRREPointMap();//初始化特征点Map
};

#endif // !defined(AFX_RULERTARGETVALUE_H__AB223D63_07D5_4007_90E6_167A36A3DC80__INCLUDED_)
