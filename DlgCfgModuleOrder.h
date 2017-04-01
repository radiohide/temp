#if !defined(AFX_DLGCFGMODULEORDER_H__6A0B502C_912A_4F7F_AF53_B0D2417F07E9__INCLUDED_)
#define AFX_DLGCFGMODULEORDER_H__6A0B502C_912A_4F7F_AF53_B0D2417F07E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgModuleOrder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgModuleOrder dialog

class CDlgCfgModuleOrder : public CDialog
{
// Construction
public:
	CDlgCfgModuleOrder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgModuleOrder)
	enum { IDD = IDD_DIALOG_CFGMODULEORDER };
	CListCtrl	m_List;
	//}}AFX_DATA
	
	CArray<CCalcModule*,CCalcModule*&> m_ArrModule;
	
	int currentSelIndex;

	void SetModuleList(CModuleList &moduleList);
	void InitListBoxBody();
	void InitListBoxHead();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgModuleOrder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCfgModuleOrder)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnButtonMoveDown();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGMODULEORDER_H__6A0B502C_912A_4F7F_AF53_B0D2417F07E9__INCLUDED_)
