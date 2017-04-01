#if !defined(AFX_PROPERTYDLGRTPOINTADV_H__22957841_5F23_4851_AD1E_8678E3EBEFDD__INCLUDED_)
#define AFX_PROPERTYDLGRTPOINTADV_H__22957841_5F23_4851_AD1E_8678E3EBEFDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRTPointAdv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPointAdv dialog

class CPropertyDlgRTPointAdv : public CDialog
{
// Construction
public:
	CPropertyDlgRTPointAdv(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRTPointAdv)
	enum { IDD = D_DIALOG_RTPOINTADV };
		// NOTE: the ClassWizard will add data members here
	CString	m_strTagDesp;
	CString	m_strTag1;
	CString	m_strTag2;
	CString	m_strTag3;
	CString	m_strTag4;
	CString	m_strTag5;
    int		m_iOutMethod;	
	BOOL	m_bChkLimit;
	double	m_dHighLimit;
	double	m_dLowLimit;
	double	m_dConstWhenBad;
	BOOL	m_bForceEnable;
	double	m_dForceValue;
	long	m_lOffset;
	int		m_iInOrOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgBCBReadReal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgBCBReadReal)
	afx_msg void OnRadio1();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRTPOINTADV_H__22957841_5F23_4851_AD1E_8678E3EBEFDD__INCLUDED_)
