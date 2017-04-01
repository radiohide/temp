#if !defined(AFX_DLGTASKSUPTIME_H__A60D0B91_60EB_482A_BC25_D2BE36F11DE1__INCLUDED_)
#define AFX_DLGTASKSUPTIME_H__A60D0B91_60EB_482A_BC25_D2BE36F11DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTaskSUpTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskSUpTime dialog

class CDlgTaskSUpTime : public CDialog
{
// Construction
public:
	CDlgTaskSUpTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTaskSUpTime)
	enum { IDD = IDD_DIALOG_TASK_STARTUP_TIME };
	CTime	m_StartDate;
	CTime	m_StartTime;

	CTime m_inStartTime;
	//}}AFX_DATA

public:
	void SetSTime(CTime sTime);
	CTime GetStartTime();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTaskSUpTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTaskSUpTime)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCurrent();
	afx_msg void OnRadioSe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASKSUPTIME_H__A60D0B91_60EB_482A_BC25_D2BE36F11DE1__INCLUDED_)
