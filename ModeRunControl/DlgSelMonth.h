#if !defined(AFX_DLGSELMONTH_H__8783B63B_CBEB_4D7D_A07B_49D2F8A7D52E__INCLUDED_)
#define AFX_DLGSELMONTH_H__8783B63B_CBEB_4D7D_A07B_49D2F8A7D52E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelMonth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelMonth dialog

class CDlgSelMonth : public CDialog
{
// Construction
public:
	CDlgSelMonth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelMonth)
	enum { IDD = IDD_DIALOG_SELMONTH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CArray<int,int> m_mNumArr;
	void SetMonthNum(CArray<int,int> &mNumArr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelMonth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelMonth)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELMONTH_H__8783B63B_CBEB_4D7D_A07B_49D2F8A7D52E__INCLUDED_)
