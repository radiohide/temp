#if !defined(AFX_PROPERTYDLGPEXTSTM_H__94368E26_CBDD_4F9B_AFC7_6C161F04C114__INCLUDED_)
#define AFX_PROPERTYDLGPEXTSTM_H__94368E26_CBDD_4F9B_AFC7_6C161F04C114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgPExtStm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPExtStm dialog

class CPropertyDlgPExtStm : public CDialog
{
// Construction
public:
	void SetDlgText(CString str);
	CPropertyDlgPExtStm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgPExtStm)
	enum { IDD = IDD_DIALOG_PEXTSTM };
	BOOL	m_bBeIgnore;
	//}}AFX_DATA
	CString m_StrWindowText;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgPExtStm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgPExtStm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGPEXTSTM_H__94368E26_CBDD_4F9B_AFC7_6C161F04C114__INCLUDED_)
