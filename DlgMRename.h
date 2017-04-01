#if !defined(AFX_DLGMRENAME_H__CA5A1509_BB2E_48BA_84B9_9E6722FA75FF__INCLUDED_)
#define AFX_DLGMRENAME_H__CA5A1509_BB2E_48BA_84B9_9E6722FA75FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMRename dialog

class CDlgMRename : public CDialog
{
// Construction
public:
	CDlgMRename(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMRename)
	enum { IDD = IDD_DIALOG_MRENAME };
	CString	m_strDesp;
	CString	m_strName;
	//}}AFX_DATA
	CString m_strWinText;
	CString m_strOldName;
	CString m_strOldDesp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMRename)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMRENAME_H__CA5A1509_BB2E_48BA_84B9_9E6722FA75FF__INCLUDED_)
