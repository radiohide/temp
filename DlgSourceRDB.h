#if !defined(AFX_DLGSOURCERDB_H__04681797_4BB3_4788_991E_3331D13A6620__INCLUDED_)
#define AFX_DLGSOURCERDB_H__04681797_4BB3_4788_991E_3331D13A6620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSourceRDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSourceRDB dialog

class CDlgSourceRDB : public CDialog
{
// Construction
public:
	CDlgSourceRDB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSourceRDB)
	enum { IDD = IDD_DIALOG_SOURCE_RDB };
	CListCtrl	m_ListCtrl;
	CString m_StrModelName;
	//}}AFX_DATA
	int m_iSelIndex;
	BOOL m_bBeEdit;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSourceRDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	void SaveSourceToDB();
	void ReConRDBSource();
	void InitListBoxHead(int tagLocalOrRemote);
	void InitListBoxBody(int tagLocalOrRemote);
	// Generated message map functions
	//{{AFX_MSG(CDlgSourceRDB)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApp();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonEdit();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOURCERDB_H__04681797_4BB3_4788_991E_3331D13A6620__INCLUDED_)
