#if !defined(AFX_DLGNEWMODEL_H__BC347260_46F0_4F13_B356_79CC52487744__INCLUDED_)
#define AFX_DLGNEWMODEL_H__BC347260_46F0_4F13_B356_79CC52487744__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog

class CDlgNewModel : public CDialog
{
// Construction
public:
	CDlgNewModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewModel)
	enum { IDD = IDD_DIALOG_ADDMODEL };
	CString	m_strDesp;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewModel)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWMODEL_H__BC347260_46F0_4F13_B356_79CC52487744__INCLUDED_)
