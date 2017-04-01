#if !defined(AFX_DLGRULEOPTFUN_H__5F7B0864_DD00_4CD7_992A_DA7497214B6F__INCLUDED_)
#define AFX_DLGRULEOPTFUN_H__5F7B0864_DD00_4CD7_992A_DA7497214B6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRuleOptFun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleOptFun dialog

class CDlgRuleOptFun : public CDialog
{
// Construction
public:
	CDlgRuleOptFun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRuleOptFun)
	enum { IDD = IDD_DIALOG_RULEOPTFUN };
	int		m_iCGID;
	CString	m_strDesp;
	CString	m_strFormula;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRuleOptFun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRuleOptFun)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRULEOPTFUN_H__5F7B0864_DD00_4CD7_992A_DA7497214B6F__INCLUDED_)
