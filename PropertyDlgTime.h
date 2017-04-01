#if !defined(AFX_PROPERTYDLGTIME_H__AED20A42_C63D_411A_B75C_EA08C75337D5__INCLUDED_)
#define AFX_PROPERTYDLGTIME_H__AED20A42_C63D_411A_B75C_EA08C75337D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTime dialog

class CPropertyDlgTime : public CDialog
{
// Construction
public:
	CPropertyDlgTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTime)
	enum { IDD = IDD_DIALOG_TIME };
	CTime	m_Date;
	CTime	m_Time;
	int		m_iTheTimeOrCurrent;
	CString m_strDateTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTime)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTIME_H__AED20A42_C63D_411A_B75C_EA08C75337D5__INCLUDED_)
