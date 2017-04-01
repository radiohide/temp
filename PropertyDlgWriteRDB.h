#if !defined(AFX_PROPERTYDLGWRITERDB_H__9C53DA80_EBE1_4F95_8E2D_127EF59E96B5__INCLUDED_)
#define AFX_PROPERTYDLGWRITERDB_H__9C53DA80_EBE1_4F95_8E2D_127EF59E96B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWriteRDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRDB dialog

class CPropertyDlgWriteRDB : public CDialog
{
// Construction
public:
	CPropertyDlgWriteRDB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWriteRDB)
	enum { IDD = IDD_DIALOG_WRITERDB };
	CComboBox	m_comboBoxDBSel;
	int m_TagTimeOffsetOrInput;//1:采用系统时间+偏移。0:采用外部偏移。
	long m_lOffset;//时间偏移量值
	CString m_strTagName;
	CString m_strTagDesp;
	CString	m_strTableName;
	CString m_strDBDesp;
	int m_iPointType;
	int m_iPointStatus;

	//}}AFX_DATA
	CStringArray despArr;//数据源描述
	CStringArray typeArr;//类型描述

	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgBCBWriteRelv)
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
	//{{AFX_MSG(CPropertyDlgBCBWriteRelv)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonRdbField();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWRITERDB_H__9C53DA80_EBE1_4F95_8E2D_127EF59E96B5__INCLUDED_)
