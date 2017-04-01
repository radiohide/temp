#if !defined(AFX_PROPERTYDLGSLEEP_H__643F0EEF_C043_47B5_9A6F_9092DF8631FF__INCLUDED_)
#define AFX_PROPERTYDLGSLEEP_H__643F0EEF_C043_47B5_9A6F_9092DF8631FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgSleep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSleep dialog

class CPropertyDlgSleep : public CDialog
{
// Construction
public:
	CPropertyDlgSleep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgSleep)
	enum { IDD = IDD_PROPERTYDLGSLEEP_DIALOG };
	int		m_iSleep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgSleep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgSleep)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSLEEP_H__643F0EEF_C043_47B5_9A6F_9092DF8631FF__INCLUDED_)
