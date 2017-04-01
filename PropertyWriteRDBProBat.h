#if !defined(AFX_PROPERTYWRITERDBPROBAT_H__73F53AD5_AF08_4736_B118_8F6204C2CC44__INCLUDED_)
#define AFX_PROPERTYWRITERDBPROBAT_H__73F53AD5_AF08_4736_B118_8F6204C2CC44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyWriteRDBProBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyWriteRDBProBat dialog

class CPropertyWriteRDBProBat : public CDialog
{
// Construction
public:
	CPropertyWriteRDBProBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyWriteRDBProBat)
	enum { IDD = IDD_PROPERTYWRITERDBPROBAT_DIALOG };
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	int		m_iSaveSeconds;
	BOOL	m_BOnlySave;
	//采用采集时间还是外部输入时间。
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyWriteRDBProBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
public:
	
	CString m_strDBName;//数据库名称
	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型

	CStringArray tagNameArr;//点名
	CStringArray unitArr;//单位
	CStringArray dispArr;//描述
	CStringArray strOutPutNameArr;//写入关系库的表名称（不包含年后缀）
private:
	CStringArray despArr;
	CStringArray typeArr;
	
private:
	void initListBoxHead();
	void initListBoxBody();
public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyWriteRDBProBat)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonRdbfield();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckOnlySave();
	afx_msg void OnSelchangeComboDbsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYWRITERDBPROBAT_H__73F53AD5_AF08_4736_B118_8F6204C2CC44__INCLUDED_)
