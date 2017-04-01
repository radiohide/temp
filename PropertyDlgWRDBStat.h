#if !defined(AFX_PROPERTYDLGWRDBSTAT_H__F659A70B_FB71_439D_B9F6_AB7138C4DA6C__INCLUDED_)
#define AFX_PROPERTYDLGWRDBSTAT_H__F659A70B_FB71_439D_B9F6_AB7138C4DA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWRDBStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWRDBStat dialog

class CPropertyDlgWRDBStat : public CDialog
{
// Construction
public:
	CPropertyDlgWRDBStat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWRDBStat)
	enum { IDD = IDD_PROPERTYDLGWRDBSTAT_DIALOG };
	CComboBox	m_comboBoxDBSel;
	BOOL	m_BMaxValue;
	BOOL	m_BMinValue;
	BOOL	m_BStat;
	BOOL	m_BCount;
	CString	m_strVarDesp;
	CString	m_strVarName;
	//}}AFX_DATA

	CString m_strDBName;//关系库名称
	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgWRDBStat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
		void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
			CStringArray &m_ArrSourceRDBName,
			CStringArray &m_ArrSourceRDBType
			);
// Implementation
private:
	CStringArray despArr;
	CStringArray typeArr;
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgWRDBStat)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWRDBSTAT_H__F659A70B_FB71_439D_B9F6_AB7138C4DA6C__INCLUDED_)
