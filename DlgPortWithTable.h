#if !defined(AFX_DLGPORTWITHTABLE_H__AB2EA921_04BD_4A55_AA3F_5F3DF0270C6E__INCLUDED_)
#define AFX_DLGPORTWITHTABLE_H__AB2EA921_04BD_4A55_AA3F_5F3DF0270C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPortWithTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPortWithTable dialog

class CDlgPortWithTable : public CDialog
{
// Construction
public:
	CDlgPortWithTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPortWithTable)
	enum { IDD = IDD_DLGPORTWITHTABLE_DIALOG };
	CString	m_strDesp;
	CString	m_strUnit;
	CString	m_strTableName;
	CString	m_strVarName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPortWithTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPortWithTable)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPORTWITHTABLE_H__AB2EA921_04BD_4A55_AA3F_5F3DF0270C6E__INCLUDED_)
