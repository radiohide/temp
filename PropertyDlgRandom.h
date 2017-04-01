#if !defined(AFX_PROPERTYDLGRANDOM_H__C62B3487_BC51_42FB_974F_C5729E3403CB__INCLUDED_)
#define AFX_PROPERTYDLGRANDOM_H__C62B3487_BC51_42FB_974F_C5729E3403CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRandom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRandom dialog

class CPropertyDlgRandom : public CDialog
{
// Construction
public:
	CPropertyDlgRandom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRandom)
	enum { IDD = IDD_DIALOG_RANDOM };
	int	m_dMin;
	int	m_dMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRandom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRandom)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRANDOM_H__C62B3487_BC51_42FB_974F_C5729E3403CB__INCLUDED_)
