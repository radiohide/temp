#if !defined(AFX_PROPERTYDLGSTRMAPTONUM_H__70727E7F_FC69_4DB8_A58A_129A8CBCD3B0__INCLUDED_)
#define AFX_PROPERTYDLGSTRMAPTONUM_H__70727E7F_FC69_4DB8_A58A_129A8CBCD3B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStrMapToNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMapToNum dialog

class CPropertyDlgStrMapToNum : public CDialog
{
// Construction
public:
	CPropertyDlgStrMapToNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStrMapToNum)
	enum { IDD = IDD_DIALOG_STRMAPTONUM };
	CComboBox	m_comboBoxDBSel;
	CString	m_strTableName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStrMapToNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CStringArray despArr;//数据源描述
	CStringArray typeArr;//类型描述
	CCalcModel *m_pModel;

	CStringArray strValueArr;
	CArray<double,double> dValueArr;
	CStringArray strDespArr;
public:
//	CString	m_strTableName;
	CString m_strDBDesp;
	CString m_strDBType;
// Implementation
private:
	CArray<int,int> m_ArrSourceRDBTagCon;//1:连接。0:未连接
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//类型
public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
	void SetModelPtr(CCalcModel *pModel);
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStrMapToNum)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonQuery();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTRMAPTONUM_H__70727E7F_FC69_4DB8_A58A_129A8CBCD3B0__INCLUDED_)
