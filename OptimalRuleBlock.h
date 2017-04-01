// OptimalRuleBlock.h: interface for the COptimalRuleBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_)
#define AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
class RuleRecommendListArr;
class COptimalRuleBlock : public CCalcBlock  
{
public:
	COptimalRuleBlock();
	virtual ~COptimalRuleBlock();
public:
//-----------------------------------------------------------------------------------------------------------
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
    //用于根据参数项和值进行属性的设置，当读取值要用
	virtual void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="");
	//重载以显示不同的属性对象对话框
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
	//重载以将本模块写入到数据库中
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord);
	//复制图形时，需要拷贝属性。
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//将属性放置传输数据中
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	BOOL getEValueBySGIDMarkVar(int iSubGID,CString SubGMark,CString varName,double &EValue);
public:
	CString	m_strDBName;
	int m_iUnit;
	int m_iSet;
	int m_iRuleID;	

	CArray<int,int> m_iCGArr;//表达式中复合组ID
	CStringArray m_strFunExpArr;//表达式
	CStringArray m_strFunDepArr;//表达式描述

	CArray<int,int> m_iVarCGArr;//变量中复合租ID
	CStringArray m_strVarNameArr;//变量名称
	CStringArray m_strVarDespArr;//变量描述
	CStringArray m_StrVarUnit;//变量单位
private:
	RuleRecommendListArr *m_RuleRecommendListArr;//真正用来寻找最优规则
};

#endif // !defined(AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_)
