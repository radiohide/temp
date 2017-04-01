#if !defined(AFX_PROPERTYDLGSAWTOOTH_H__B48B83C3_9945_43A9_9A1E_E807C5FA59A1__INCLUDED_)
#define AFX_PROPERTYDLGSAWTOOTH_H__B48B83C3_9945_43A9_9A1E_E807C5FA59A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgSawtooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSawtooth dialog

class CPropertyDlgSawtooth : public CDialog
{
// Construction
public:
	CPropertyDlgSawtooth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgSawtooth)
	enum { IDD = IDD_DIALOG_SAWTOOTH };
	int		m_iMin;
	int		m_iMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgSawtooth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgSawtooth)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSAWTOOTH_H__B48B83C3_9945_43A9_9A1E_E807C5FA59A1__INCLUDED_)
