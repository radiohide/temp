#if !defined(AFX_DLGPROGRESS_H__BCA81C58_875A_4F5D_9595_C45D70F3B5FF__INCLUDED_)
#define AFX_DLGPROGRESS_H__BCA81C58_875A_4F5D_9595_C45D70F3B5FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor
	void SetRange(int  Begin, int End);
	BOOL m_bCancel;
// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DLGPROGRESS };
	CProgressCtrl	m_progTotal;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual void OnCancel();
	afx_msg void OnProgress(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__BCA81C58_875A_4F5D_9595_C45D70F3B5FF__INCLUDED_)
