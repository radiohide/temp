#if !defined(AFX_DLGCOALFLOWTAGDATA_H__07A19742_D288_4A3C_884A_0E8EEAC1591E__INCLUDED_)
#define AFX_DLGCOALFLOWTAGDATA_H__07A19742_D288_4A3C_884A_0E8EEAC1591E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCoalFlowTagData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCoalFlowTagData dialog

class CDlgCoalFlowTagData : public CDialog
{
// Construction
public:
	CDlgCoalFlowTagData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCoalFlowTagData)
	enum { IDD = IDD_DIALOG_COAL_FLOW_TAGDATA };
	CString	m_strDesp;
	CString	m_strTagName;
	CString	m_strUnit;
	//}}AFX_DATA

	CString m_strWinName;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCoalFlowTagData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCoalFlowTagData)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOALFLOWTAGDATA_H__07A19742_D288_4A3C_884A_0E8EEAC1591E__INCLUDED_)
