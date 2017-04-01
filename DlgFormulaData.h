#if !defined(AFX_DLGFORMULADATA_H__32E3A620_2536_4592_9655_2379D745429C__INCLUDED_)
#define AFX_DLGFORMULADATA_H__32E3A620_2536_4592_9655_2379D745429C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaData dialog

class CDlgFormulaData : public CDialog
{
// Construction
public:
	CDlgFormulaData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormulaData)
	enum { IDD = IDD_DIALOG_FORMUDATA };
	CString	m_EditFormula;
	CString	m_EditVarName;
	CString	m_StrType;
	//}}AFX_DATA

	void SetType(int type);
	int GetType();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaData)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFORMULADATA_H__32E3A620_2536_4592_9655_2379D745429C__INCLUDED_)
