#if !defined(AFX_PROPERTYDLGGAIN_H__EEB2A814_A3FA_4454_9DD6_75E5EC27DFE2__INCLUDED_)
#define AFX_PROPERTYDLGGAIN_H__EEB2A814_A3FA_4454_9DD6_75E5EC27DFE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgGain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgGain dialog

class CPropertyDlgGain : public CDialog
{
// Construction
public:
	CPropertyDlgGain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgGain)
	enum { IDD = IDD_DIALOG_GAIN };
	double	m_dGainValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgGain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgGain)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGGAIN_H__EEB2A814_A3FA_4454_9DD6_75E5EC27DFE2__INCLUDED_)
