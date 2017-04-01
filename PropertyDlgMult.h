#if !defined(AFX_PROPERTYDLGMULT_H__3788AABE_62D3_4827_91D5_1BEB63A1E302__INCLUDED_)
#define AFX_PROPERTYDLGMULT_H__3788AABE_62D3_4827_91D5_1BEB63A1E302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgMult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMult dialog

class CPropertyDlgMult : public CDialog
{
// Construction
public:
	CPropertyDlgMult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgMult)
	enum { IDD = IDD_DIALOG_MULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CString m_strInputEdit;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgMult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgMult)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGMULT_H__3788AABE_62D3_4827_91D5_1BEB63A1E302__INCLUDED_)
