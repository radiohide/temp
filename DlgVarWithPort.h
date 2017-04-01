#if !defined(AFX_DLGVARWITHPORT_H__03B88A35_5FBA_4281_A3C4_A7A342A07312__INCLUDED_)
#define AFX_DLGVARWITHPORT_H__03B88A35_5FBA_4281_A3C4_A7A342A07312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVarWithPort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVarWithPort dialog

class CDlgVarWithPort : public CDialog
{
// Construction
public:
	CDlgVarWithPort(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVarWithPort)
	enum { IDD = IDD_DLGVARWITHPORT_DIALOG };
	CString	m_strDesp;
	CString	m_strUnit;
	CString	m_strVarName;
	CString	m_strPortName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVarWithPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVarWithPort)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVARWITHPORT_H__03B88A35_5FBA_4281_A3C4_A7A342A07312__INCLUDED_)
