#if !defined(AFX_PROPERTYDLGRDBSTATSERIESBAT_H__DA6DBF22_DACD_44CF_A254_E9E8126885AB__INCLUDED_)
#define AFX_PROPERTYDLGRDBSTATSERIESBAT_H__DA6DBF22_DACD_44CF_A254_E9E8126885AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRDBStatSeriesBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBStatSeriesBat dialog

class CPropertyDlgRDBStatSeriesBat : public CDialog
{
// Construction
public:
	CPropertyDlgRDBStatSeriesBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRDBStatSeriesBat)
	enum { IDD = IDD_PROPERTYDLGRDBSTATSERIESBAT_DIALOG };
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRDBStatSeriesBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	CString m_strDBName;//数据库名称

	CStringArray strTableArr;//表名
	CStringArray strTagArr;//变量名
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	public:

	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型

public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	void initListBoxBody();
	void initListBoxHead();
private:
	CStringArray despArr;
	CStringArray typeArr;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRDBStatSeriesBat)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRDBSTATSERIESBAT_H__DA6DBF22_DACD_44CF_A254_E9E8126885AB__INCLUDED_)
