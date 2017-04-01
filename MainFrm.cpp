// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "MainFrm.h"
#include "ePerfManageDoc.h"
#include "ePerfManageView.h"

#include "PreviewPane.h"
#include "FViewBlockLink.h"

#include "ThreadRunPara.h"
//#include "CalcModel.h"

#include "ElementInOut.h"
#include "CSpreadSheet.h"

////////////////////////////////////
#include "CalcBlock.h"
#include "CalcUniValue.h"
////////////////////////////////////
#include "CalcModule.h"
#include "CalcModel.h"

#include "TreeItemObj.h"
#include "SocketManage.h"

//����ͷ�ļ�
#pragma comment(lib, "libhasp_windows_81332.lib")
#define Encryption_HASP
#include "hasp_api.h"
//���ܹ���ʱ����
static int g_ErrorNum = 0;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DWORD WINAPI TRunInCycle(LPVOID pthread);
/////////////////////////////////////////////////////////////////////////
extern CPtrArray ListPoteDll;
extern CLog g_pLogfile;

extern  void InitDllMapping();

extern bool g_SocketManageInit(const char* IP,int port);
extern void g_SocketManageExit();
extern CSocketServ* g_ServSocketInit(const char* IP,int port);
extern void g_ServSocketExit(CSocketServ*& pSockServ);
extern CSocketManage* g_pSocketManage;
extern CSocketServ* g_pSocketLogServ;
extern CSocketServ* g_pSocketWorkSpaceServ;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#define WM_TASKGUID		WM_USER+200 

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_RUN_RT_START, OnRunRtStart)
	ON_UPDATE_COMMAND_UI(ID_RUN_RT_START, OnUpdateRunRtStart)
	ON_COMMAND(ID_RUN_RT_STOP, OnRunRtStop)
	ON_UPDATE_COMMAND_UI(ID_RUN_RT_STOP, OnUpdateRunRtStop)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_WM_TIMER()
	ON_COMMAND(ID_RUN_HIST_SELTIME, OnRunHistSeltime)
	ON_UPDATE_COMMAND_UI(ID_RUN_HIST_SELTIME, OnUpdateRunHistSeltime)
	ON_COMMAND(ID_RUN_HIST_STEP2, OnRunHistStep2)
	ON_UPDATE_COMMAND_UI(ID_RUN_HIST_STEP2, OnUpdateRunHistStep2)
	ON_COMMAND(ID_RUN_HIST_STEP3, OnRunHistStep3)
	ON_UPDATE_COMMAND_UI(ID_RUN_HIST_STEP3, OnUpdateRunHistStep3)
	ON_COMMAND(ID_RUN_HIST_STEP4, OnRunHistStep4)
	ON_UPDATE_COMMAND_UI(ID_RUN_HIST_STEP4, OnUpdateRunHistStep4)
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
//	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND(ID_VIEW_OUTLOOKBAR, OnViewOutlookBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLOOKBAR, OnUpdateViewOutlookBar)
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_4, OnOutlookBarShortcut)

	ON_MESSAGE(WM_MSG_MODELRUN_STATUS,OnStatus)
	ON_MESSAGE(WM_MSG_MODELRUN_LOG,OnLog)
	//ON_NOTIFY(NM_DBLCLK, ID_SHORTCUTS_TREE_1, OnSelectModuleTree)
	//ON_NOTIFY(TVN_DBCLICKED, ID_SHORTCUTS_TREE_1, OnSelectModuleTree)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)

	//ON_NOTIFY(TVN_SELCHANGED, ID_SHORTCUTS_TREE_2, OnDBClickLibTree)


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bConnectOK = FALSE;
	// TODO: add member initialization code here
	m_LastShortCutID = -1;
	m_bInRealRunState = FALSE;

	m_strModelFolder = _T("");
}

