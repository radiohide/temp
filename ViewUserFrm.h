#if !defined(AFX_VIEWUSERFRM_H__DEA743EC_61EE_422B_8D29_153C49DA7159__INCLUDED_)
#define AFX_VIEWUSERFRM_H__DEA743EC_61EE_422B_8D29_153C49DA7159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewUserFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewUserFrm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewUserFrm : public CFormView
{
protected:
	CViewUserFrm();
	CViewUserFrm(UINT nIDTemplate);           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewUserFrm)

// Form Data
public:
	//{{AFX_DATA(CViewUserFrm)
	enum { IDD = IDD_DIALOG_CFGSETIME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	virtual void RefreshWindow();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewUserFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewUserFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewUserFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWUSERFRM_H__DEA743EC_61EE_422B_8D29_153C49DA7159__INCLUDED_)
