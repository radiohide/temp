#if !defined(AFX_PROPERTYDLGHISTSERIES_H__8B82FD89_F8ED_4243_8FD3_68734D270D68__INCLUDED_)
#define AFX_PROPERTYDLGHISTSERIES_H__8B82FD89_F8ED_4243_8FD3_68734D270D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgHistSeries.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeries dialog

class CPropertyDlgHistSeries : public CDialog
{
// Construction
public:
	CPropertyDlgHistSeries(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgHistSeries)
	enum { IDD = IDD_DIALOG_HISTSERIES };
	CString	m_strPointName;
	int		m_iDataSource;
	int		m_iReadModel;
	int		m_ISnapStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgHistSeries)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgHistSeries)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGHISTSERIES_H__8B82FD89_F8ED_4243_8FD3_68734D270D68__INCLUDED_)
