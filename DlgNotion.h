#if !defined(AFX_DLGNOTION_H__BA9B7466_A533_4F81_91F4_56E0CE1A3E32__INCLUDED_)
#define AFX_DLGNOTION_H__BA9B7466_A533_4F81_91F4_56E0CE1A3E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNotion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNotion dialog
#define IDD_DIALOG_NOTION               219
class CDlgNotion : public CDialog
{
// Construction
public:
	CDlgNotion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNotion)
	enum { IDD = IDD_DIALOG_NOTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNotion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNotion)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNOTION_H__BA9B7466_A533_4F81_91F4_56E0CE1A3E32__INCLUDED_)
