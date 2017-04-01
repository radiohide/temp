#if !defined(AFX_PROPERTYDLGRTDBHISTSERIESBAT_H__2927C629_C844_4A01_AB52_BD09F3712D91__INCLUDED_)
#define AFX_PROPERTYDLGRTDBHISTSERIESBAT_H__2927C629_C844_4A01_AB52_BD09F3712D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRTDBHistSeriesBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTDBHistSeriesBat dialog

class CPropertyDlgRTDBHistSeriesBat : public CDialog
{
// Construction
public:
	CPropertyDlgRTDBHistSeriesBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRTDBHistSeriesBat)
	enum { IDD = IDD_PROPERTYDLGRTDBHISTSERIESBAT_DIALOG };
	CListCtrl	m_List;
	int		iSnapshotSpan;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRTDBHistSeriesBat)
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
	//{{AFX_MSG(CPropertyDlgRTDBHistSeriesBat)
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

#endif // !defined(AFX_PROPERTYDLGRTDBHISTSERIESBAT_H__2927C629_C844_4A01_AB52_BD09F3712D91__INCLUDED_)
