#if !defined(AFX_PROPERTYDLGPROCTOSTATIS_H__7A120FC4_078A_46FE_9D6F_90C634ADDE7A__INCLUDED_)
#define AFX_PROPERTYDLGPROCTOSTATIS_H__7A120FC4_078A_46FE_9D6F_90C634ADDE7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgProcToStatis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgProcToStatis dialog

class CPropertyDlgProcToStatis : public CDialog
{
// Construction
public:
	CPropertyDlgProcToStatis(CWnd* pParent = NULL);   // standard constructor

	void initListBoxBody();
	void initListBoxHead();

// Dialog Data
	//{{AFX_DATA(CPropertyDlgProcToStatis)
	enum { IDD = IDD_DIALOG_PROCESSTOSTATIS };
	CListCtrl	m_List;
	int		m_nStatType;
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgProcToStatis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL



// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgProcToStatis)
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

#endif // !defined(AFX_PROPERTYDLGPROCTOSTATIS_H__7A120FC4_078A_46FE_9D6F_90C634ADDE7A__INCLUDED_)
