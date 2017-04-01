#if !defined(AFX_PROPERTYDLGCOALK_H__3B8BE5F5_4BE0_47E9_A4CC_EBFB59E0553D__INCLUDED_)
#define AFX_PROPERTYDLGCOALK_H__3B8BE5F5_4BE0_47E9_A4CC_EBFB59E0553D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgCoalK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalK dialog

class CPropertyDlgCoalK : public CDialog
{
// Construction
public:
	CPropertyDlgCoalK(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgCoalK)
	enum { IDD = IDD_DIALOG_COALK };
	CComboBox	m_comboBoxDBSel;
	CComboBox	m_ComboBoxStepUnit;
	CListCtrl	m_List;
	CTime	m_TimeE;
	CTime	m_TimeS;
	int		m_iBDay;
	int		m_iFDay;
	CString	m_StrPowerUnit;
	int		m_iStepLength;
	CString	m_strPowerTagName;
	int m_iStepUnit;
	long	m_lOffset;
	int		m_TagTimeOffsetOrInput;
	CString	m_strTableName;
	//}}AFX_DATA

	CStringArray m_strFlowTagNameArr;
	CStringArray m_strFlowUnitArr;
	CStringArray m_strFlowDespArr;
	
	CString m_strDBName;

	CArray<int,int> m_ArrSourceRDBTagCon;
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;

	void SetSETime(int iSHour,int iSMinute,int iSSecond,int iEHour,int iEMinute,int iESecond);
	void SetStepUnit(int iStepUnit);
	void SetFlowTagData(CStringArray &strFlowTagNameArr,
						CStringArray &strFlowUnitArr,
						CStringArray &strFlowDespArr);
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	CStringArray despArr;
	CStringArray typeArr;
private:
	void initListBoxBody();
	void initListBoxHead();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgCoalK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgCoalK)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddFlow();
	afx_msg void OnButtonEditFlow();
	afx_msg void OnButtonDelFlow();
	afx_msg void OnDblclkListFlow(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGCOALK_H__3B8BE5F5_4BE0_47E9_A4CC_EBFB59E0553D__INCLUDED_)
