#if !defined(AFX_DLGADDMODULE_H__3DFD174C_79B5_46E4_9CD0_CE53FBA73872__INCLUDED_)
#define AFX_DLGADDMODULE_H__3DFD174C_79B5_46E4_9CD0_CE53FBA73872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddModule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddModule dialog

class CDlgAddModule : public CDialog
{
// Construction
public:
	CDlgAddModule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddModule)
	enum { IDD = IDD_DIALOG_ADDMODULE };
	CString	m_strDesp;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddModule)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDMODULE_H__3DFD174C_79B5_46E4_9CD0_CE53FBA73872__INCLUDED_)
