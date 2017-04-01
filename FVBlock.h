#if !defined(AFX_FVBLOCK_H__5843C27A_6FA5_4AB1_B0A7_C54AFB7144BA__INCLUDED_)
#define AFX_FVBLOCK_H__5843C27A_6FA5_4AB1_B0A7_C54AFB7144BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FVBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFVBlock form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BlockGridCtrl.h"
class CFVBlock : public CFormView
{
protected:
	CFVBlock();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFVBlock)

// Form Data
public:
	//{{AFX_DATA(CFVBlock)
	enum { IDD = IDD_FORMVIEW_BLOCK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void UpdateBlock();
	CString m_strBlockName;
	CString m_strModuleName;
protected:
	CBlockGridCtrl   m_GridCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFVBlock)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual ~CFVBlock();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFVBlock)
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FVBLOCK_H__5843C27A_6FA5_4AB1_B0A7_C54AFB7144BA__INCLUDED_)
