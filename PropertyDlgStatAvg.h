#if !defined(AFX_PROPERTYDLGSTATAVG_H__1A9DCC46_A8F8_4794_95A4_59DAC104E98F__INCLUDED_)
#define AFX_PROPERTYDLGSTATAVG_H__1A9DCC46_A8F8_4794_95A4_59DAC104E98F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatAvg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatAvg dialog

class CPropertyDlgStatAvg : public CDialog
{
// Construction
public:
	CPropertyDlgStatAvg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatAvg)
	enum { IDD = IDD_DIALOG_STAT_AVG };
	CButton	m_BtnCheckCon;
	CComboBox	m_ComBoConFun2;
	CComboBox	m_ComBoConReqType;
	CListCtrl	m_List;
	CComboBox	m_comboBoxDBSel;
	BOOL	m_iConOn;
	int		m_iCon1Fun;
	int		m_iCon2Fun;
	int		m_iConReqType;
	CString	m_strConDesp;
	CString	m_strConUnit;
	CString	m_strConVar;
	double	m_dCon1Value;
	double	m_dCon2Value;
	int		m_iConSnapShotT;
	//}}AFX_DATA

	CString m_strDBName;//关系库名称
	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型

	CStringArray tagNameArr;//点名
	CStringArray unitArr;//单位
	CStringArray dispArr;//描述
	CArray<int,int> reqTypeArr;//查询方式 //0,快照,1原始
	CArray<int,int> snapshotTArr;//快照周期
	CStringArray strOutPutNameArr;//写入关系库的表名称（不包含年后缀）
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
	void initConVarExpr();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatAvg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatAvg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonAdv();
	afx_msg void OnSelchangeComboConReqType();
	afx_msg void OnSelchangeComboConFun2();
	afx_msg void OnCheckCon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATAVG_H__1A9DCC46_A8F8_4794_95A4_59DAC104E98F__INCLUDED_)
