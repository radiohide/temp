#if !defined(AFX_PROPERTYDLGSTRTONUMDATA_H__EFAF4EF3_EF5B_4C1E_B15E_32D83C38E296__INCLUDED_)
#define AFX_PROPERTYDLGSTRTONUMDATA_H__EFAF4EF3_EF5B_4C1E_B15E_32D83C38E296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStrToNumData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrToNumData dialog

class CPropertyDlgStrToNumData : public CDialog
{
// Construction
public:
	CPropertyDlgStrToNumData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStrToNumData)
	enum { IDD = IDD_DIALOG_STRTONUMDATA };
	CListCtrl	m_List;
	//}}AFX_DATA
	CStringArray *pStrValueArr;
	CArray<double,double> *pDValueArr;
	CStringArray *pStrDespArr;
private:
	void InitListBoxHead();
	void InitListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStrToNumData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStrToNumData)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTRTONUMDATA_H__EFAF4EF3_EF5B_4C1E_B15E_32D83C38E296__INCLUDED_)
