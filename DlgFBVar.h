#if !defined(AFX_DLGFBVAR_H__8A688CF9_A0A7_406B_A6BF_346037CCCE71__INCLUDED_)
#define AFX_DLGFBVAR_H__8A688CF9_A0A7_406B_A6BF_346037CCCE71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFBVar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFBVar dialog
#include "XListCtrl.h"

class CDlgFBVar : public CDialog
{
// Construction
public:
	CDlgFBVar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFBVar)
	enum { IDD = IDD_DIALOG_RULE_FBVAR };
	CXListCtrl	m_List;
	//}}AFX_DATA
	void InitListBoxHead();
	void FillListBoxBody();

	CStringArray *pNameArr;
	CStringArray *pDespArr;
	CArray<int,int> *pTagBeInArr;//参与规则推导的前件变量标记,1:参与,0:不参与

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFBVar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void saveCurListSel();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFBVar)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFBVAR_H__8A688CF9_A0A7_406B_A6BF_346037CCCE71__INCLUDED_)
