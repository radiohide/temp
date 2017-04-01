// SysVarCG.h: interface for the SysVarCG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVARCG_H__94094FC6_CB90_4883_9568_AC36F4E624E4__INCLUDED_)
#define AFX_SYSVARCG_H__94094FC6_CB90_4883_9568_AC36F4E624E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SysVarG.h"
class CCalcModel;
class SysVarCG  
{
public:
	CString ID;
	CString Name;
	CString Desp;
	CArray<SysVarG*,SysVarG*&> *pSysVarGArr;//全部的子租
	CArray<SysVarG*,SysVarG*&> SysVarSelfGArr;//自身的子租
private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
public:
	SysVarCG();
	virtual ~SysVarCG();
	void InitSysVarCG();
	void SetGArr(CArray<SysVarG*,SysVarG*&> *pSysVarGArr);
	CString GetNodeText();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
private:
	void readCGVar(CStringArray &gIDArr);
	SysVarG* getSysVarGByStrGID(CString strGID);
};

#endif // !defined(AFX_SYSVARCG_H__94094FC6_CB90_4883_9568_AC36F4E624E4__INCLUDED_)
