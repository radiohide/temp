#if !defined(AFX_DLGOPENMODEL_H__BB90D0B3_49C4_41B1_827F_896BA55339BF__INCLUDED_)
#define AFX_DLGOPENMODEL_H__BB90D0B3_49C4_41B1_827F_896BA55339BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOpenModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenModel dialog

class CDlgOpenModel : public CDialog
{
// Construction
public:
	CDlgOpenModel(CWnd* pParent = NULL);   // standard constructor
    CString m_strSelModelName;
	CString m_strSelModelDesp;
	CString m_strSelModelIP;
	CString m_strPort;
	int m_iTagModeLocalOrRemote;//0:local;1:remote
// Dialog Data
	//{{AFX_DATA(CDlgOpenModel)
	enum { IDD = IDD_DIALOG_SELMODEL };
	CComboBox	m_ComBo;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOpenModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitListBoxHead(int tagLocalOrRemote);
	void InitListBoxBody(int tagLocalOrRemote);

	// Generated message map functions
	//{{AFX_MSG(CDlgOpenModel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboModesource();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPENMODEL_H__BB90D0B3_49C4_41B1_827F_896BA55339BF__INCLUDED_)
