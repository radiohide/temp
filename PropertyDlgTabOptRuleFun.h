#if !defined(AFX_PROPERTYDLGTABOPTRULEFUN_H__011B89AF_E2E9_4490_A49A_6A0E5B3E14F3__INCLUDED_)
#define AFX_PROPERTYDLGTABOPTRULEFUN_H__011B89AF_E2E9_4490_A49A_6A0E5B3E14F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTabOptRuleFun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabOptRuleFun dialog

class CPropertyDlgTabOptRuleFun : public CDialog
{
// Construction
public:
	CPropertyDlgTabOptRuleFun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTabOptRuleFun)
	enum { IDD = IDD_DIALOG_TAB_OPTRULEFUN };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTabOptRuleFun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRuleData(CArray<int,int> *m_piCGArr,
			CStringArray *m_pStrFunExpArr,
			CStringArray *m_pStrFunDepArr);
	void FillListBox();
private:
	void initPortListBoxHead();
private:
	CArray<int,int> *m_piCGArr;//符合组ID
	CStringArray *m_pStrFunExpArr;//表达式
	CStringArray *m_pStrFunDepArr;//表达式描述
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTabOptRuleFun)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTABOPTRULEFUN_H__011B89AF_E2E9_4490_A49A_6A0E5B3E14F3__INCLUDED_)
