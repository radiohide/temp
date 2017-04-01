#if !defined(AFX_DLGDEPMODEL_H__BB01EB83_3C44_4285_AB1E_A294E304C030__INCLUDED_)
#define AFX_DLGDEPMODEL_H__BB01EB83_3C44_4285_AB1E_A294E304C030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDepModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDepModel dialog

class CDlgDepModel : public CDialog
{
// Construction
public:
	CDlgDepModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDepModel)
	enum { IDD = IDD_DIALOG_DEPMODEL };
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA
	
	CStringArray strIPChangeArr;//本次操作有多少IP修改过

	bool beEdit;//被编辑过，即与数据库不一致。
	void InitListBoxHead();
	void InitListBoxBody(int tagLocalOrRemote);
	void WriteData(_ConnectionPtr &pCon);
	void RefurbishMangerToolList();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDepModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDepModel)
	afx_msg void OnButtonEdit();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonApply();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEPMODEL_H__BB01EB83_3C44_4285_AB1E_A294E304C030__INCLUDED_)
