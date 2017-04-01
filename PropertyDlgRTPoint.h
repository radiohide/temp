#if !defined(AFX_PROPERTYDLGRTPOINT_H__D8B1FE37_8D0E_4B61_82B0_7CF674382679__INCLUDED_)
#define AFX_PROPERTYDLGRTPOINT_H__D8B1FE37_8D0E_4B61_82B0_7CF674382679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRTPoint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPoint dialog

class CPropertyDlgRTPoint : public CDialog
{
// Construction
public:
	CPropertyDlgRTPoint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRTPoint)
	enum { IDD = IDD_DIALOG_RTPOINT };
	CComboBox	m_CComboBoxQueryType;
	long	m_lOffset;
	CString	m_strTagName;
	CString	m_strTagDesp;
	int		m_TagTimeOffsetOrInput;
	int		m_TagQueryTimeSpan;
	long	m_lTimeLength;
	int m_iQueryType;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRTPoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRTPoint)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRTPOINT_H__D8B1FE37_8D0E_4B61_82B0_7CF674382679__INCLUDED_)
