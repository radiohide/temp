#if !defined(AFX_DLGBLOCKNAME_H__479715C8_A2F1_47FA_ADE7_9D88882D7B80__INCLUDED_)
#define AFX_DLGBLOCKNAME_H__479715C8_A2F1_47FA_ADE7_9D88882D7B80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlockName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockName dialog

class CDlgBlockName : public CDialog
{
// Construction
public:
	CDlgBlockName(CWnd* pParent = NULL);   // standard constructor
    CString m_strWndTitle;
// Dialog Data
	//{{AFX_DATA(CDlgBlockName)
	enum { IDD = IDD_DIALOG_BLOCKNAME };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBlockName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLOCKNAME_H__479715C8_A2F1_47FA_ADE7_9D88882D7B80__INCLUDED_)
