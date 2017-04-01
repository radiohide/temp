#if !defined(AFX_PROPERTYDLGRTDBHISTSERIESBATWP_H__F49AAC09_9781_4A0C_BD1E_C51C2D510735__INCLUDED_)
#define AFX_PROPERTYDLGRTDBHISTSERIESBATWP_H__F49AAC09_9781_4A0C_BD1E_C51C2D510735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRTDBHistSeriesBatWP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropertyDlgRTDBHistSeriesBatWP dialog

class PropertyDlgRTDBHistSeriesBatWP : public CDialog
{
// Construction
public:
	PropertyDlgRTDBHistSeriesBatWP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PropertyDlgRTDBHistSeriesBatWP)
	enum { IDD = IDD_DIALOG_RTDBHistSeriesBat };
	CListCtrl	m_List;
	int		iSnapshotSpan;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropertyDlgRTDBHistSeriesBatWP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CStringArray strVarArr;//
	CStringArray strPortArr;//
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
		
	int iRawOrSnapshot;//0:原始；1:快照
private:
	void initListBoxBody();
	void initListBoxHead();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PropertyDlgRTDBHistSeriesBatWP)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRTDBHISTSERIESBATWP_H__F49AAC09_9781_4A0C_BD1E_C51C2D510735__INCLUDED_)
