#if !defined(AFX_PROPERTYDLGSTATADD_H__25376CF5_CDAA_4914_80E5_863846E288A0__INCLUDED_)
#define AFX_PROPERTYDLGSTATADD_H__25376CF5_CDAA_4914_80E5_863846E288A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatAdd dialog

class CPropertyDlgStatAdd : public CDialog
{
// Construction
public:
	CPropertyDlgStatAdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatAdd)
	enum { IDD = IDD_PROPERTYDLGSTATADD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	CString m_strInputEdit;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatAdd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATADD_H__25376CF5_CDAA_4914_80E5_863846E288A0__INCLUDED_)
