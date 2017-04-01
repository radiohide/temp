#if !defined(AFX_DLGTABFINDBLOCK_H__262FC850_43BC_46FA_A4C3_18C62DEC2FB1__INCLUDED_)
#define AFX_DLGTABFINDBLOCK_H__262FC850_43BC_46FA_A4C3_18C62DEC2FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTabFindBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTabFindBlock dialog

class CDlgTabFindBlock : public CDialog
{
// Construction
public:
	CDlgTabFindBlock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTabFindBlock)
	enum { IDD = IDD_DIG_TAB_FINDBLOCK };
	BOOL	m_bAllSame;
	CString	m_strRange;
	CString	m_strType;
	CString	m_strFind;
	CString	m_strReplace;
	//}}AFX_DATA
	BOOL m_bTagEdit;//�Ƿ񱻱༭
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTabFindBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSomeCtrlObjHide();
	void SetSomeCtrlObjShow();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTabFindBlock)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFind();
	afx_msg void OnChangeEditReplace();
	afx_msg void OnCheckAllsame();
	afx_msg void OnSelchangeComboRange();
	afx_msg void OnSelchangeComboType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTABFINDBLOCK_H__262FC850_43BC_46FA_A4C3_18C62DEC2FB1__INCLUDED_)
