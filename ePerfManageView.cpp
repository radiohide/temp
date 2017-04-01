// ePerfManageView.cpp : implementation of the CEPerfManageView class
//

#include "stdafx.h"
#include "ePerfManage.h"

#include "ePerfManageDoc.h"
#include "ePerfManageView.h"

#include "ViewModuleFrm.h"
#include "ViewDebugFrm.h"
#include "ViewWSpaceFrm.h"

#include "CalcModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CEPerfManageView


IMPLEMENT_DYNCREATE(CEPerfManageView, CBCGPTabView)

BEGIN_MESSAGE_MAP(CEPerfManageView, CBCGPTabView)
	//{{AFX_MSG_MAP(CEPerfManageView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TASKGUID, OnUserGuid) 
	ON_MESSAGE(WM_TASKGUID_CLOSE_CURRENT_TABVIEW, OnUserTabClose) //do
	ON_MESSAGE(WM_TASKGUID_CLOSE_ALL_TABVIEW, OnUserAllTabClose) 
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageView construction/destruction

CEPerfManageView::CEPerfManageView()
{
	// TODO: add construction code here

}

CEPerfManageView::~CEPerfManageView()
{
}

BOOL CEPerfManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageView drawing

void CEPerfManageView::OnDraw(CDC* pDC)
{
	CEPerfManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}
int CEPerfManageView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//加上这句话才能自动删除窗口!
	m_wndTabs.AutoDestroyWindow (true);
	//m_wndTabs.SetActiveTabColor(RGB(255,0,0));

	return 0;
}

CEPerfManageDoc *CEPerfManageView::GetDocumentFromView()
{
	CEPerfManageDoc* pDoc = GetDocument();
	return pDoc;
}

LRESULT CEPerfManageView::OnUserGuid(WPARAM wParam,LPARAM lParam)
{
	CEPerfManageDoc *pDoc = GetDocument();
	if(pDoc==NULL ) return -1;
	switch (wParam)
	{
	case 0: //关闭所有的
		{
          m_wndTabs.RemoveAllTabs();
		break;
		}
	case 1: //打开
		{
			CString strModuleName =theApp.m_pTreeItemObj->m_strName ;
			int index = GetTabIndexByLable(strModuleName);
			if(index<0)
			{
				AddView(RUNTIME_CLASS (CViewModuleFrm),strModuleName,100);
				SetActiveView(m_wndTabs.GetTabsNum()-1);
				CViewModuleFrm *pView = (CViewModuleFrm *)GetActiveView();
				pView->m_strModuleName = strModuleName;
				pView->OnInitialUpdate();
			}
			else
			{
				SetActiveView(index);
			}
		   break;
		}
		case 11: //打开模型，不进行活动
		{
			CString strModuleName =theApp.m_pTreeItemObj->m_strName ;
			int index = GetTabIndexByLable(strModuleName);
			if(index<0)
			{
				AddView(RUNTIME_CLASS (CViewModuleFrm),strModuleName,100);
				CWnd* pWnd=NULL;
				if(FindTabWndByLable(pWnd,strModuleName))
				{
					CViewModuleFrm *pView=(CViewModuleFrm *)pWnd;
					pView->m_strModuleName = strModuleName;
					pView->OnInitialUpdate();
				}
			}
		   break;
		}
	case 2:
		{
			CString strModuleName =_T("调试窗口") ;
			int index = GetTabIndexByLable(strModuleName);
			if(index<0)
			{
				AddView(RUNTIME_CLASS (CViewDebugFrm),strModuleName,100);
				SetActiveView(m_wndTabs.GetTabsNum()-1);
				CViewDebugFrm *pView = (CViewDebugFrm *)GetActiveView();
				pView->OnInitialUpdate();
				pView->LoadModel(pDoc->m_pModel);
				pView->SetFocusListCtrl();
			}
			else
			{
				SetActiveView(index);
				CViewDebugFrm *pView = (CViewDebugFrm *)GetActiveView();
				pView->LoadModel(pDoc->m_pModel);
				pView->SetFocusListCtrl();
			}
		break;
		}
	case 20:
		{
			CString strModuleName =_T("调试窗口") ;
			int index = GetTabIndexByLable(strModuleName);
			if(index>=0)
			{
				RemoveView(index);
			}
		break;
		}
	case 3:
		{
			CString strTabName =theApp.m_pTreeItemObj->m_strName ;
			int index = GetTabIndexByLable(strTabName);
			if(index<0)
			{
				AddView(RUNTIME_CLASS (CViewWSpaceFrm),strTabName,100);
				SetActiveView(m_wndTabs.GetTabsNum()-1);
				CViewWSpaceFrm *pView = (CViewWSpaceFrm *)GetActiveView();
				pView->TagType=theApp.m_pTreeItemObj->m_iItemType;
				pView->OnInitialUpdate();
			}
			else
			{
				if(wParam==3)
					SetActiveView(index);
				//CViewWSpaceFrm *pView = (CViewWSpaceFrm *)GetActiveView();
				CWnd* pWnd=NULL;
				if(FindTabWndByLable(pWnd,strTabName))
				{
					CViewWSpaceFrm *pView=(CViewWSpaceFrm *)pWnd;
					pView->FillGCtrl();
				}
			}
			break;
		}
	case 30://只刷新数据，不一定会设置CViewWSpaceFrm为活动页面
		{
			CView *pActiveView=GetActiveView();
			if(pActiveView!=NULL)
			{
				((CViewUserFrm*)pActiveView)->RefreshWindow();
			}
			break;
		}
		case 4://保存所有模块
		{
			if(pDoc->m_pModel!=NULL)
			{
				CCalcModule* pModule;
				for(int i=0;i<pDoc->m_pModel->GetModuleCount();i++)
				{
					pModule=pDoc->m_pModel->GetModuleByIndex(i);
					if(pModule!=NULL)
					{
						CString strModule=pModule->GetModuleName();
						CWnd* pWnd=NULL;
						if(FindTabWndByLable(pWnd,strModule))
						{
							CViewModuleFrm *pView=(CViewModuleFrm *)pWnd;
							pView->OnFileSaveModule();
						}
					}
				}
			}
			break;
		}
	}
	if(pDoc->m_pModel!=NULL)
	{
		CString str;
		if(pDoc->m_pModel->m_iTagModeLocalOrRemote==0)//local
		{
			str.Format("VeStore-DisCalc--%s--本地",pDoc->m_pModel->m_strModelName);
		}
		else
			str.Format("VeStore-DisCalc--%s--远程",pDoc->m_pModel->m_strModelName);
		((CMainFrame*)AfxGetMainWnd())->SetWindowText(str);
	}	
	return 0;
}

