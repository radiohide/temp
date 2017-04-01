#if !defined(AFX_DLGVARSEVALUE_H__CC72ED68_3C8B_4066_9AD9_218B5CB2D720__INCLUDED_)
#define AFX_DLGVARSEVALUE_H__CC72ED68_3C8B_4066_9AD9_218B5CB2D720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVarSEValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVarSEValue dialog

class CDlgVarSEValue : public CDialog
{
// Construction
public:
	CDlgVarSEValue(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVarSEValue)
	enum { IDD = IDD_DIALOG_VAR_SEVALUE };
	double	m_dEValue;
	int		m_iState;
	double	m_dSValue;
	int		m_iUpOrDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVarSEValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVarSEValue)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVARSEVALUE_H__CC72ED68_3C8B_4066_9AD9_218B5CB2D720__INCLUDED_)
