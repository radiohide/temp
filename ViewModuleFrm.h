#if !defined(AFX_VIEWMODULEFRM_H__2291F3B2_2F47_49ED_BD4E_9D16470CE6F9__INCLUDED_)
#define AFX_VIEWMODULEFRM_H__2291F3B2_2F47_49ED_BD4E_9D16470CE6F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewModuleFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewModuleFrm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "FlowchartEditor/FlowchartEntityContainer.h"
#include "FlowchartEditor/FlowchartEditor.h"

#include "CalcModel.h"
#include "ViewUserFrm.h"
class CViewModuleFrm : public CViewUserFrm
{
protected:
	CViewModuleFrm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewModuleFrm)
/////关于组态视图///////////////////////////////////////////////
public:
	CFlowchartEntityContainer*	GetData();
	void FindBlockByStrName(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr);
	void FindBlockByStrDesp(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr);
	void FindBlockByStrPro(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr);

	void ReplaceBlockName(CString strOldBName,CString strOldReplace,CString strNewReplace);
	void ReplaceBlockDesp(CString strOldBName,CString strOldReplace,CString strNewReplace);
	void ReplaceBlockPro(CString strOldBName,CString strOldReplace,CString strNewReplace);
protected:
	CFlowchartEntityContainer	m_objs;
	CFlowchartEditor	m_editor;
	int					m_screenResolutionX;
///////////////////////////////////////////////////////////////////
// Attributes
public:
	CString m_strModuleName; //该view显示的模块名称
// Operations
public:
	void SetUnselAllObj();//不选择任何计算块
	void SetSelBlockInVeiwByName(CString strBlockName);//选择指定计算块
	BOOL SaveModuleEntity(_ConnectionPtr &pCon);
	BOOL ReadModuleEntity(_ConnectionPtr &pCon);
	void OnButtonBlock(CString strBlockType,CString strBlockName,CString strBlockDesp);
	virtual void RefreshWindow();
// Form Data
public:
	//{{AFX_DATA(CViewModuleFrm)
	enum { IDD = IDD_FORMVIEW_MODULE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewModuleFrm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewModuleFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void ShowPortValue();
	// Generated message map functions
	//{{AFX_MSG(CViewModuleFrm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnFileSaveModule();
protected:
	afx_msg void OnCfgSetBlockCalID();
	//afx_msg void OnCfgOrdershow();
	//afx_msg void OnUpdateCfgOrdershow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMagPortvalueshow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMODULEFRM_H__2291F3B2_2F47_49ED_BD4E_9D16470CE6F9__INCLUDED_)
