#if !defined(AFX_DLGLOSTPOWERVAR_H__F9D822E5_3EFA_42EB_997C_EE2DC1A74464__INCLUDED_)
#define AFX_DLGLOSTPOWERVAR_H__F9D822E5_3EFA_42EB_997C_EE2DC1A74464__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLostPowerVar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLostPowerVar dialog

class CDlgLostPowerVar : public CDialog
{
// Construction
public:
	CDlgLostPowerVar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLostPowerVar)
	enum { IDD = IDD_DLGLOSTPOWERVAR_DIALOG };
	CString	m_strDesp;
	CString	m_strInput;
	int		m_iValidMax;
	int		m_iValidMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLostPowerVar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLostPowerVar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOSTPOWERVAR_H__F9D822E5_3EFA_42EB_997C_EE2DC1A74464__INCLUDED_)
