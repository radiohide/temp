#if !defined(AFX_PROPERTYDLGCLOCK_H__697150FE_B5BA_4EAA_8276_DBBCFD952E8B__INCLUDED_)
#define AFX_PROPERTYDLGCLOCK_H__697150FE_B5BA_4EAA_8276_DBBCFD952E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgClock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgClock dialog

class CPropertyDlgClock : public CDialog
{
// Construction
public:
	CPropertyDlgClock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgClock)
	enum { IDD = IDD_DIALOG_CLOCK };
	BOOL	m_iBeHaveTime;
	BOOL	m_iBeHaveBeginEndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgClock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgClock)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGCLOCK_H__697150FE_B5BA_4EAA_8276_DBBCFD952E8B__INCLUDED_)
