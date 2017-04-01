#if !defined(AFX_PROPERTYDLGWASP_H__521BE8D1_4F18_4532_8BDE_74592254FB08__INCLUDED_)
#define AFX_PROPERTYDLGWASP_H__521BE8D1_4F18_4532_8BDE_74592254FB08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWASP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWASP dialog

class CPropertyDlgWASP : public CDialog
{
// Construction
public:
	CPropertyDlgWASP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWASP)
	enum { IDD = IDD_DIALOG_WASP };
	CComboBox	m_comboxFunDesp;
	CString	m_strFunDesp;
	CString	m_strFunName;
	CString	m_strBlockDesp;
	//}}AFX_DATA
public:
	CStringArray *m_pStrArrFunDisp;//所有方法描述
	CStringArray *m_pStrArrFunName;//所有方法名称

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgWASP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgWASP)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFundesp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWASP_H__521BE8D1_4F18_4532_8BDE_74592254FB08__INCLUDED_)
