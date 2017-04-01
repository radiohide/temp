// ClassViewBar.cpp: implementation of the CClassViewBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ClassViewBar.h"
#include "TreeItemObj.h"
#include "ePerfManageDoc.h"
#include "MainFrm.h"
#include "FVModuleView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassViewBar::CClassViewBar()
{
}

CClassViewBar::~CClassViewBar()
{
}

BEGIN_MESSAGE_MAP(CClassViewBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CClassViewBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_COMMAND(ID_LIBBLOCK_PROP, OnProperties)
	ON_COMMAND(ID_LIBBLOCK_ADD, OnAddLibBlock) //添加计算块
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassViewBar message handlers

int CClassViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create views:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | 
								WS_CLIPCHILDREN;
	
	if (!m_wndClassView.Create (dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_ClassViewImages.Create (IDB_CLASS_VIEW, 16, 0, RGB (255, 0, 0));
	m_wndClassView.SetImageList (&m_ClassViewImages, TVSIL_NORMAL);

	return 0;
}

void CClassViewBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	//AdjustLayout ();
	int nBorderSize =1;
	m_wndClassView.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

//计算库
void CClassViewBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID (pWndTree);

	if (point != CPoint (-1, -1))
	{
		//---------------------
		// Select clicked item:
		//---------------------
		CPoint ptTree = point;
		pWndTree->ScreenToClient (&ptTree);

		HTREEITEM hTreeItem = pWndTree->HitTest (ptTree);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem (hTreeItem);
			CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
			ASSERT_VALID (pWS);
			if(pWS->m_iItemType!=0)
			{
				return;
			}
		}
	}
	else  return;



	pWndTree->SetFocus ();
	CMenu menu;
	menu.LoadMenu (IDR_POPUP_LIBBLOCK);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPFrameWnd)))
	{
	   CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	   
	   if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
		   return;
	   
	   ((CBCGPFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
	   //UpdateDialogControls(this, FALSE);
	}
}

void CClassViewBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}
}

BOOL CClassViewBar::PreTranslateMessage(MSG* pMsg) 
{
	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}
void CClassViewBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndClassView.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (1, 1);
	dc.Draw3dRect (rectTree, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
}
void CClassViewBar::FillClassView (_ConnectionPtr &pCon)
{
	m_wndClassView.LoadLibBlocks(pCon);

}
void CClassViewBar::OnProperties() 
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID (pWndTree);

	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();

	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	ASSERT_VALID (pWS);
	CString str;
	str.Format("%s-%d-%s",pWS->m_strName,pWS->m_iItemType,pWS->m_strBlockType);
	AfxMessageBox(str);
	
}
//当添加模块时，需要确定添加到哪个模块中

void CClassViewBar::OnAddLibBlock()
{
	//确定是哪个模块，哪个类别
	CString strAddType;
	strAddType.Empty();
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
 	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	if(pWS->m_iItemType ==0)
	{
           strAddType = pWS->m_strBlockType; 
	}
	else
	{
		return;
	}
	//////////////////////////////////////////////
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain==NULL) return;
	CView* pView = pMain->GetActiveView();
	if(pView==NULL) return;
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)pView->GetDocument();
	if(pDoc!=NULL)
	{
			pDoc->AddLibBlock(strAddType);
	}
 
}