CMainFrame::~CMainFrame()
{
	
}
int   TestRDBCon(_ConnectionPtr &pCon)
{
	int iRet=0;
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select id from modellist order by id asc limit 0,1");
	try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(...)
	{
		//AfxMessageBox(e.Description());
		return iRet;
	} 
	iRet=1;
	return iRet;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPToolBar::SetLook2000 ();
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	//------------------------------------------------------------------
	// Outlook bar will be created at left, so temporary disable docking
	// at the left side:
	//------------------------------------------------------------------
	EnableDocking (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	if (!CreateDockingBars ())
	{
		return -1;
	}
    m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndModelView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndManageViewBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndClassView);
    m_wndModelView.AttachToTabWnd (&m_wndClassView, DM_STANDARD, FALSE, NULL);
	m_wndManageViewBar.AttachToTabWnd (&m_wndClassView, DM_STANDARD, FALSE, NULL);

	//----------------------------------------------------
	// Outlook bar is created and docking on the left side
	// should be allowed.
	//----------------------------------------------------
	EnableDocking (CBRS_ALIGN_LEFT);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);


	if (!m_wndOutput.Create (_T("���"), this, CRect (0, 0, 150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
    m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndOutput);

	//�����̶߳���
    theApp.g_pThreadPara=new ThreadRunPara;
    theApp.g_pThreadPara->m_hWnd = m_hWnd;
    //�����ģ��
	LoadBlockLib();

	
	//*************��Ʒ����******************
	/*HINSTANCE hDLL=NULL;
	CString path= gGetExeFileDir() + "ProductGuard.dll";
	hDLL = LoadLibrary(path);
	if(hDLL == NULL)
	{
		AfxMessageBox("����'ProductGuard.dll'ʧ��,����ϵ��Ʒ�����ˣ�");
		return -1;
	}
	path= gGetExeFileDir() + "ePerfManage.CFG";
	char *ecode=new char[493];
	GetPrivateProfileString ("Licence", "SN", "", ecode, 493, path);
	typedef int(__stdcall *p_parseSerialNumber)(char *);
	p_parseSerialNumber parseSerialNumber =(p_parseSerialNumber)GetProcAddress(hDLL,"parseSerialNumber");
	
	int exp = parseSerialNumber(ecode);
	FreeLibrary(hDLL);
	if(exp <= 0)
	{
		CString str;
		str.Format("���к���Ч���������Ϊ: %d", exp);
		AfxMessageBox(str);
		return -1;
	}
	else
	{
		CTimeSpan ts(exp);
		long hour=ts.GetTotalHours();
		long minute=ts.GetMinutes();
		long second=ts.GetSeconds();
		CString str;
		str.Format("��ʹ�õ������ð汾������ʹ��ʱ��Ϊ:%.2dСʱ%.2d����%.2d��", hour,minute,second);
		AfxMessageBox(str);
	}
	m_iTimerID1=this->SetTimer(1,(exp-5*60>0?(exp-5*60):1)*1000,NULL);//��ʱ��ʾ
	m_iTimerID2=this->SetTimer(2,exp*1000,NULL);//��ʱ�˳�

  */
	
	//*************��Ʒ����******************
	SetTimer(1,1800000,NULL);	//��Сʱ���һ�μ��ܹ�	
	
	//AfxMessageBox("��ʹ�õ������ð汾������ʹ��ʱ��Ϊ4Сʱ��");
	//m_iTimerID1=this->SetTimer(1,(3600*4-5*60)*1000,NULL);//��ʱ��ʾ
	//m_iTimerID2=this->SetTimer(2,3600*4*1000,NULL);//��ʱ�˳�

	return 0;
}
/*void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
		{
			KillTimer(m_iTimerID1);			
			m_NotionDlg.DoModal();
			break;
		}
	case 2:
		{
			KillTimer(m_iTimerID2);
			if(m_NotionDlg.m_hWnd)
			{
				m_NotionDlg.SendMessage(WM_CLOSE);
			}
			AfxMessageBox("����ʹ��ʱ���ѵ���ллʹ��!");
			this->PostMessage(WM_CLOSE);
			break;
		}
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
    CFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	return TRUE;
}

void CMainFrame::OnViewOutlookBar() 
{
//	ShowControlBar (&m_wndShortcutsBar,
//					!(m_wndShortcutsBar.IsVisible ()),
//					FALSE, TRUE);
//	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutlookBar(CCmdUI* pCmdUI) 
{
	//pCmdUI->SetCheck (m_wndShortcutsBar.IsVisible ());
}

void CMainFrame::OnOutlookBarShortcut(UINT id)
{
	// TODO: process shortcuts bar commands here...
	if(m_LastShortCutID == -1)
	{
		if(theApp.m_pView!=NULL)
		{
			UINT IDBUTTON = id - ID_SHORTCUT_1+1;
			theApp.m_pView->PostMessage(WM_TASKGUID,IDBUTTON);
			m_LastShortCutID = id;
		}
	}
	else
	{
		if(m_LastShortCutID!=id)
		{
			if(theApp.m_pView!=NULL)
			{
				UINT IDBUTTON = id - ID_SHORTCUT_1+1;
				theApp.m_pView->PostMessage(WM_TASKGUID,IDBUTTON);
				m_LastShortCutID = id;
			}

		}

	}
}

//��ʼʵʱ������ģ��
void CMainFrame::OnRunRtStart() 
{
	//////////////////////////////////////////////////
    CEPerfManageDoc* pDoc =(CEPerfManageDoc*)GetActiveDocument();
	//�ĵ�Ӧ������ͼ��
	//////////////////////////////////////////////////
	m_wndOutput.Clear_Ouput(0);
	//////////////////////////////////////////////////////////////////////
    /*g_pThreadPara->intCycle=1;
	g_pThreadPara->m_pModel = pDoc->m_pModel;

	DWORD ThreadID;
	DWORD code;
	if(!GetExitCodeThread(g_hThreadCycle,&code)||(code!=STILL_ACTIVE))
	{
		g_hThreadCycle=CreateThread(NULL,0,TRunInCycle,g_pThreadPara,0,&ThreadID);//��������ʼ�߳�
	}
	
	m_bInRealRunState = TRUE;*/
}

