#if !defined(AFX_DLGRULEFBLIST_H__D05C3331_B25C_4AED_8705_E15FE5A6253B__INCLUDED_)
#define AFX_DLGRULEFBLIST_H__D05C3331_B25C_4AED_8705_E15FE5A6253B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRuleFBList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleFBList dialog

class CDlgRuleFBList : public CDialog
{
// Construction
public:
	CDlgRuleFBList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRuleFBList)
	enum { IDD = IDD_DIALOG_RULE_FBLIST };
	CListCtrl	m_List;
	//}}AFX_DATA
	void InitListBoxHead();
	void ReadFBData();
	void InitListBoxBody();

	CStringArray tableNameArr;
	CStringArray tableDespArr;
	CString	m_strRuleDBName;
	CString	m_strRuleDBDesp;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRuleFBList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRuleFBList)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRULEFBLIST_H__D05C3331_B25C_4AED_8705_E15FE5A6253B__INCLUDED_)
