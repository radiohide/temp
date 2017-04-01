// ModelFrame.cpp: implementation of the CModelFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ModelFrame.h"
#include "ePerfManageDoc.h"

///////////////////////////////////////////////////////////////////////////
/*
如何在新的窗口下运行
新窗口运行的机制是在其中建立线程
*/
///////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

IMPLEMENT_DYNCREATE(CModelFrame, CBCGPFrameWnd)

CModelFrame::CModelFrame()
{
	
}

CModelFrame::~CModelFrame()
{
    
}
BEGIN_MESSAGE_MAP(CModelFrame, CBCGPFrameWnd)
	//{{AFX_MSG_MAP(CModelFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//ON_COMMAND(ID_RUN_STOPEXECUTE_INWINDOW, OnRunStopexecuteInwindow)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelFrame message handlers

int CModelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_pMainWnd = theApp.m_pMainWnd;

	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//-----------------
	// Create menu bar:
	//-----------------
	if (!m_wndMenuBar.Create (this, dwDefaultToolbarStyle, ID_MODEL_MENU))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	//---------------------
	// Create main toolbar:
	//---------------------
	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		ID_VIEW_MAIL_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MODELFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

	//-------------------
	// Create status bar:
	//-------------------
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking (CBRS_ALIGN_ANY);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);

	return 0;
}
//*************************************************************************************
LRESULT CModelFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	UINT uiToolBarId = (UINT) wp;
	if (uiToolBarId == IDR_MODELFRAME)
	{
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_FILE_MODEL_NEW));
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_FILE_MODEL_CLOSE));
	}

	return 0;
}
//**************************************************************************************
void CModelFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CBCGPFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE)
	{
		theApp.SetActiveFrame (CEPerfManageApp::ModelFrame);
		theApp.m_pMainWnd = this;
	}
}

//**************************************************************************************
void CModelFrame::OnClose() 
{
	theApp.m_pMainWnd = m_pMainWnd;
	theApp.m_bExistModelFrame = FALSE;
	CBCGPFrameWnd::OnClose();
}
//*************************************************************************************
BOOL CModelFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	BOOL bRes = CBCGPFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
	if (bRes)
	{
		InitialUpdateFrame (pContext->m_pCurrentDoc, TRUE);
	}

	theApp.SetActiveFrame (CEPerfManageApp::ModelFrame);
	return bRes;
}


/*
void CModelFrame::OnRunStopexecuteInwindow() 
{
	// TODO: Add your command handler code here
	DWORD code;
	if(GetExitCodeThread(m_hThreadCycle,&code))
	{
		if(code==STILL_ACTIVE)									//如果当前线程还活动
		{
			TerminateThread(m_hThreadCycle,0);						//前些终止线程
			CloseHandle(m_hThreadCycle);								//销毁线程句柄
		}
	}
	
}
*/