void CMainFrame::OnUpdateRunRtStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bInRealRunState);
}
//ֹͣģ�͵�ʵʱ����
void CMainFrame::OnRunRtStop() 
{
	// TODO: Add your command handler code here
	StopAllThread();
	m_bInRealRunState = FALSE;
}

void CMainFrame::OnUpdateRunRtStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bInRealRunState);
}
//ֹͣ�߳�
void CMainFrame::StopAllThread()
{
	DWORD code;
	if(GetExitCodeThread(theApp.g_hThreadCycle,&code))
		if(code==STILL_ACTIVE)									//�����ǰ�̻߳��
		{
			TerminateThread(theApp.g_hThreadCycle,0);						//ǰЩ��ֹ�߳�
			CloseHandle(theApp.g_hThreadCycle);								//�����߳̾��
		}
/*	if(GetExitCodeThread(hThreadSeries,&code))
		if(code==STILL_ACTIVE)									//�����ǰ�̻߳��
		{
			TerminateThread(hThreadSeries,0);						//ǰЩ��ֹ�߳�
			CloseHandle(hThreadSeries);								//�����߳̾��
		}
	if(GetExitCodeThread(hThreadCalcOne,&code))
		if(code==STILL_ACTIVE)									//�����ǰ�̻߳��
		{
			TerminateThread(hThreadCalcOne,0);						//ǰЩ��ֹ�߳�
			CloseHandle(hThreadCalcOne);								//�����߳̾��
		}*/
}
LRESULT CMainFrame::OnStatus(WPARAM wParam, LPARAM lParam)
{
	char* pMsg = (char*) lParam;
    CString str;
	str.Format("%s",pMsg);
	m_wndOutput.Output_Add(0, str);

	return 0;
}
LRESULT CMainFrame::OnLog(WPARAM wParam, LPARAM lParam)
{
	char* pMsg = (char*) lParam;
    CString str;
	str.Format("%s",pMsg);
	m_wndOutput.Log_Add(2, str);
	return 0;
}
void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!m_wndOutput.IsVisible (),
					FALSE, FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());

}
//ȷ����ʷʱ��Σ���Ҫ��Ϊģ�͵Ĳ�㹤���ռ�������׼��
////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRunHistSeltime() 
{

	/////////////////////////////////////////////////
	
	/////////////////////////////////////////////////
	/*
	//����Դ���󣺳�������
	CSource* pSource1 = new CSource();
	CSource* pSource2 = new CSource();
    CCalcBlock *pBlock = new CCalcBlock();
    //create 
	pBlock
	pBlock->ReadBlock();
	int inNum = pBlock->GetInputNum();
	int outNum = pBlock->GetOutputNum();
	pBlock->InitCalc();
	pBlock->Calc();
*/
	/*
	// TODO: Add your command handler code here
    //����һ������Ϊ���
	CElementInOut *pElement = new CElementInOut("A1");
	g_WorkSpaceList.SetAt("A1",pElement);

	//strcpy(pElement->LongID,Rows[1]);
	strcpy(pElement->sDsp,"���A1");
	strcpy(pElement->sUnit,"MW");
	//pElement->sCalcSource = "ģ��1";
	pElement->PValue = 540;


	pElement = new CElementInOut("A2",16.2);
	g_WorkSpaceList.SetAt("A2",pElement);

	//strcpy(pElement->LongID,Rows[1]);
	strcpy(pElement->sDsp,"���A2");
	strcpy(pElement->sUnit,"MW");
	//pElement->sCalcSource = "ģ��1";
	//pElement->PValue = 16.2;


	pElement = new CElementInOut("R","[PerMHan]*2");
	g_WorkSpaceList.SetAt("R",pElement);

	strcpy(pElement->sDsp,"���R");
	strcpy(pElement->sUnit,"MW");


	//CString Sform = "$hsstp([A1],[A2])" ;// 

	CString Sform = "([A1]*[A2])/2.0" ;

	pElement = new CElementInOut("PerMHan",Sform);
	g_WorkSpaceList.SetAt("PerMHan",pElement);

	strcpy(pElement->sDsp,"���PerMHan");
	strcpy(pElement->sUnit,"KJ");
	//���н���
	pElement->ParseFormula();

	POSITION pos = g_WorkSpaceList.GetStartPosition();
    while(pos)
    {
		CString strKey;
		CElementInOut *pObj;
        g_WorkSpaceList.GetNextAssoc(pos, strKey, pObj);
		CString strDesp = pObj->sDsp ;
		if(!pObj)
			continue;
		if(pObj->PType != 1) //�Ǽ���㣬�����н���
			continue;
	
		int result = pObj->ParseFormula();
		if(result != 0)
		{
			CString strLog;	strLog.Format("\"%s\"ģ���\"%s\"�Ĺ�ʽ�����д���",pObj->sCalcSource,pObj->LongID);
			g_pLogfile.WriteFileData(strLog);
		}
    }

	pos = g_WorkSpaceList.GetStartPosition();
    while(pos)
    {
		CString strKey;
		CElementInOut *pObj;
        g_WorkSpaceList.GetNextAssoc(pos, strKey, pObj);
		CString strDesp = pObj->sDsp ;
		if(!pObj)
			continue;
		if(pObj->PType != 1) //�Ǽ���㣬�����н���
			continue;
	
		int result = pObj->ParseFormula();
		if(result != 0)
		{
			CString strLog;	strLog.Format("\"%s\"ģ���\"%s\"�Ĺ�ʽ�����д���",pObj->sCalcSource,pObj->LongID);
			g_pLogfile.WriteFileData(strLog);
		}
    }

	//��ʼ���м���
	CString strName = "R";
	CElementInOut* RtPt = g_WorkSpaceList[strName];
	RtPt->CalcFormula();
    double value1 = RtPt->PValue;

	strName = "A1";
	CElementInOut* RtPt1 = g_WorkSpaceList[strName];

	RtPt1->CalcFormula();

	double value = RtPt1->PValue;

	{
			POSITION pos = RtPt1->m_listRef.GetHeadPosition();
			while(pos)
			{
				char * strKey;
				CElementInOut *pObj = (CElementInOut *)RtPt1->m_listRef.GetNext(pos);
				double Value=pObj->GetValue();
				time_t PTime=pObj->PTime;
				int PState=pObj->PState;
				char* LongID=pObj->LongID;
				char* sDsp=pObj->sDsp;
				char* sUnit=pObj->sUnit;
			}
	}  
*/	
}

