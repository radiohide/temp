#if !defined(AFX_PROPERTYDLGRDBHISTSERIESBAT_H__17966C51_D6E8_48D6_AA3E_731879EC7596__INCLUDED_)
#define AFX_PROPERTYDLGRDBHISTSERIESBAT_H__17966C51_D6E8_48D6_AA3E_731879EC7596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRDBHistSeriesBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBHistSeriesBat dialog

class CPropertyDlgRDBHistSeriesBat : public CDialog
{
// Construction
public:
	CPropertyDlgRDBHistSeriesBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRDBHistSeriesBat)
	enum { IDD = IDD_PROPERTYDLGRDBHISTSERIESBAT_DIALOG };
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRDBHistSeriesBat)
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
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRDBHistSeriesBat)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnButtonRdbfield();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRDBHISTSERIESBAT_H__17966C51_D6E8_48D6_AA3E_731879EC7596__INCLUDED_)
