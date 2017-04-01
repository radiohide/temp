#if !defined(AFX_PROPERTYDLGCOALDATA_H__79FC105B_5D5D_4620_A85D_2CF16946D525__INCLUDED_)
#define AFX_PROPERTYDLGCOALDATA_H__79FC105B_5D5D_4620_A85D_2CF16946D525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgCoalData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalData dialog
class CPropertyDlgTabCoalOut;
class CPropertyDlgTabCoalKTag;
class CPropertyDlgCoalData : public CDialog
{
// Construction
public:
	CPropertyDlgCoalData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgCoalData)
	enum { IDD = IDD_DIALOG_COALDATA };
	CListCtrl	m_List;
	CComboBox	m_comboBoxDBSel;
	CString	m_strTableName;
	int		m_TagTimeOffsetOrInput;
	long	m_lOffset;
	int	m_iDays;
	int		m_iNearTimeOrValue;
	double	m_dThreshold;
	//}}AFX_DATA
	CPropertyDlgTabCoalKTag *pTabCoalKTag;
	CPropertyDlgTabCoalOut *pTabCoalOut;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgCoalData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CString m_strDBName;
	CArray<int,int> m_ArrSourceRDBTagCon;
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;

	CStringArray m_strCoalTagName;
	CStringArray m_strCoalTagDesp;
	CStringArray m_strCoalTagUnit;

	void SetCoalTagData(CStringArray &m_strCoalTagName,
		CStringArray &m_strCoalTagDesp,
		CStringArray &m_strCoalTagUnit
		);
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

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgCoalData)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTablist1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGCOALDATA_H__79FC105B_5D5D_4620_A85D_2CF16946D525__INCLUDED_)
