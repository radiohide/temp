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

	CArray<int,int> m_iCGArr;//表达式中复合组ID
	CStringArray m_strFunExpArr;//表达式
	CStringArray m_strFunDepArr;//表达式描述

	CArray<int,int> m_iVarCGArr;//变量中复合租ID
	CStringArray m_strVarNameArr;//变量名称
	CStringArray m_strVarDespArr;//变量描述
	CStringArray m_StrVarUnit;//变量单位

public:
	void SetPtrModel(CCalcModel *p);
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
private:
	void initTabCtrl();//初始化Tab控件的时候，创建每页所需要的数据
	BOOL readUnitList();//读取机组信息
	void comboBoxUnitSelAddStr();
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);//
	BOOL readRuleList();
	void comboBoxRuleListAddStr();
private:
	CCalcModel* m_pModel;
	CPropertyDlgTabOptRuleFun *pDlgFun;
	CPropertyDlgTabRuleVar *pDlgVar;

	CArray<int,int> m_ArrSourceRDBTagCon;//1:连接。0:未连接
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//类型

	CStringArray nameArr;//数据源描述
	CStringArray typeArr;//类型描述

	CStringArray unitNameArr;//机组名称
	CArray<int,int> unitArr;//单元号
	CArray<int,int> setsArr;//机组号

	CArray<int,int> ruleIDArr;//规则ID
	CStringArray ruleNameArr;//规则名称
	CStringArray ruleDespArr;//规则描述
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
