#if !defined(AFX_DLGVARDATAWITHREQ_H__A43226F9_51E8_42FB_9FC8_6795E8A2ED30__INCLUDED_)
#define AFX_DLGVARDATAWITHREQ_H__A43226F9_51E8_42FB_9FC8_6795E8A2ED30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVarDataWithReq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataWithReq dialog

class CDlgVarDataWithReq : public CDialog
{
// Construction
public:
	CDlgVarDataWithReq(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVarDataWithReq)
	enum { IDD = IDD_DIALOG_VARDATA_REQ };
	CComboBox	m_ComBo;
	CString	m_strDesp;
	int		m_iSnapshotT;
	CString	m_strUnit;
	CString	m_strVarName;
	CString	m_strOutName;
	//}}AFX_DATA
	int m_iSnapType;

	bool m_bEnableReqType;//查询方式是否可用
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVarDataWithReq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVarDataWithReq)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboReqType();
	afx_msg void OnChangeEditVarName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVARDATAWITHREQ_H__A43226F9_51E8_42FB_9FC8_6795E8A2ED30__INCLUDED_)
