#if !defined(AFX_PROPERTYDLGINTEGRATE_H__9A8B8AE5_6DCE_4AC4_BC16_5302A0B18B13__INCLUDED_)
#define AFX_PROPERTYDLGINTEGRATE_H__9A8B8AE5_6DCE_4AC4_BC16_5302A0B18B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgIntegrate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIntegrate dialog

class CPropertyDlgIntegrate : public CDialog
{
// Construction
public:
	CPropertyDlgIntegrate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgIntegrate)
	enum { IDD = IDD_DIALOG_INTEGRATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgIntegrate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgIntegrate)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGINTEGRATE_H__9A8B8AE5_6DCE_4AC4_BC16_5302A0B18B13__INCLUDED_)