void CMainFrame::OnUpdateRunHistSeltime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//����ȷ������ʷ����ʱ��Σ�Ȼ��������������з���������Щ�����ݲ�����
//���������ݲ����õģ�Ȼ������Ƿ���ì�ܡ�
//���������������ʷ����ֱ���㣬����������Ρ������п��ܼ�������ݲ��ԣ�
//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRunHistStep2() 
{
}
void CMainFrame::OnUpdateRunHistStep2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

////////////////////////////////////////////////////////////////////
//��ѡ��ĳ��ģ��ʱ
/*
void CMainFrame::OnSelectModuleTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTreeItem = m_wndTree.GetSelectedItem ();
	if (hTreeItem != NULL)
	{
		//CWorkspaceObj* pWS = (CWorkspaceObj*) m_wndFolders.GetItemData (hTreeItem);
		//ASSERT_VALID (pWS);

		//CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, GetParentFrame ());
		//if (pMainFrame != NULL && m_bNotifyFrame)
		//{
		//	pMainFrame->SetWorkSpace (pWS);
		//}
		//m_wndTree.
		CString str = m_wndTree.GetItemText(hTreeItem);
		AfxMessageBox(str);
	}
	
	*pResult = 0;
}*/
//������
void CMainFrame::ReLoadTree()
{
   //ɾ�����еĽڵ�
   //m_wndTree.DeleteAllItems();
   //�������ɽڵ�,����ģ���б�
   


}
//��ģ�͵Ĺ���Ϊ����ģ�����ݿ⣬���ػ�Զ�̵Ķ�����



