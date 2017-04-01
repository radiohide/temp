#if !defined(AFX_PROPERTYDLGLINKTO_H__B4972F69_D14D_49FD_AA8D_EC812EE5FFA7__INCLUDED_)
#define AFX_PROPERTYDLGLINKTO_H__B4972F69_D14D_49FD_AA8D_EC812EE5FFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLinkTo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkTo dialog

class CPropertyDlgLinkTo : public CDialog
{
// Construction
public:
	CPropertyDlgLinkTo(CWnd* pParent = NULL);   // standard constructor
	
	int GetType(int num);
	void SetType(int num,int type);
// Dialog Data
	//{{AFX_DATA(CPropertyDlgLinkTo)
	enum { IDD = IDD_DIALOG_LINKTO };
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
	//{{AFX_VIRTUAL(CPropertyDlgLinkTo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLinkTo)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGLINKTO_H__B4972F69_D14D_49FD_AA8D_EC812EE5FFA7__INCLUDED_)
