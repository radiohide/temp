#if !defined(AFX_PROPERTYDLGSTAT_H__DC361079_2E29_4D4D_8E54_3899C5544000__INCLUDED_)
#define AFX_PROPERTYDLGSTAT_H__DC361079_2E29_4D4D_8E54_3899C5544000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStat dialog

class CPropertyDlgStat : public CDialog
{
// Construction
public:
	CPropertyDlgStat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStat)
	enum { IDD = IDD_DIALOG_STAT };
	CListCtrl	m_List;
	CComboBox	m_comboBoxDBSel;
	int		m_iSETimeType;
	//}}AFX_DATA

	CString m_strTitle;
	CString m_strBlockDesp;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CCalcModel *m_pModel;
	int m_iVarType;
	CString m_strDBName;

	CArray<int,int> m_ArrSourceRDBTagCon;
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;


	CStringArray m_strVarNameArr;//变量名称
	CStringArray m_strVarDespArr;//变量描述
	CStringArray m_strVarUnitArr;//机组类型
	CStringArray m_strUnitSetArr;//机组名称

// Implementation
public:
void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	void initListBoxHead();
	void initListBoxBody();
	void addVarToList(CString strVarName,CString strDesp,CString strUnit,
					  int iUnit,int iSet);
private:
	CStringArray despArr;
	CStringArray typeArr;
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStat)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonVarSel();
	afx_msg void OnButtonDelFlow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTAT_H__DC361079_2E29_4D4D_8E54_3899C5544000__INCLUDED_)
