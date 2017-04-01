// RuleFB.h: interface for the RuleFB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULEFB_H__2FB64B7C_F88A_451D_91FA_7E285E6CFAB6__INCLUDED_)
#define AFX_RULEFB_H__2FB64B7C_F88A_451D_91FA_7E285E6CFAB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SubGroupInfTag.h"

class RuleFB  
{
public:
	RuleFB();
	virtual ~RuleFB();
	CString GetFSubGsDesp();
	CString GetBSubGsDesp();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
public:
	void InitCGAndSGData();
	int RuleID;//这个前后件规则的ID
	int FID;
	int BID;
	CString FBDesp;
	
	CString FCGName;
	CString BCGName;
	CString FCGDesp;
	CString BCGDesp;

	CArray<SubGroupInfTag*,SubGroupInfTag*&> FSGInfTagArr;
	CArray<SubGroupInfTag*,SubGroupInfTag*&> BSGInfTagArr;
private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
private:
	void readSGInfByCGID(int CGID,int FOrBTag);//FOrBTag,1为F，2为B
	void readCGInf();
};

#endif // !defined(AFX_RULEFB_H__2FB64B7C_F88A_451D_91FA_7E285E6CFAB6__INCLUDED_)