LRESULT CEPerfManageView::OnUserAllTabClose(WPARAM wParam,LPARAM lParam)
{
	int allTabsNum=m_wndTabs.GetTabsNum();
	for(int i=allTabsNum-1;i>=0;i--)
	{
		RemoveView(i);
	}
	return 0;
}
void CEPerfManageView::CloseUserTabByName(CString strName)
{
	int index = GetTabIndexByLable(strName);
	if(index>=0)
	{
		RemoveView(index);
	}
}
LRESULT CEPerfManageView::OnUserTabClose(WPARAM wParam,LPARAM lParam)
{
	int actTabIndex=m_wndTabs.GetActiveTab();

	CString str;
	str.Format("MSG_关闭！TabView序号:%d",actTabIndex);
	//AfxMessageBox(str);

	RemoveView(actTabIndex);

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CEPerfManageView diagnostics

#ifdef _DEBUG
void CEPerfManageView::AssertValid() const
{
	CView::AssertValid();
}

void CEPerfManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEPerfManageDoc* CEPerfManageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEPerfManageDoc)));
	return (CEPerfManageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageView message handlers

void CEPerfManageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
}
//显示模块的时候，需要根据名称找到模块下的计算块信息
//打开一个模块，新增一个页面，看有没有页面

//当双击到某个块的时候，应该到该模块的定义页面[定义输入输出，定义属性]
//测点对象块：测点的个数，每个测点
void CEPerfManageView::ShowSelectedBlock(CString strModuleName,CString strBlockName)
{

}

BOOL CEPerfManageView::FindTabWndByLable(CWnd*& pWnd,CString str)
{
	pWnd = NULL;
	int n = m_wndTabs.GetTabsNum();
	BOOL bFind =FALSE;
	for(int i=0;i<n;i++)
	{
		CString strLable;
		BOOL bOK = m_wndTabs.GetTabLabel(i,strLable);
		if(bOK)
		{
			if(strLable==str)
			{
				bFind = TRUE;
				pWnd = m_wndTabs.GetTabWnd(i);
				break;
			}
		}
	
	}
	return bFind;
}

int CEPerfManageView::GetTabIndexByLable(CString strtabText)
{
    int index=-1;
   	int n = m_wndTabs.GetTabsNum();
	for(int i=0;i<n;i++)
	{
		CString strLable;
		BOOL bOK = m_wndTabs.GetTabLabel(i,strLable);
   	    if(strLable==strtabText)
		{
			index = i;
			break;
		}
	}
	return index;

}
CView* CEPerfManageView::GetTabViewByIndex(int index)
{
	if((index>GetTabShowCount()-1)||(index<0))
		return NULL;
	return	(CView*)m_wndTabs.GetTabWnd(index);
}

int CEPerfManageView::GetTabShowCount()
{
	return	m_wndTabs.GetTabsNum();
}

void CEPerfManageView::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPTabView::OnSize(nType, cx, cy);
	
    TRACE("onsize,view\n");	
}

