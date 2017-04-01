#if !defined(AFX_PROPERTYDLGRDBSTATTAGBAT_H__AAB5B1D4_F899_4A1C_BCC1_B0A9E213FBE3__INCLUDED_)
#define AFX_PROPERTYDLGRDBSTATTAGBAT_H__AAB5B1D4_F899_4A1C_BCC1_B0A9E213FBE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRDBStatTagBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBStatTagBat dialog

class CPropertyDlgRDBStatTagBat : public CDialog
{
// Construction
public:
	CPropertyDlgRDBStatTagBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRDBStatTagBat)
	enum { IDD = IDD_PROPERTYDLGRDBSTATTAGBAT_DIALOG };
	CListCtrl	m_List;
	CComboBox	m_comboBoxDBSel;
	int		m_iNearSec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRDBStatTagBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:

	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;

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
	//{{AFX_MSG(CPropertyDlgRDBStatTagBat)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRdbfield();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboDbsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRDBSTATTAGBAT_H__AAB5B1D4_F899_4A1C_BCC1_B0A9E213FBE3__INCLUDED_)
