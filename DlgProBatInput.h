#if !defined(AFX_DLGPROBATINPUT_H__86CA6678_6F5E_4E3C_8520_DBEFD3D04A8F__INCLUDED_)
#define AFX_DLGPROBATINPUT_H__86CA6678_6F5E_4E3C_8520_DBEFD3D04A8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProBatInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatInput dialog

class CDlgProBatInput : public CDialog
{
// Construction
public:
	CDlgProBatInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProBatInput)
	enum { IDD = IDD_DIALOG_PROBATINPUT };
	CString	m_strDesp;
	CString	m_strInput;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProBatInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProBatInput)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROBATINPUT_H__86CA6678_6F5E_4E3C_8520_DBEFD3D04A8F__INCLUDED_)
