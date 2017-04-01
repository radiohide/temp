#if !defined(AFX_PROPERTYDLGSTABLEJUDGE_H__DE3764BA_B1E8_47DC_A1A6_A721A7DDFFD1__INCLUDED_)
#define AFX_PROPERTYDLGSTABLEJUDGE_H__DE3764BA_B1E8_47DC_A1A6_A721A7DDFFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStableJudge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStableJudge dialog

class CPropertyDlgStableJudge : public CDialog
{
// Construction
public:
	CPropertyDlgStableJudge(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStableJudge)
	enum { IDD = IDD_DIALOG_STABLEJUDGE };
	double	m_dDown2;
	double	m_dDown1;
	double	m_dFazhi1;
	double	m_dFazhi2;
	int		m_iRadio;
	BOOL	m_BCalcNoStable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStableJudge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStableJudge)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTABLEJUDGE_H__DE3764BA_B1E8_47DC_A1A6_A721A7DDFFD1__INCLUDED_)
