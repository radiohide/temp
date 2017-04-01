#if !defined(AFX_PROPERTYDLGPULSE_H__332944C4_B3B3_4FA1_AF83_750DDC522FA5__INCLUDED_)
#define AFX_PROPERTYDLGPULSE_H__332944C4_B3B3_4FA1_AF83_750DDC522FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgPulse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPulse dialog

class CPropertyDlgPulse : public CDialog
{
// Construction
public:
	CPropertyDlgPulse(CWnd* pParent = NULL);   // standard constructor

	int  m_index;
// Dialog Data
	//{{AFX_DATA(CPropertyDlgPulse)
	enum { IDD = IDD_DIALOG_PULSE };
	CEdit	m_EditStep;
	CEdit	m_EditInitValue;
	CComboBox	m_CombSource;
	double	m_dFengzhi;
	double	m_dZhouqi;
	double	m_dYanchi;
	double	m_dMaichongKD;
	double	m_dInitValue;
	double	m_dStep;
	double	m_dFengzhiNegative;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgPulse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgPulse)
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit6();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGPULSE_H__332944C4_B3B3_4FA1_AF83_750DDC522FA5__INCLUDED_)
