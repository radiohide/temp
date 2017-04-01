#if !defined(AFX_DLGVARDATABASE_H__9C128BBD_4327_4C52_80C3_593606FCB7C1__INCLUDED_)
#define AFX_DLGVARDATABASE_H__9C128BBD_4327_4C52_80C3_593606FCB7C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVarDataBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataBase dialog

class CDlgVarDataBase : public CDialog
{
// Construction
public:
	CDlgVarDataBase(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVarDataBase)
	enum { IDD = IDD_DIALOG_VARDATA_BASE };
	CString	m_strDesp;
	CString	m_strVarName;
	CString	m_strUnit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVarDataBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVarDataBase)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVARDATABASE_H__9C128BBD_4327_4C52_80C3_593606FCB7C1__INCLUDED_)
