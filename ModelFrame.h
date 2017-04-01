// ModelFrame.h: interface for the CModelFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELFRAME_H__62F6EAF5_1ABD_433E_894E_5BBB6E9034E6__INCLUDED_)
#define AFX_MODELFRAME_H__62F6EAF5_1ABD_433E_894E_5BBB6E9034E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadRunPara.h"
#include "CalcModel.h"

class CModelFrame : public CBCGPFrameWnd  
{
	DECLARE_DYNCREATE(CModelFrame)
public:
	CModelFrame();
	virtual ~CModelFrame();

protected:  // control bar embedded members
	CBCGPMenuBar	m_wndMenuBar;
	CBCGPToolBar	m_wndToolBar;
	CBCGPStatusBar	m_wndStatusBar;

	CWnd*			m_pMainWnd;
	// Operations
public:
	    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailFrame)
	public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMailFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
//	afx_msg void OnRunStopexecuteInwindow();
	//}}AFX_MSG
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MODELFRAME_H__62F6EAF5_1ABD_433E_894E_5BBB6E9034E6__INCLUDED_)
