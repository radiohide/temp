#if !defined(AFX_PROPERTYDLGRDBHISTTAGBAT_H__FC7B4A15_2D00_4446_9ADA_3B0791B7921C__INCLUDED_)
#define AFX_PROPERTYDLGRDBHISTTAGBAT_H__FC7B4A15_2D00_4446_9ADA_3B0791B7921C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRDBHistTagBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBHistTagBat dialog

class CPropertyDlgRDBHistTagBat : public CDialog
{
// Construction
public:
	CPropertyDlgRDBHistTagBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRDBHistTagBat)
	enum { IDD = IDD_PROPERTYDLGRDBHISTTAGBAT_DIALOG };
	CListCtrl	m_List;
	CComboBox	m_comboBoxDBSel;
	int		m_iNearSec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRDBHistTagBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:

	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;

	CString m_strDBName;//���ݿ�����

	CStringArray strTableArr;//����
	CStringArray strTagArr;//������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ
	public:

	CArray<int,int> m_ArrSourceRDBTagCon;//���ӱ�־
	CStringArray m_ArrSourceRDBName;//��������
	CStringArray m_ArrSourceRDBType;//��������
public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	void initListBoxBody();
	void initListBoxHead();
private:
	CStringArray despArr;
	CStringArray typeArr;
// Implementation
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRDBHistTagBat)
	afx_msg void OnButtonRdbfield();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangeComboDbsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRDBHISTTAGBAT_H__FC7B4A15_2D00_4446_9ADA_3B0791B7921C__INCLUDED_)
