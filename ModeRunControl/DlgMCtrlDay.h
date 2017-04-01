#if !defined(AFX_DLGMCTRLDAY_H__922E6DF9_C171_46CB_8F3A_D2B556B3105E__INCLUDED_)
#define AFX_DLGMCTRLDAY_H__922E6DF9_C171_46CB_8F3A_D2B556B3105E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMCtrlDay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDay dialog
#include "DlgMCtrlType.h"
#include "MCtrlDay.h"
#include "DlgSetTimeAndSETime.h"

class CDlgMCtrlDay : public CDlgMCtrlType
{
// Construction
public:
	CDlgMCtrlDay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMCtrlDay)
	enum { IDD = IDD_DIALOG_MCTRL_DAY };
	CListCtrl	m_List;
	//}}AFX_DATA

	void InitListBoxHead();
	void InitListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMCtrlDay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void ShowCtrlData();
	virtual void SetCtrlData();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMCtrlDay)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCTRLDAY_H__922E6DF9_C171_46CB_8F3A_D2B556B3105E__INCLUDED_)
