#if !defined(AFX_PROPERTYDLGSTRMERGE_H__3D355162_FFDB_4145_AECE_730B9784E1F2__INCLUDED_)
#define AFX_PROPERTYDLGSTRMERGE_H__3D355162_FFDB_4145_AECE_730B9784E1F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStrMerge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMerge dialog

class CPropertyDlgStrMerge : public CDialog
{
// Construction
public:
	CPropertyDlgStrMerge(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStrMerge)
	enum { IDD = IDD_DIALOG_STRMERGE };
	int		m_strInputNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStrMerge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStrMerge)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTRMERGE_H__3D355162_FFDB_4145_AECE_730B9784E1F2__INCLUDED_)
