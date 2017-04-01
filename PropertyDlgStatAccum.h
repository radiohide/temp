#if !defined(AFX_PROPERTYDLGSTATACCUM_H__93B71B1C_7D58_4D41_B2F7_118DAEE0D599__INCLUDED_)
#define AFX_PROPERTYDLGSTATACCUM_H__93B71B1C_7D58_4D41_B2F7_118DAEE0D599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatAccum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatAccum dialog

class CPropertyDlgStatAccum : public CDialog
{
// Construction
public:
	CPropertyDlgStatAccum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatAccum)
	enum { IDD = IDD_DIALOG_STAT_ACCUM };
	CComboBox	m_ComBoConFun2;
	CButton	m_BtnCheckCon;
	CComboBox	m_comboStatType;
	CComboBox	m_comboBoxDBSel;
	CListCtrl	m_List;
	int		m_comboBoxStatType;//ͳ������,0����ͳ�ƣ�1��λ��ֵͳ��
	double	m_dGain;//�Ŵ���
	BOOL	m_iConOn;
	int		m_iConReqType;
	CString	m_strConDesp;
	CString	m_strConUnit;
	CString	m_strConVar;
	double	m_dCon1Value;
	double	m_dCon2Value;
	int		m_iConSnapShotT;
	int		m_iCon1Fun;
	int		m_iCon2Fun;
	//}}AFX_DATA
	CString m_strDBName;//��ϵ������
	CArray<int,int> m_ArrSourceRDBTagCon;//���ӱ�־
	CStringArray m_ArrSourceRDBName;//��������
	CStringArray m_ArrSourceRDBType;//��������

	CStringArray tagNameArr;//����
	CStringArray unitArr;//��λ
	CStringArray dispArr;//����
	CArray<int,int> reqTypeArr;//��ѯ��ʽ
	CArray<int,int> snapshotTArr;//��������
	CStringArray strOutPutNameArr;//д���ϵ��ı����ƣ����������׺��
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
	void initConVarExpr();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatAccum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatAccum)
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButtonAdv();
	afx_msg void OnCheckCon();
	afx_msg void OnSelchangeComboConFun2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATACCUM_H__93B71B1C_7D58_4D41_B2F7_118DAEE0D599__INCLUDED_)
