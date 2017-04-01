#if !defined(AFX_DLGDISDATA_H__610625E6_82E4_4B00_9F60_52D5AF5CD52C__INCLUDED_)
#define AFX_DLGDISDATA_H__610625E6_82E4_4B00_9F60_52D5AF5CD52C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDisData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDisData dialog

class CDlgDisData : public CDialog
{
// Construction
public:
	CDlgDisData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDisData)
	enum { IDD = IDD_DIALOG_DISDATA };
	double	m_dDisValue;
	double	m_dEValue;
	double	m_dSValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDisData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDisData)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISDATA_H__610625E6_82E4_4B00_9F60_52D5AF5CD52C__INCLUDED_)
