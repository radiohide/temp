#if !defined(AFX_PROPERTYDLGDBPOINTADV_H__74D3F225_0440_4538_91FF_202FA5357020__INCLUDED_)
#define AFX_PROPERTYDLGDBPOINTADV_H__74D3F225_0440_4538_91FF_202FA5357020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgDBPointAdv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBPointAdv dialog

class CPropertyDlgDBPointAdv : public CDialog
{
// Construction
public:
	CPropertyDlgDBPointAdv(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgDBPointAdv)
	enum { IDD = IDD_DIALOG_DBPOINTADV };
	CString	m_strTagDesp;
	CString	m_strTag1;
	CString	m_strTag2;
	CString	m_strTag3;
	CString	m_strTag4;
	CString	m_strTag5;
    int		m_iOutMethod;	
	BOOL	m_bChkLimit;
	double	m_dHighLimit;
	double	m_dLowLimit;
	double	m_dConstWhenBad;
	BOOL	m_bForceEnable;
	double	m_dForceValue;
	long	m_lOffset;
	int		m_iInOrOut;

	CString	m_strDBType;
	CString m_strDBDesp;
	CComboBox	m_comboBoxDBSel;
	CString m_strTableName;
	int m_iForwardOrBack;
	//}}AFX_DATA
	CStringArray despArr;//数据源描述
	CStringArray typeArr;//类型描述

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgBCBReadReal)
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
	//{{AFX_MSG(CPropertyDlgBCBReadReal)
	afx_msg void OnRadio1();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio2();
	afx_msg void OnSelchangeComboDbsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGDBPOINTADV_H__74D3F225_0440_4538_91FF_202FA5357020__INCLUDED_)
