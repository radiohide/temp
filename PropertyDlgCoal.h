#if !defined(AFX_PROPERTYDLGCOAL_H__1C68C603_72E6_4DDE_8841_10DCD9B79E25__INCLUDED_)
#define AFX_PROPERTYDLGCOAL_H__1C68C603_72E6_4DDE_8841_10DCD9B79E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgCoal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoal dialog

class CPropertyDlgCoal : public CDialog
{
// Construction
public:
	CPropertyDlgCoal(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgCoal)
	enum { IDD = IDD_DIALOG_COAL };
	double	m_dMf;
	BOOL	m_bFixMf;
	int		m_iH;
	int		m_iLwCoalAr;
	int		m_iMs;
	int		m_iS;
	int		m_iAshAr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgCoal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgCoal)
	virtual void OnOK();
	afx_msg void OnCheckFixmf();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGCOAL_H__1C68C603_72E6_4DDE_8841_10DCD9B79E25__INCLUDED_)
