// SysVarG.h: interface for the SysVarG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVARG_H__88D7EB57_F33A_45A0_8D18_ACBD2CF00C33__INCLUDED_)
#define AFX_SYSVARG_H__88D7EB57_F33A_45A0_8D18_ACBD2CF00C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SysVar.h"
class CCalcModel;
class SysVarG  
{
public:
	CString ID;
	CString Name;
	CString Desp;
	CString Type;
	CArray<SysVar*,SysVar*&> *pSysVarArr;//全部的变量
	CArray<SysVar*,SysVar*&> SysSelfVarArr;//自己的变量
private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
public:
	SysVarG();
	void SetVarArr(CArray<SysVar*,SysVar*&> *pSysVarArr);
	virtual ~SysVarG();
	void InitSysVarG();
	void readGVar(CStringArray &strNameArr);
	SysVar* getSysVarByStrName(CString strName);
	CString GetNodeText();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
};

#endif // !defined(AFX_SYSVARG_H__88D7EB57_F33A_45A0_8D18_ACBD2CF00C33__INCLUDED_)
