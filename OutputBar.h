// OutputBar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__5851B3E1_A4E2_4D87_926C_B10595224AC0__INCLUDED_)
#define AFX_OUTPUTBAR_H__5851B3E1_A4E2_4D87_926C_B10595224AC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CCalcBlock;
class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:
	CFont			m_Font;
	CBCGPTabWnd	m_wndTabs;

	//CListCtrl	m_wndList1;
	CListBox    m_wndList1;
	CListBox    m_wndLog;

	//CListCtrl	m_wndList2;
	CBCGPListCtrl	m_wndWatch;
	//CListCtrl	m_wndList3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ActivePage(int pageID);
	CListCtrl* GetWatchListCtrl();
	void Clear_Ouput(int page);
	void Output_Add(int page,CString str);
	void Clear_Log(int page);
	void Log_Add(int page, CString str);
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__5851B3E1_A4E2_4D87_926C_B10595224AC0__INCLUDED_)
