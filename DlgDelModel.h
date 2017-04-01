#if !defined(AFX_DLGDELMODEL_H__8C339A2E_FB90_4C37_B717_884EE3E84005__INCLUDED_)
#define AFX_DLGDELMODEL_H__8C339A2E_FB90_4C37_B717_884EE3E84005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDelModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDelModel dialog

class CDlgDelModel : public CDialog
{
// Construction
public:
	CDlgDelModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDelModel)
	enum { IDD = IDD_DIALOG_DEL_MODEL };
	CListCtrl	m_ListCtrl;
	CComboBox	m_ComBo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDelModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	void InitListBoxHead(int tagLocalOrRemote);
	void InitListBoxBody(int tagLocalOrRemote);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDelModel)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboModesource();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDELMODEL_H__8C339A2E_FB90_4C37_B717_884EE3E84005__INCLUDED_)