void CMainFrame::ParseModuleFile(CString strFile)
{
	/*
    CString strFullPath = m_strModelFolder + "����ģ��1.xls";

	strFullPath = "d:\\����ģ��1.xls";

	CSpreadSheet SS(strFullPath, "Sheet1",false);

	CStringArray Rows, Column;
	CString tempString = "";
	
	int RowCount = SS.GetTotalRows();    // ������
	CString strCell;
	//��ʼ��ȡ��ģ�飬��������ԣ�
	BOOL bEnd = FALSE;
	int  RowIndex = 2;
	BOOL bIsBlock_B=0,bIsBlock_E=1;
	CBlock* pNewBlock=NULL;
    
	while(RowIndex<=RowCount)
	{
		//if(!SS.ReadRow(Rows, RowIndex)) {CString strLog;	strLog.Format("��ȡ%d������ʧ��",i);AfxMessageBox(strLog);RowIndex++}
       	SS.ReadRow(Rows, RowIndex);
		strCell = Rows[0];

		if(strCell.IsEmpty()) {RowIndex++;continue;}

        //�Ը���������ж�
		if(strCell == "BlockBegin") //����һ����Ŀ�ʼ
		{
			bIsBlock_B = 1; 
			bIsBlock_E = 0;
			//��ʼ���
			pNewBlock = new CBlock();
			RowIndex++; 
			continue;
			
		}
		if(strCell == "BlockEnd")  //����һ����Ľ���
		{
			//˵��ĳ���������ʶ
			if(bIsBlock_B)
			{
				bIsBlock_E = 1; //�ÿ����
				bIsBlock_B = 0; //��ʼ��ʶ

			}
			else
			{
			    TRACE("Some wrong occur\n");
			}
		}
		//�����κ�һ���У����Ƿ�ǰ��û�ж�ȡ���
		if(bIsBlock_B && !bIsBlock_E) //��ʼ��û�н���
		{
			if(strCell == "Block")
			{
				strcpy(pNewBlock->m_sName,Rows[1]);
                strcpy(pNewBlock->m_sType,Rows[2]);
			}
			else if(strCell == "Out")//strcmp(strCell,"Out"))
			{
			    CString strVarName = Rows[1];
				CString strDesp = Rows[2];
                pNewBlock->AddOutput(strVarName,strDesp);
			}
			else if(strCell=="OutInner")
			{
				CString strVarName = Rows[1];
				CString strDesp = Rows[2];
                pNewBlock->AddOutput(strVarName,strDesp);
			}
			else if(strCell=="In")
			{
				CString strVarName = Rows[1];
				CString strDesp = Rows[2];
                pNewBlock->AddInput (strVarName,strDesp);
			}
			else if(strCell=="Express")
			{
	            CString strVarName = Rows[1];
				CString strFormula = Rows[2];
				CString strType = Rows[3];
                pNewBlock->SetExpression(strVarName,strFormula,strType);
			}
			
		}
        //////////////////////////////////////////////////////
		RowIndex++; //��ȡ��һ�����д���
	}
	*/
}
//��ȡ�ļ��������ص����б�ؼ���
void CMainFrame::ReloadModuleTree(CString strFile)
{

}

void CMainFrame::OnRunHistStep3() 
{
	
}

void CMainFrame::OnUpdateRunHistStep3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//
void CMainFrame::OnRunHistStep4() 
{
	
}

