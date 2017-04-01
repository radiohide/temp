#if !defined(AFX_PROPERTYDLGTABCOALOUT_H__CC602CCB_4ADC_489B_BFDA_06053DCC33D4__INCLUDED_)
#define AFX_PROPERTYDLGTABCOALOUT_H__CC602CCB_4ADC_489B_BFDA_06053DCC33D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTabCoalOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalOut dialog

class CPropertyDlgTabCoalOut : public CDialog
{
// Construction
public:
	CPropertyDlgTabCoalOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTabCoalOut)
	enum { IDD = IDD_DIALOG_TAB_COALOUT };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTabCoalOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
		CStringArray *m_pStrCoalTagName;
		CStringArray *m_pStrCoalTagDesp;
		CStringArray *m_pStrCoalTagUnit;
private:
	void initListBoxBody();
	void initListBoxHead();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTabCoalOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonEdit();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTABCOALOUT_H__CC602CCB_4ADC_489B_BFDA_06053DCC33D4__INCLUDED_)
