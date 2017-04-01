#if !defined(AFX_DLGCFGSETIME_H__9766D801_E5D4_4505_B4BC_A9367E9BA7DF__INCLUDED_)
#define AFX_DLGCFGSETIME_H__9766D801_E5D4_4505_B4BC_A9367E9BA7DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgSETime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgSETime dialog

class CDlgCfgSETime : public CDialog
{
// Construction
public:
	CDlgCfgSETime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgSETime)
	enum { IDD = IDD_DIALOG_CFGSETIME };
	CTime	m_EndDate;
	CTime	m_EndTime;
	CTime	m_StartDate;
	CTime	m_StartTime;

	CTime m_inStartTime;
	CTime m_inEndTime;
	//}}AFX_DATA

public:
	CTime GetEndTime();
	CTime GetStartTime();
	void SetSETime(CTime sTime,CTime eTime);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgSETime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCfgSETime)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEtime();
	afx_msg void OnRadioCurrent();
	afx_msg void OnRadioSe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGSETIME_H__9766D801_E5D4_4505_B4BC_A9367E9BA7DF__INCLUDED_)
