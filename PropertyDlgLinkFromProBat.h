#if !defined(AFX_PROPERTYDLGLINKFROMPROBAT_H__31EFC534_980C_473F_95C5_A35A63684997__INCLUDED_)
#define AFX_PROPERTYDLGLINKFROMPROBAT_H__31EFC534_980C_473F_95C5_A35A63684997__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLinkFromProBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFromProBat dialog

class CPropertyDlgLinkFromProBat : public CDialog
{
// Construction
public:
	CPropertyDlgLinkFromProBat(CWnd* pParent = NULL);   // standard constructor

	void initListBoxBody();
	void initListBoxHead();

// Dialog Data
	//{{AFX_DATA(CPropertyDlgLinkFromProBat)
	enum { IDD = IDD_DIALOG_LINKFROMPROBAT };
	CListCtrl	m_List;
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
    int          m_nWindowsType;//
	CString	m_showText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgLinkFromProBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLinkFromProBat)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGLINKFROMPROBAT_H__31EFC534_980C_473F_95C5_A35A63684997__INCLUDED_)
