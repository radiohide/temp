#if !defined(AFX_PROPERTYDLGAVGSERIES_H__3260F13A_C8D7_476A_9664_B0E2F8534EA8__INCLUDED_)
#define AFX_PROPERTYDLGAVGSERIES_H__3260F13A_C8D7_476A_9664_B0E2F8534EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgAvgSeries.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAvgSeries dialog

class CPropertyDlgAvgSeries : public CDialog
{
// Construction
public:
	CPropertyDlgAvgSeries(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgAvgSeries)
	enum { IDD = IDD_DIALOG_AVGSERIES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgAvgSeries)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgAvgSeries)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGAVGSERIES_H__3260F13A_C8D7_476A_9664_B0E2F8534EA8__INCLUDED_)
