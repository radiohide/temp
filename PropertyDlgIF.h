#if !defined(AFX_PROPERTYDLGIF_H__F3EE3594_0A78_43D5_917B_FCF79F837B09__INCLUDED_)
#define AFX_PROPERTYDLGIF_H__F3EE3594_0A78_43D5_917B_FCF79F837B09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgIF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIF dialog

class CPropertyDlgIF : public CDialog
{
// Construction
public:
	CPropertyDlgIF(CWnd* pParent = NULL);   // standard constructor
	int GetType();
	void SetType(int type);	
// Dialog Data
	//{{AFX_DATA(CPropertyDlgIF)
	enum { IDD = IDD_DIALOG_IF };
		// NOTE: the ClassWizard will add data members here
	CString	m_StrType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgIF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgIF)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGIF_H__F3EE3594_0A78_43D5_917B_FCF79F837B09__INCLUDED_)
