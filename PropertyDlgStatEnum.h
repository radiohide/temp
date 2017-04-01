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
	long	m_lSnapshotT;//��ѯ����
	int		m_iReqType;//��ѯ��ʽ:0���գ�1ԭʼ
	int		m_iStatType;//ͳ�Ʒ�ʽ��0���ִ�����1����ʱ����2�������
	CString	m_strOutName;
	//}}AFX_DATA
	CString m_strDBName;//��ϵ������
	CArray<int,int> m_ArrSourceRDBTagCon;//���ӱ�־
	CStringArray m_ArrSourceRDBName;//��������
	CStringArray m_ArrSourceRDBType;//��������

	CArray<double,double> m_dSValueArr;//���俪ʼ
	CArray<double,double> m_dEValueArr;//�������
	CArray<int,int> m_iSETag;//����״̬
	CArray<int,int> m_iUpOrDowmLimit;//���޻�����ƣ�0���ޣ�1����
	int m_iTagTimeIntM;//�Ƿ�����������2��ȡ��
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
