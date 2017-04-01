#if !defined(AFX_DLGPORTDATA_H__6D050882_1B46_43BE_8919_F0A0F294452C__INCLUDED_)
#define AFX_DLGPORTDATA_H__6D050882_1B46_43BE_8919_F0A0F294452C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPortData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPortData dialog

class CDlgPortData : public CDialog
{
// Construction
public:
	int GetType();
	void SetType(int type);
	CDlgPortData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPortData)
	enum { IDD = IDD_DIALOG_PORTDATA };
	CString	m_StrType;
	CString	m_StrName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPortData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPortData)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPORTDATA_H__6D050882_1B46_43BE_8919_F0A0F294452C__INCLUDED_)
