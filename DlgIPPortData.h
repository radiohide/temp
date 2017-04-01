#if !defined(AFX_DLGIPPORTDATA_H__E7C560F3_E838_43A4_81B6_FE5F0DC279C8__INCLUDED_)
#define AFX_DLGIPPORTDATA_H__E7C560F3_E838_43A4_81B6_FE5F0DC279C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIPPortData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIPPortData dialog

class CDlgIPPortData : public CDialog
{
// Construction
public:
	CDlgIPPortData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIPPortData)
	enum { IDD = IDD_DIALOG_IPPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CString m_StrIP;
	CString m_StrPort;
	CString m_strModeName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIPPortData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIPPortData)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIPPORTDATA_H__E7C560F3_E838_43A4_81B6_FE5F0DC279C8__INCLUDED_)
