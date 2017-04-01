#if !defined(AFX_DLGMCTRLWEEK_H__4A41A324_33A2_41EE_8D90_FEBC14F14DA5__INCLUDED_)
#define AFX_DLGMCTRLWEEK_H__4A41A324_33A2_41EE_8D90_FEBC14F14DA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMCtrlWeek.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlWeek dialog
#include "DlgMCtrlType.h"
#include "MCtrlWeek.h"
#include "DlgSelWeek.h"
#include "DlgSetTimeAndSETime.h"

class CDlgMCtrlWeek : public CDlgMCtrlType
{
// Construction
public:
	CDlgMCtrlWeek(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMCtrlWeek)
	enum { IDD = IDD_DIALOG_MCTRL_WEEK };
	CTreeCtrl	m_ctlTree;
	CListCtrl	m_List;
	//}}AFX_DATA
	HTREEITEM m_hRoot;//¸ù½Úµã
	NM_TREEVIEW* pNMTreeView;

	void InitTree();
	void InitListBoxHead();
	void InitListBoxBody(int wInNum);

	virtual void ShowCtrlData();
	virtual void SetCtrlData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMCtrlWeek)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMCtrlWeek)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeWeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnselweek();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCTRLWEEK_H__4A41A324_33A2_41EE_8D90_FEBC14F14DA5__INCLUDED_)
