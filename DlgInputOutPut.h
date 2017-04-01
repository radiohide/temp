#if !defined(AFX_DLGINPUTOUTPUT_H__D5C62B3B_B6E7_49A0_9186_6E9A2256C35D__INCLUDED_)
#define AFX_DLGINPUTOUTPUT_H__D5C62B3B_B6E7_49A0_9186_6E9A2256C35D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputOutPut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutPut dialog

class CDlgInputOutPut : public CDialog
{
// Construction
public:
	CDlgInputOutPut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputOutPut)
	enum { IDD = IDD_DLGINPUTOUTPUT_DIALOG };
	CString	m_strDesp;
	CString	m_strInput;
	CString	m_strOutput;
	CString	m_strUnit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputOutPut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputOutPut)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTOUTPUT_H__D5C62B3B_B6E7_49A0_9186_6E9A2256C35D__INCLUDED_)
