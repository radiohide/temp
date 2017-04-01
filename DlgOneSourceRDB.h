#if !defined(AFX_DLGONESOURCERDB_H__436405DD_1B14_4C21_922F_F4964EE48084__INCLUDED_)
#define AFX_DLGONESOURCERDB_H__436405DD_1B14_4C21_922F_F4964EE48084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOneSourceRDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOneSourceRDB dialog

class CDlgOneSourceRDB : public CDialog
{
// Construction
public:
	CDlgOneSourceRDB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOneSourceRDB)
	enum { IDD = IDD_DIALOG_ONE_RDBSOURCE };
	CComboBox	m_ComBo;
	CString	m_strFileName;
	CString	m_strIP;
	CString	m_strRDBName;
	CString	m_strRDBPass;
	CString	m_strSourceName;
	CString	m_strUserName;
	//}}AFX_DATA
	CString m_strRDBType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOneSourceRDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOneSourceRDB)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGONESOURCERDB_H__436405DD_1B14_4C21_922F_F4964EE48084__INCLUDED_)
