#if !defined(AFX_DLGMCTRLMONTH_H__F3C02515_EBD7_486F_B6D0_0EF2F7FEAB62__INCLUDED_)
#define AFX_DLGMCTRLMONTH_H__F3C02515_EBD7_486F_B6D0_0EF2F7FEAB62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMCtrlMonth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlMonth dialog
#include "DlgMCtrlType.h"
#include "MCtrlMonth.h"

class CDlgMCtrlMonth : public CDlgMCtrlType
{
// Construction
public:
	CDlgMCtrlMonth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMCtrlMonth)
	enum { IDD = IDD_DIALOG_MCTRL_MONTH };
	CTreeCtrl	m_ctlTree;
	CListCtrl	m_List;
	//}}AFX_DATA
	HTREEITEM m_hRoot;//根节点

	void InitTree();
	void InitListBoxHead();
	void InitListBoxBody(int mInNum);//mInNum ：1月到12月：0~11；全部为12

	virtual void ShowCtrlData();
	virtual void SetCtrlData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMCtrlMonth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMCtrlMonth)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeMonth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnselmonth();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCTRLMONTH_H__F3C02515_EBD7_486F_B6D0_0EF2F7FEAB62__INCLUDED_)
