#if !defined(AFX_PROPERTYDLGDISCRETETABLE_H__C2A2D2EC_674F_44FF_9978_82F546167456__INCLUDED_)
#define AFX_PROPERTYDLGDISCRETETABLE_H__C2A2D2EC_674F_44FF_9978_82F546167456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgDiscreteTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDiscreteTable dialog

class CPropertyDlgDiscreteTable : public CDialog
{
private:
	CArray<int,int> m_ArrSourceRDBTagCon;//1:连接。0:未连接
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//类型

	CStringArray  m_division_code;
	CStringArray  m_code_desp;
	CStringArray  m_low_value;
	CStringArray  m_high_value;
// Construction
public:
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	CPropertyDlgDiscreteTable(CWnd* pParent = NULL);   // standard constructor

	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);

// Dialog Data
	//{{AFX_DATA(CPropertyDlgDiscreteTable)
	enum { IDD = IDD_DIALOG_DISCRETETABLE };
	CComboBox	m_ComDBSel;
	CString	m_strTableName;
	CString	m_strDBType;
	//}}AFX_DATA

	int iComIndex;
	CString m_strDBDesp;

   	CStringArray despArr;//数据源描述
	CStringArray typeArr;//类型描述
   //用于查询按钮
	CCalcModel* m_pModelPro;
	double m_dFromInput;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgDiscreteTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgDiscreteTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonSearch();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGDISCRETETABLE_H__C2A2D2EC_674F_44FF_9978_82F546167456__INCLUDED_)
