#if !defined(AFX_PROPERTYDLGTABRULEVAR_H__AE03B517_2A33_4886_82E2_E69EFAF95019__INCLUDED_)
#define AFX_PROPERTYDLGTABRULEVAR_H__AE03B517_2A33_4886_82E2_E69EFAF95019__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTabRuleVar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabRuleVar dialog
class CCalcModel;
class CPropertyDlgTabRuleVar : public CDialog
{
// Construction
public:
	CPropertyDlgTabRuleVar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTabRuleVar)
	enum { IDD = IDD_DIALOG_TAB_RULEVAR };
	CListCtrl	m_List;
	//}}AFX_DATA
private:
	CCalcModel *m_pModel;
	CString *m_pStrDBName;
	int *m_piUnit;
	int *m_piSet;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTabRuleVar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CArray<int,int> *m_piVarCGArr;//变量中复合租ID
	CStringArray *m_pStrVarNameArr;//变量名称
	CStringArray *m_pStrVarDespArr;//变量描述
	CStringArray *m_pStrVarUnit;//变量单位
public:
	void SetRDBInf(CCalcModel *pModel,CString *pStrDBName,int *piUnit,int *piSet);
	void SetVarData(CArray<int,int> *m_piVarCGArr,
		CStringArray *m_pStrVarNameArr,
		CStringArray *m_pStrVarDespArr,
		CStringArray *m_pStrVarUnit
		);
private:
	void initPortListBoxBody();
	void initPortListBoxHead();
	BOOL beExistInSel(CString strVarName);
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTabRuleVar)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonVarSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTABRULEVAR_H__AE03B517_2A33_4886_82E2_E69EFAF95019__INCLUDED_)
