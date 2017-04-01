#if !defined(AFX_PROPERTYDLGSTOP_H__41F7F528_3B21_481B_A08C_B53D2DC23E2B__INCLUDED_)
#define AFX_PROPERTYDLGSTOP_H__41F7F528_3B21_481B_A08C_B53D2DC23E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStop dialog

class CPropertyDlgStop : public CDialog
{
// Construction
public:
	CPropertyDlgStop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStop)
	enum { IDD = IDD_DIALOG_STOP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CString m_StrBlockDespText;
	CString m_StrWinText;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStop)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTOP_H__41F7F528_3B21_481B_A08C_B53D2DC23E2B__INCLUDED_)
