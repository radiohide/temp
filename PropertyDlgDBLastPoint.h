#if !defined(AFX_PROPERTYDLGDBLASTPOINT_H__57C7CD9E_1C4B_4E6A_98AF_BAE88B749AFD__INCLUDED_)
#define AFX_PROPERTYDLGDBLASTPOINT_H__57C7CD9E_1C4B_4E6A_98AF_BAE88B749AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgDBLastPoint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBLastPoint dialog

class CPropertyDlgDBLastPoint : public CDialog
{
// Construction
public:
	CPropertyDlgDBLastPoint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgDBLastPoint)
	enum { IDD = IDD_DIALOG_DBLASTPOINT };
	CComboBox	m_comboBoxDBSel;
	long	m_lOffset;
	CString	m_strTagName;
	CString	m_strTagDesp;
	int		m_TagTimeOffsetOrInput;
	int		m_iForwardOrBack;
	CString	m_strTableName;
	CString	m_strDBType;
	CString m_strDBDesp;
	CString	m_strSelDBDesp;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CStringArray despArr;//数据源描述
	CStringArray typeArr;//类型描述
	
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgDBLastPoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

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
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgDBLastPoint)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonRdbField();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGDBLASTPOINT_H__57C7CD9E_1C4B_4E6A_98AF_BAE88B749AFD__INCLUDED_)
