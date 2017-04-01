#if !defined(AFX_DLGDISCRETEVALUE_H__94193686_73AE_4821_97B7_19B4DDD0355B__INCLUDED_)
#define AFX_DLGDISCRETEVALUE_H__94193686_73AE_4821_97B7_19B4DDD0355B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiscreteValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDiscreteValue dialog

class CDlgDiscreteValue : public CDialog
{
// Construction
public:
	void InitList();
	void SetLstCtrlStyle();
	CDlgDiscreteValue(CWnd* pParent = NULL);   // standard constructor

public:
	void FillList();
	CStringArray  *m_division_code;
	CStringArray  *m_code_desp;
	CStringArray  *m_low_value;
	CStringArray  *m_high_value;
// Dialog Data
	//{{AFX_DATA(CDlgDiscreteValue)
	enum { IDD = IDD_DIALOG_DISCRETEVALUE };
	CListCtrl	m_ctrListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiscreteValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDiscreteValue)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISCRETEVALUE_H__94193686_73AE_4821_97B7_19B4DDD0355B__INCLUDED_)
