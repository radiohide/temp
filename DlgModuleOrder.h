#if !defined(AFX_DLGMODULEORDER_H__6042BF1E_3D71_40B4_BBDA_46216449038C__INCLUDED_)
#define AFX_DLGMODULEORDER_H__6042BF1E_3D71_40B4_BBDA_46216449038C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModuleOrder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModuleOrder dialog

class CDlgModuleOrder : public CDialog
{
// Construction
public:
	CDlgModuleOrder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModuleOrder)
	enum { IDD = IDD_DIALOG_MODULEORDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModuleOrder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModuleOrder)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODULEORDER_H__6042BF1E_3D71_40B4_BBDA_46216449038C__INCLUDED_)
