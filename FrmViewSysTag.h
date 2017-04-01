#if !defined(AFX_FRMVIEWSYSTAG_H__DE077039_E628_42B4_AC66_44025A9F3551__INCLUDED_)
#define AFX_FRMVIEWSYSTAG_H__DE077039_E628_42B4_AC66_44025A9F3551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrmViewSysTag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrmViewSysTag form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFrmViewSysTag : public CFormView
{
protected:
	CFrmViewSysTag();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFrmViewSysTag)

// Form Data
public:
	//{{AFX_DATA(CFrmViewSysTag)
	enum { IDD = IDD_FORMVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void Show();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrmViewSysTag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrmViewSysTag();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFrmViewSysTag)
	afx_msg void OnButtonConfirm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMVIEWSYSTAG_H__DE077039_E628_42B4_AC66_44025A9F3551__INCLUDED_)
