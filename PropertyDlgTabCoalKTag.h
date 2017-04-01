#if !defined(AFX_PROPERTYDLGTABCOALKTAG_H__B945BB00_19EF_4A29_B84D_4D6A99324764__INCLUDED_)
#define AFX_PROPERTYDLGTABCOALKTAG_H__B945BB00_19EF_4A29_B84D_4D6A99324764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTabCoalKTag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalKTag dialog

class CPropertyDlgTabCoalKTag : public CDialog
{
// Construction
public:
	CPropertyDlgTabCoalKTag(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTabCoalKTag)
	enum { IDD = IDD_DIALOG_TAB_COALKTAG };
	CListCtrl	m_List;
	//}}AFX_DATA

	CString *m_pStrPowerTagName;
	CString *m_pStrPowerUnit;

	CStringArray *m_pStrFlowTagNameArr;
	CStringArray *m_pStrFlowUnitArr;
	CStringArray *m_pStrFlowDespArr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTabCoalKTag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void initListBoxBody();
	void initListBoxHead();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTabCoalKTag)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddFlow();
	afx_msg void OnButtonEditFlow();
	afx_msg void OnButtonDelFlow();
	afx_msg void OnChangeEditTagnamePower();
	afx_msg void OnChangeEditPowerUnit();
	afx_msg void OnDblclkListFlow(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTABCOALKTAG_H__B945BB00_19EF_4A29_B84D_4D6A99324764__INCLUDED_)