void CMainFrame::OnUpdateRunHistStep4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	// TODO: Add your message handler code here
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState != WA_INACTIVE)
	{
		theApp.SetActiveFrame (CEPerfManageApp::MainFrame);
		theApp.m_pMainWnd = this;
	}	
}
//�ӱ��ص�ģ�Ϳ��ļ��м���ģ�ͣ�����鱣�浽MDB�ļ���
//
void CMainFrame::LoadBlockLib()
{
    ///////////////////////////////////////////////////////////
   if(theApp.m_bLibConOK)
   m_wndClassView.FillClassView(theApp.m_pConLib);
   ///////////////////////////////////////////////////////////
}
//���Ӵ���
void CMainFrame::ShowOutputWatch(CCalcBlock* pBlock)
{
	pBlock->ShowBlockInfoInListCtrl (m_wndOutput.GetWatchListCtrl());
}
//���ݵ�ǰ���ĵ�
void CMainFrame::RefreshModelTree(CCalcModel* pModel)
{
	m_wndModelView.RefreshModelTree(pModel);
	
}
BOOL CMainFrame::CreateDockingBars ()
{
	//------------------
	// Crate class view:
	//------------------
	if (!m_wndClassView.Create (_T("ģ���"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_VIEW_CLASS,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View bar\n");
		return FALSE;      // fail to create
	}

	//HICON hClassViewIcon = (HICON) ::LoadImage (::AfxGetResourceHandle (), 
	//			MAKEINTRESOURCE (IDI_CLASS_VIEW),
	//			IMAGE_ICON, ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0);
	//m_wndClassView.SetIcon (hClassViewIcon, FALSE);

	if (!m_wndModelView.Create (_T("ģ�͹���"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_MODELCLASS,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return FALSE;      // fail to create
	}

	if (!m_wndManageViewBar.Create (_T("ģ��ά��"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_MODELMANAGECLASS,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return FALSE;      // fail to create
	}

	m_wndManageViewBar.RefreshTreeContent();

	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	/*CRect rectClient;
	GetClientRect (rectClient);

	m_wndSplitter.CreateStatic (this, 2, 1);
	m_wndSplitter.CreateView (0, 0, pContext->m_pNewViewClass, 
							CSize (0, rectClient.Height ()* 2/ 3), pContext);
	m_wndSplitter.CreateView (1, 0, RUNTIME_CLASS (CFViewBlockLink), 
							CSize (0, rectClient.Height () / 3), pContext);
	return TRUE;*/
	return CFrameWnd::OnCreateClient(lpcs, pContext);
	
}

void CMainFrame::ClearOutput(int page)
{
		m_wndOutput.Clear_Ouput(page);
}
void CMainFrame::ClearLog(int page)
{
		m_wndOutput.Clear_Log(page);
}
void CMainFrame::ActiveOutPutPage(int Id)
{
		m_wndOutput.ActivePage(Id);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
	if(pDoc!=NULL)
	{
		if(pDoc->m_pModel!=NULL)
		{
			if(pDoc->m_pModel->GetModuleCount()>0)//����ģ������ģ��ʱ��ʾ����
			{
				int ret=AfxMessageBox("�Ƿ�Ҫ���浱ǰģ�ͣ�",MB_YESNOCANCEL);
				if(IDCANCEL==ret)
					return;
				else if(IDYES==ret)
				{
					if(theApp.m_pView!=NULL)
					{
						theApp.m_pView->SendMessage(WM_TASKGUID,4);
					}
					CFrameWnd::OnClose();
				}
				else if(IDNO==ret)
					CFrameWnd::OnClose();
			}
			else
				CFrameWnd::OnClose();
		}
		else
			CFrameWnd::OnClose();
	}
	else
		CFrameWnd::OnClose();
}

//�������غ���
CString strLog;
BOOL CMainFrame::CheckHasp()
{
#ifdef Encryption_HASP
	
	//�Ƚ��м��ܹ���½
	
	//login 
	const hasp_feature_t feature = 9;	//�˴����з���Աע�⣺Ӧ�����ù�����Ա��ȡ�Լ������Ӧ��feature id����ʾ����1Ϊʵʱ��ʷ���ݿ������feature id
	
	hasp_handle_t handle = HASP_INVALID_HANDLE_VALUE;
	hasp_status_t status;
	
	unsigned char vendor_code[] = "jYYLIo4kJSyCUwuNEBi6mzbTmq4J9jeigktb2Z2J4M01+0Y4D3o6JDM5pDf2WvJ8EhU5stZMxV5JLX2EOHO9jyemfVLoZcRhYwoyPOdZA3WSK8H9Lr+qwG8k4KxK4tInoDk/P5i4u0Ypb8AcuulrKzM0pe4ij56Ur2v02k1fSZMhp4JmKV0stPKtuJ5EFyLZ5jJa/SXJsdpvVcae3MFefPYf5hCdDrqXttbGvbCHl/GGdWvzxqE0iXTyozBmZlPBBn+MppLylyVZZLLlLVgagwLx2PtfQI/QvN8lKwVoZ5eyx9oNvydh8U/u8LS8+6YF58bgsv5/nWEexgWpyaWoyOvqbk9X1AhaSN9V9pCDL35ydOcOIHEfgRdCt+zRoDwy4CyJWzFXSFAZPq7vjTdVazcLvQdQnihQoluzxoxseiXzjGxxV7m17eqWh8COG+8yxm/qR/OwvGoJLPeGL7tTZRgAriDXSomiOABcKROi1gowX3A6IuLkkR0OBTHpj7+Tm2qglAkp66F7m5SNWMK0//g5SyVTHvXXQ2VfooO/k+hLo/GLbvdxtvbngPqMknQlGbwGPteLRwpNiZ68JILgkDwF9l87vM5HjT3wZXD61AlbKtuv8NSBYYxCUsDYGWJG6xdnxTAkH9PTzAbrJ9V90Fm31mv9UybteHAL13wgBeGJluA6jCPq2tD0ksNBCzRySRQuexFy66QMjW5B7VdPgPPYraG62FLAeqcyBi0GOYlVkTV42S6QgZWz6nxhJgSCLIoSwQw3bFsrEPBVJQ5rCCqzlHv/Zvzmi5JndpgpZGXVbLtR5lsXOwrYGdJaLdLRJOx9Q/EiUEV0C8SF4sYhCotdMgfJTDmIT3YzTNjAKulGma7IuBvOCuTx/rfEZwRcPFcjPKuyYPep4zJRvwb56Idn0vvC8ZCwNjGCZ1f393Y=";
	status = hasp_login(feature, vendor_code, &handle);
	
		
	if (status != HASP_STATUS_OK)
	{
		switch (status)
		{
		case HASP_FEATURE_NOT_FOUND:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_FEATURE_NOT_FOUND");
			break;
		case HASP_HASP_NOT_FOUND:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_HASP_NOT_FOUND");
			break;
		case HASP_OLD_DRIVER:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_OLD_DRIVER");
			break;
		case HASP_NO_DRIVER:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_NO_DRIVER");
			break;
		case HASP_INV_VCODE:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_INV_VCODE");
			break;
		case HASP_FEATURE_TYPE_NOT_IMPL:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_FEATURE_TYPE_NOT_IMPL");
			break;
		case HASP_TMOF:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_TMOF");
			AfxMessageBox("HASP_TMOF");
			break;
		case HASP_TS_DETECTED:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_TS_DETECTED");
			break;
			break;
		case HASP_LOCAL_COMM_ERR:
			//status = HASP_STATUS_OK;	//������ôд����λע�⡣���Ǽ�������һ��bug.?��ôд���幷Ҳ����true,����
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","HASP_LOCAL_COMM_ERR");
			break;
		default:
			strLog.Format("����,ԭ����%s!��ȷ���Ƿ��ѻ����ȷ�������Ȩ,����ϵͳ��װ����ȷ�ļ��ܹ�","��������");
			break;
		}
	}
	else
		hasp_logout(handle);
	
	if(status)
	{
		//AfxMessageBox(strLog);
		return false;
	}
	
#endif
	return true;
	
}


void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
#ifdef Encryption_HASP
	//BOOL bDog = CheckHasp();
	if(CheckHasp())
	{
		g_ErrorNum = 0;
	}
	else
	{	
		g_ErrorNum++;
		if(g_ErrorNum >= 480)	//
		{
			AfxMessageBox(strLog);	
			KillTimer(1); 
			::PostQuitMessage(0);
		}
	}
#endif	
	//CMainFrame::OnTimer(nIDEvent);
}

//��������ܽ���
