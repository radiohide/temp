#if !defined(AFX_PROPERTYDLGSINEWAVE_H__A5B81493_E2EF_463E_827B_89572F48321A__INCLUDED_)
#define AFX_PROPERTYDLGSINEWAVE_H__A5B81493_E2EF_463E_827B_89572F48321A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgSineWave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSineWave dialog

class CPropertyDlgSineWave : public CDialog
{
// Construction
public:
	CPropertyDlgSineWave(CWnd* pParent = NULL);   // standard constructor

	int  m_index;
// Dialog Data
	//{{AFX_DATA(CPropertyDlgSineWave)
	enum { IDD = IDD_DIALOG_SINEWAVE };
	CEdit	m_EditStep;
	CEdit	m_EditInitValue;
	CComboBox	m_CombSource;
	double	m_dFengzhi;
	double	m_dZhouqi;
	double	m_dLeftRight;
	double	m_dUpDown;
	double	m_dInitValue;
	double	m_dStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgSineWave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgSineWave)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSINEWAVE_H__A5B81493_E2EF_463E_827B_89572F48321A__INCLUDED_)
