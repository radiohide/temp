#if !defined(AFX_PROPERTYWRITERDBSTATBAT_H__5BDC46F5_7CFE_4362_8DA9_2F76093653CC__INCLUDED_)
#define AFX_PROPERTYWRITERDBSTATBAT_H__5BDC46F5_7CFE_4362_8DA9_2F76093653CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyWriteRDBStatBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyWriteRDBStatBat dialog

class CPropertyWriteRDBStatBat : public CDialog
{
// Construction
public:
	CPropertyWriteRDBStatBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyWriteRDBStatBat)
	enum { IDD = IDD_PROPERTYWRITERDBSTATBAT_DIALOG };
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	//}}AFX_DATA
public:
	CString m_strDBName;//���ݿ�����
	CArray<int,int> m_ArrSourceRDBTagCon;//���ӱ�־
	CStringArray m_ArrSourceRDBName;//��������
	CStringArray m_ArrSourceRDBType;//��������

	CStringArray tagNameArr;//����
	CStringArray unitArr;//��λ
	CStringArray dispArr;//����
	CStringArray strOutPutNameArr;//д���ϵ��ı����ƣ����������׺��
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

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyWriteRDBStatBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyWriteRDBStatBat)
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYWRITERDBSTATBAT_H__5BDC46F5_7CFE_4362_8DA9_2F76093653CC__INCLUDED_)
