#if !defined(AFX_DLGMCTRLDUTY_H__6BF48E42_28B8_4873_8F8C_635623ABBD42__INCLUDED_)
#define AFX_DLGMCTRLDUTY_H__6BF48E42_28B8_4873_8F8C_635623ABBD42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMCtrlDuty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDuty dialog
#include "DlgMCtrlType.h"
#include "DlgSetTimeAndSETime.h"
class CDlgMCtrlDuty : public CDlgMCtrlType
{
// Construction
public:
	CDlgMCtrlDuty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMCtrlDuty)
	enum { IDD = IDD_DIALOG_MCTRL_DUTY };
	CListCtrl	m_List;
	//}}AFX_DATA

	void InitListBoxHead();
	void InitListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMCtrlDuty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void ShowCtrlData();
	virtual void SetCtrlData();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMCtrlDuty)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCTRLDUTY_H__6BF48E42_28B8_4873_8F8C_635623ABBD42__INCLUDED_)
