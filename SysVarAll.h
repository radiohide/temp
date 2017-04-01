// SysVarAll.h: interface for the SysVarAll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVARALL_H__F0634AEB_28AF_4BCB_B847_72495F1F7785__INCLUDED_)
#define AFX_SYSVARALL_H__F0634AEB_28AF_4BCB_B847_72495F1F7785__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SysVarCG.h"
#include "SysVarG.h"
#include "SysVar.h"
class CCalcModel;
class SysVarAll  
{
public:
	CArray<SysVarCG*,SysVarCG*&> SysVarCGArr;//ȫ���ĸ�����
	CArray<SysVarG*,SysVarG*&> SysVarGArr;//ȫ��������
	CArray<SysVar*,SysVar*&> SysVarArr;//ȫ���ı���
	SysVar* pCurrentSelSysVar;
private:
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;
public:
	SysVarAll();
	virtual ~SysVarAll();
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void InitSysVarAll();
	void GetSelObj(CString strNodeText,int &type,SysVarCG *&pRSysVarCG,SysVarG *&pRSysVarG);
	SysVar* GetSysVarByVarName(CString strVarName);
private:
	void readSysVarCG();
	void readSysVarG();
	void readSysVar();

	void delSysVarCGArr();
	void delSysVarGArr();
	void delSysVarArr();

};

#endif // !defined(AFX_SYSVARALL_H__F0634AEB_28AF_4BCB_B847_72495F1F7785__INCLUDED_)
