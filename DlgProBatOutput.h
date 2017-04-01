#if !defined(AFX_DLGPROBATOUTPUT_H__8D83F528_398A_428E_9A95_F32A47E5A466__INCLUDED_)
#define AFX_DLGPROBATOUTPUT_H__8D83F528_398A_428E_9A95_F32A47E5A466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProBatOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatOutput dialog

class CDlgProBatOutput : public CDialog
{
// Construction
public:
	CDlgProBatOutput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProBatOutput)
	enum { IDD = IDD_DIALOG_PROBATOUTPUT };
	CString	m_strDesp;
	CString	m_strOutput;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProBatOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProBatOutput)
	virtual void OnOK();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROBATOUTPUT_H__8D83F528_398A_428E_9A95_F32A47E5A466__INCLUDED_)
