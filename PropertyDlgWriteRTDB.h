#if !defined(AFX_PROPERTYDLGWRITERTDB_H__ABA13D4A_7819_4842_9690_BB585E971494__INCLUDED_)
#define AFX_PROPERTYDLGWRITERTDB_H__ABA13D4A_7819_4842_9690_BB585E971494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWriteRTDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRTDB dialog

class CPropertyDlgWriteRTDB : public CDialog
{
// Construction
public:
	CPropertyDlgWriteRTDB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWriteRTDB)
	enum { IDD = IDD_DIALOG_WRITERTDB };

	int m_TagTimeOffsetOrInput;//1:采用系统时间+偏移。0:采用外部偏移。
	long m_lOffset;//时间偏移量值
	CString m_strTagName;
	CString m_strTagDesp;
	int m_iPointType;
	int m_iPointStatus;
	BOOL	m_BCheckPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgBCBWriteReal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgBCBWriteReal)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWRITERTDB_H__ABA13D4A_7819_4842_9690_BB585E971494__INCLUDED_)
