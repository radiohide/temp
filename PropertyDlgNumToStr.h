#if !defined(AFX_PROPERTYDLGNUMTOSTR_H__1FE8F6F5_106A_440F_B63F_DDF678C9AF09__INCLUDED_)
#define AFX_PROPERTYDLGNUMTOSTR_H__1FE8F6F5_106A_440F_B63F_DDF678C9AF09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgNumToStr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgNumToStr dialog

class CPropertyDlgNumToStr : public CDialog
{
// Construction
public:
	CPropertyDlgNumToStr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgNumToStr)
	enum { IDD = IDD_DIALOG_NUM_TO_STR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgNumToStr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgNumToStr)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGNUMTOSTR_H__1FE8F6F5_106A_440F_B63F_DDF678C9AF09__INCLUDED_)
