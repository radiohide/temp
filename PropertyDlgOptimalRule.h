#if !defined(AFX_PROPERTYDLGOPTIMALRULE_H__23A9C230_99D5_4818_890D_CD566A140C52__INCLUDED_)
#define AFX_PROPERTYDLGOPTIMALRULE_H__23A9C230_99D5_4818_890D_CD566A140C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgOptimalRule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgOptimalRule dialog
class CPropertyDlgTabOptRuleFun;
class CPropertyDlgTabRuleVar;
class CCalcModel;
class CPropertyDlgOptimalRule : public CDialog
{
// Construction
public:
	CPropertyDlgOptimalRule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgOptimalRule)
	enum { IDD = IDD_DIALOG_OPTIMALRULE };
	CComboBox	m_comboBoxRuleList;
	CComboBox	m_comboBoxUnitSel;
	CComboBox	m_comboBoxDBSel;
	CTabCtrl	m_TabList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgOptimalRule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
public:
	CString	m_strDBName;
	int m_iUnit;
	int m_iSet;
	int m_iRuleID; 

	CArray<int,int> m_iCGArr;//���ʽ�и�����ID
	CStringArray m_strFunExpArr;//���ʽ
	CStringArray m_strFunDepArr;//���ʽ����

	CArray<int,int> m_iVarCGArr;//�����и�����ID
	CStringArray m_strVarNameArr;//��������
	CStringArray m_strVarDespArr;//��������
	CStringArray m_StrVarUnit;//������λ

public:
	void SetPtrModel(CCalcModel *p);
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	void initTabCtrl();//��ʼ��Tab�ؼ���ʱ�򣬴���ÿҳ����Ҫ������
	BOOL readUnitList();//��ȡ������Ϣ
	void comboBoxUnitSelAddStr();
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);//
	BOOL readRuleList();
	void comboBoxRuleListAddStr();
private:
	CCalcModel* m_pModel;
	CPropertyDlgTabOptRuleFun *pDlgFun;
	CPropertyDlgTabRuleVar *pDlgVar;

	CArray<int,int> m_ArrSourceRDBTagCon;//1:���ӡ�0:δ����
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//����

	CStringArray nameArr;//����Դ����
	CStringArray typeArr;//��������

	CStringArray unitNameArr;//��������
	CArray<int,int> unitArr;//��Ԫ��
	CArray<int,int> setsArr;//�����

	CArray<int,int> ruleIDArr;//����ID
	CStringArray ruleNameArr;//��������
	CStringArray ruleDespArr;//��������
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgOptimalRule)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboDbSel();
	afx_msg void OnSelchangeComboUnitSel();
	afx_msg void OnSelchangeComboRulelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGOPTIMALRULE_H__23A9C230_99D5_4818_890D_CD566A140C52__INCLUDED_)
