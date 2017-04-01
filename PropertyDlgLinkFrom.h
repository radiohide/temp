#if !defined(AFX_PROPERTYDLGLINKFROM_H__9424E297_E6F1_414E_922D_7F528B889463__INCLUDED_)
#define AFX_PROPERTYDLGLINKFROM_H__9424E297_E6F1_414E_922D_7F528B889463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLinkFrom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFrom dialog

class CPropertyDlgLinkFrom : public CDialog
{
// Construction
public:
	CPropertyDlgLinkFrom(CWnd* pParent = NULL);   // standard constructor
	int GetType(int num);
	void SetType(int num,int type);
// Dialog Data
	//{{AFX_DATA(CPropertyDlgLinkFrom)
	enum { IDD = IDD_DIALOG_LINKFROM };
	CString	m_VarTag1;
	CString	m_VarTag2;
	CString	m_VarTag3;
	CString	m_VarTag4;
	CString	m_VarTag5;
	//}}AFX_DATA
	CString m_StrType1;
	CString m_StrType2;
	CString m_StrType3;
	CString m_StrType4;
	CString m_StrType5;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgLinkFrom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLinkFrom)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGLINKFROM_H__9424E297_E6F1_414E_922D_7F528B889463__INCLUDED_)
