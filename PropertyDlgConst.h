#if !defined(AFX_PROPERTYDLGCONST_H__C672B649_8044_4A16_BBF0_32A49AA8CA81__INCLUDED_)
#define AFX_PROPERTYDLGCONST_H__C672B649_8044_4A16_BBF0_32A49AA8CA81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgConst.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgConst dialog

class CPropertyDlgConst : public CDialog
{
// Construction
public:
	CPropertyDlgConst(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgConst)
	enum { IDD = IDD_DIALOG_CONST };
	double	m_dConstValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgConst)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgConst)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGCONST_H__C672B649_8044_4A16_BBF0_32A49AA8CA81__INCLUDED_)
