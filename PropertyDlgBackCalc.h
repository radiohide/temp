#if !defined(AFX_PROPERTYDLGBACKCALC_H__CAB7E679_8F8B_41F2_B6FC_59E0FDE91172__INCLUDED_)
#define AFX_PROPERTYDLGBACKCALC_H__CAB7E679_8F8B_41F2_B6FC_59E0FDE91172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgBackCalc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgBackCalc dialog

class CPropertyDlgBackCalc : public CDialog
{
// Construction
public:
	CPropertyDlgBackCalc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgBackCalc)
	enum { IDD = IDD_DIALOG_BACKCALC };
	int		m_iBackCalc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgBackCalc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgBackCalc)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGBACKCALC_H__CAB7E679_8F8B_41F2_B6FC_59E0FDE91172__INCLUDED_)
