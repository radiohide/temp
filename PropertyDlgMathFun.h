#if !defined(AFX_PROPERTYDLGMATHFUN_H__0FC7A624_0361_4F2F_86F3_1C0F3E7D5BA3__INCLUDED_)
#define AFX_PROPERTYDLGMATHFUN_H__0FC7A624_0361_4F2F_86F3_1C0F3E7D5BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgMathFun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMathFun dialog

class CPropertyDlgMathFun : public CDialog
{
// Construction
public:
	CPropertyDlgMathFun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgMathFun)
	enum { IDD = IDD_DIALOG_MATHFUN };
	CComboBox	m_comboxFunDesp;
	CString	m_strFunDesp;
	CString	m_strFunName;
	CString	m_strBlockDesp;
	CString m_strPropertityTableName;
	UINT	m_iInputCount;
	//}}AFX_DATA
public:
	CStringArray *m_pStrArrFunDisp;//所有方法描述
	CStringArray *m_pStrArrFunName;//所有方法名称


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgMathFun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgMathFun)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFundesp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGMATHFUN_H__0FC7A624_0361_4F2F_86F3_1C0F3E7D5BA3__INCLUDED_)
