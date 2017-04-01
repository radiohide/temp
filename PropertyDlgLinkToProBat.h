#if !defined(AFX_PROPERTYDLGLINKTOPROBAT_H__F94E3BC5_3CEA_40A6_ADCD_36EF8FC3BEC3__INCLUDED_)
#define AFX_PROPERTYDLGLINKTOPROBAT_H__F94E3BC5_3CEA_40A6_ADCD_36EF8FC3BEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLinkToProBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkToProBat dialog

class CPropertyDlgLinkToProBat : public CDialog
{
// Construction
public:
	CPropertyDlgLinkToProBat(CWnd* pParent = NULL);   // standard constructor

	void initListBoxBody();
	void initListBoxHead();

// Dialog Data
	//{{AFX_DATA(CPropertyDlgLinkToProBat)
	enum { IDD = IDD_DIALOG_LINKTOPROBAT };
	CListCtrl	m_List;
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	int          m_nWindowsType;//1 过程 2 过程序列 3 统计 4 统计序列
	CString	m_showText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgLinkToProBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLinkToProBat)
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

#endif // !defined(AFX_PROPERTYDLGLINKTOPROBAT_H__F94E3BC5_3CEA_40A6_ADCD_36EF8FC3BEC3__INCLUDED_)
