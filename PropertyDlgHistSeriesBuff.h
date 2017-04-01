#if !defined(AFX_PROPERTYDLGHISTSERIESBUFF_H__BDFA5180_3A1F_4EA0_A6FC_100DECE8F8C2__INCLUDED_)
#define AFX_PROPERTYDLGHISTSERIESBUFF_H__BDFA5180_3A1F_4EA0_A6FC_100DECE8F8C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgHistSeriesBuff.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeriesBuff dialog

class CPropertyDlgHistSeriesBuff : public CDialog
{
// Construction
public:
	CPropertyDlgHistSeriesBuff(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgHistSeriesBuff)
	enum { IDD = IDD_DIALOG_HISTSERIESBUFF };
	int		m_ISnapStep;
	int		m_iDataSource;
	int		m_iReadModel;
	CString	m_strPointName;
	int		m_iOutputTime;
	int		m_iBuffTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgHistSeriesBuff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgHistSeriesBuff)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGHISTSERIESBUFF_H__BDFA5180_3A1F_4EA0_A6FC_100DECE8F8C2__INCLUDED_)
