#if !defined(AFX_PROPERTYDLGSUMSERIES_H__320B693C_577E_470B_B036_160CB91E9BC6__INCLUDED_)
#define AFX_PROPERTYDLGSUMSERIES_H__320B693C_577E_470B_B036_160CB91E9BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgSumSeries.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSumSeries dialog

class CPropertyDlgSumSeries : public CDialog
{
// Construction
public:
	CPropertyDlgSumSeries(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgSumSeries)
	enum { IDD = IDD_DIALOG_SUMSERIES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgSumSeries)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgSumSeries)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSUMSERIES_H__320B693C_577E_470B_B036_160CB91E9BC6__INCLUDED_)
