#if !defined(AFX_PROPERTYDLGSTATENUM_H__046695BA_B0CE_478E_928B_0EE27AE707F4__INCLUDED_)
#define AFX_PROPERTYDLGSTATENUM_H__046695BA_B0CE_478E_928B_0EE27AE707F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatEnum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatEnum dialog

class CPropertyDlgStatEnum : public CDialog
{
// Construction
public:
	CPropertyDlgStatEnum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatEnum)
	enum { IDD = IDD_DIALOG_STAT_ENUM };
	CComboBox	m_ComBoReqType;
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	CString	m_strVarName;
	CString	m_strVarUnit;
	CString	m_strVarDesp;
	long	m_lSnapshotT;//查询周期
	int		m_iReqType;//查询方式:0快照；1原始
	int		m_iStatType;//统计方式：0出现次数、1持续时长、2进入次数
	CString	m_strOutName;
	//}}AFX_DATA
	CString m_strDBName;//关系库名称
	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型

	CArray<double,double> m_dSValueArr;//区间开始
	CArray<double,double> m_dEValueArr;//区间结束
	CArray<int,int> m_iSETag;//区间状态
	CArray<int,int> m_iUpOrDowmLimit;//高限或低限制，0低限；1高限
	int m_iTagTimeIntM;//是否整分钟左右2秒取整
private:
	CStringArray despArr;
	CStringArray typeArr;
public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
	
private:
	void initListBoxHead();
	void initListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatEnum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatEnum)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	virtual void OnOK();
	afx_msg void OnSelchangeComboReqType();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditVarname();
	afx_msg void OnButtonAdv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATENUM_H__046695BA_B0CE_478E_928B_0EE27AE707F4__INCLUDED_)
