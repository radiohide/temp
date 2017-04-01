#if !defined(AFX_DLGSELWEEK_H__DDF92E25_FA2B_4652_BC0C_AFCAC3C51286__INCLUDED_)
#define AFX_DLGSELWEEK_H__DDF92E25_FA2B_4652_BC0C_AFCAC3C51286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelWeek.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelWeek dialog

class CDlgSelWeek : public CDialog
{
// Construction
public:
	CDlgSelWeek(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelWeek)
	enum { IDD = IDD_DIALOG_SELWEEK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CArray<int,int> m_wNumArr;
	void SetWeekNum(CArray<int,int> &wNumArr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelWeek)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelWeek)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELWEEK_H__DDF92E25_FA2B_4652_BC0C_AFCAC3C51286__INCLUDED_)
