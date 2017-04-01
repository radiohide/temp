#if !defined(AFX_DLGWINDPOWER_H__3DDB34F1_99DF_4FEA_BD03_C3747DE407C0__INCLUDED_)
#define AFX_DLGWINDPOWER_H__3DDB34F1_99DF_4FEA_BD03_C3747DE407C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWindPower.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWindPower dialog

class CDlgWindPower : public CDialog
{
// Construction
public:
	CDlgWindPower(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWindPower)
	enum { IDD = IDD_DLGWINDPOWER_DIALOG };
	double	m_dPower;
	double	m_dWindSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWindPower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWindPower)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWINDPOWER_H__3DDB34F1_99DF_4FEA_BD03_C3747DE407C0__INCLUDED_)
