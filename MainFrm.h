// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__666DD5C5_8116_4A1B_A9C8_6D0B8169E37B__INCLUDED_)
#define AFX_MAINFRM_H__666DD5C5_8116_4A1B_A9C8_6D0B8169E37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CFrameWnd CBCGPFrameWnd

#include "OutputBar.h"
#include "ModuleTree.h"
#include "ModuleLibTree.h"
#include "ClassViewBar.h"
#include "ClassViewModelBar.h"
#include "ClassViewManageBar.h"
#include "DlgNotion.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
// Attributes
	_ConnectionPtr m_pCon; //连接MYSQL的连接
	BOOL           m_bConnectOK; //标志连接成功
	CDlgNotion m_NotionDlg;
public:
//关于模型的变量
	CString m_strModelFolder;//模型所在的路径
	CString m_strArrayModuleList; //模块的列表，表明该模型具有多少个模块文件；对模块文件
	//可采取打开该模块文件，该模块文件中包括了若干个子计算块，每个计算块在模块文件中进行描述；
	//计算块：输入；输出是什么；算法是什么；也是描述，是否需要对象的生成呢？

public:
    UINT m_LastShortCutID; //上次点击的按钮
	int m_iTimerID1;//定时提醒。
	int m_iTimerID2;//定时提醒。

	BOOL m_bInRealRunState; //是否在实时运行的过程中
// Operations

	void StopAllThread();
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ActiveOutPutPage(int Id);
	void ClearOutput(int page);
	void ClearLog(int page);
	void ReloadModuleTree(CString strFile);
	void ParseModuleFile(CString strFile);
	void ReLoadTree();
	//计算块库加载
	void LoadBlockLib();
	void RefreshModelTree(CCalcModel* pModel);
	void ShowOutputWatch(CCalcBlock* pBlock);

	//加密狗函数
	BOOL CheckHasp();

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	CClassViewBar	   m_wndClassView;
public:  // control bar embedded members
	CBCGPMenuBar		m_wndMenuBar;
	CBCGPStatusBar	    m_wndStatusBar;
	CBCGPToolBar		m_wndToolBar;

	COutputBar	        m_wndOutput;	
	CClassViewModelBar m_wndModelView;
	CClassViewManageBar m_wndManageViewBar;
	CBCGPSplitterWnd	m_wndSplitter;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRunRtStart();
	afx_msg void OnUpdateRunRtStart(CCmdUI* pCmdUI);
	afx_msg void OnRunRtStop();
	afx_msg void OnUpdateRunRtStop(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRunHistSeltime();
	afx_msg void OnUpdateRunHistSeltime(CCmdUI* pCmdUI);
	afx_msg void OnRunHistStep2();
	afx_msg void OnUpdateRunHistStep2(CCmdUI* pCmdUI);
	afx_msg void OnRunHistStep3();
	afx_msg void OnUpdateRunHistStep3(CCmdUI* pCmdUI);
	afx_msg void OnRunHistStep4();
	afx_msg void OnUpdateRunHistStep4(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	//}}AFX_MSG
//	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewOutlookBar();
	afx_msg void OnUpdateViewOutlookBar(CCmdUI* pCmdUI);
	void OnOutlookBarShortcut(UINT id);

	LRESULT OnStatus(WPARAM wParam, LPARAM lParam);
	LRESULT OnLog(WPARAM wParam, LPARAM lParam);
	
    //afx_msg void OnDBClickLibTree(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	BOOL CreateDockingBars ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__666DD5C5_8116_4A1B_A9C8_6D0B8169E37B__INCLUDED_)
