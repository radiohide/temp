// ePerfManage.h : main header file for the EPERFMANAGE application
//

#if !defined(AFX_EPERFMANAGE_H__6806E9C6_D4B6_41D6_98C1_F0D032ACB9C3__INCLUDED_)
#define AFX_EPERFMANAGE_H__6806E9C6_D4B6_41D6_98C1_F0D032ACB9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp:
// See ePerfManage.cpp for the implementation of this class
//
#include "TreeItemObj.h"
#include "FlowchartEditor/FlowchartClipboardHandler.h"

#include "ThreadRunPara.h"

class CEPerfManageApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	    HANDLE                 g_hThreadScheduleTask;//ʵʱѭ������ľ��
	    HANDLE                 g_hThreadCycle;//ʵʱѭ������ľ��
		HANDLE                 g_hThreadInterActive; //�����û�ָ����ʱ��
       ThreadRunPara *   g_pThreadPara; //ȫ�ֵ��̲߳���ָ��

		CFlowchartClipboardHandler	m_clip;
public:
	CEPerfManageApp();

	CView *m_pView;

    CTreeItemObj* m_pTreeItemObj;

	_ConnectionPtr m_pConLocalModel; //���ӱ���ģ�͵�����
	_ConnectionPtr m_pConRemoteModel; //����Զ��ģ�͵�����
	_ConnectionPtr m_pConLib; //����ģ��������

	enum OUTLOOK_FRAME
	{
		MainFrame,
		ModelFrame
	};
	BOOL m_bExistModelFrame;
public:
	BOOL m_bLibConOK;
	BOOL m_bModelLocalConOK;
	BOOL m_bModelRemoteConOK;
	BOOL EstablishLibaryConnect();
	BOOL EstablishLocalModelConnect();
	BOOL EstablishRemoteModelConnect();
	void SetActiveFrame (OUTLOOK_FRAME frame);
	
public:
	BOOL EstablishMySQLConnectPtr(_ConnectionPtr &m_pCon,CString strDBip,CString strDBname,CString strUsername,CString strPassword);
	BOOL EstablishMDBConnectPtr(_ConnectionPtr &m_pCon,CString strFileName);
	void CloseConnectPrt(_ConnectionPtr &pCon);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEPerfManageApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEPerfManageApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnViewClosepage();
	afx_msg void OnUpdateViewClosepage(CCmdUI* pCmdUI);
	afx_msg void OnViewCloseallpage();
	afx_msg void OnUpdateViewCloseallpage(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CEPerfManageApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPERFMANAGE_H__6806E9C6_D4B6_41D6_98C1_F0D032ACB9C3__INCLUDED_)
