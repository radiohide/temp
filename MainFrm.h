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
	_ConnectionPtr m_pCon; //����MYSQL������
	BOOL           m_bConnectOK; //��־���ӳɹ�
	CDlgNotion m_NotionDlg;
public:
//����ģ�͵ı���
	CString m_strModelFolder;//ģ�����ڵ�·��
	CString m_strArrayModuleList; //ģ����б�������ģ�;��ж��ٸ�ģ���ļ�����ģ���ļ�
	//�ɲ�ȡ�򿪸�ģ���ļ�����ģ���ļ��а��������ɸ��Ӽ���飬ÿ���������ģ���ļ��н���������
	//����飺���룻�����ʲô���㷨��ʲô��Ҳ���������Ƿ���Ҫ����������أ�

public:
    UINT m_LastShortCutID; //�ϴε���İ�ť
	int m_iTimerID1;//��ʱ���ѡ�
	int m_iTimerID2;//��ʱ���ѡ�

	BOOL m_bInRealRunState; //�Ƿ���ʵʱ���еĹ�����
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
	//���������
	void LoadBlockLib();
	void RefreshModelTree(CCalcModel* pModel);
	void ShowOutputWatch(CCalcBlock* pBlock);

	//���ܹ�����
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
