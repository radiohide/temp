#if !defined(AFX_PROPERTYDLGRTPOINTBAT_H__E83EB9AC_A45A_4B55_84D0_5C563E2FA392__INCLUDED_)
#define AFX_PROPERTYDLGRTPOINTBAT_H__E83EB9AC_A45A_4B55_84D0_5C563E2FA392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRTPointBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPointBat dialog

class CPropertyDlgRTPointBat : public CDialog
{
// Construction
public:
	CPropertyDlgRTPointBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRTPointBat)
	enum { IDD = IDD_PROPERTYDLGRTPOINTBAT_DIALOG };
	int m_iSysOrInput;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRTPointBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CStringArray strRTDBTagArr;//实时库点名
	CStringArray strPortTagArr;//端口点名
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
private:
	void initListBoxHead();
	void initListBoxBody();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRTPointBat)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonInput();
	afx_msg void OnRadio1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRTPOINTBAT_H__E83EB9AC_A45A_4B55_84D0_5C563E2FA392__INCLUDED_)
