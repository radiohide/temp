#if !defined(AFX_PROPERTYDLGADD_H__B2147207_402C_4AC6_83B9_429DEF7F8400__INCLUDED_)
#define AFX_PROPERTYDLGADD_H__B2147207_402C_4AC6_83B9_429DEF7F8400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAdd dialog

class CPropertyDlgAdd : public CDialog
{
// Construction
public:
	CPropertyDlgAdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgAdd)
	enum { IDD = IDD_DIALOG_ADD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CString m_strInputEdit;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgAdd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGADD_H__B2147207_402C_4AC6_83B9_429DEF7F8400__INCLUDED_)
