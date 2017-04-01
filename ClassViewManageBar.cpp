// ClassViewManageBar.cpp: implementation of the CClassViewManageBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ClassViewManageBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassViewManageBar::CClassViewManageBar()
{

}

CClassViewManageBar::~CClassViewManageBar()
{

}
BEGIN_MESSAGE_MAP(CClassViewManageBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CClassViewManageBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	//ON_COMMAND(ID_LIBBLOCK_OPEN, OnOpenBlock)
	ON_NOTIFY(NM_DBLCLK, IDC_MANAGETREE, OnDblclkModelTree)

	ON_COMMAND(ID_MODULE_PROP, OnModuleProp)
	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassViewManageBar message handlers

int CClassViewManageBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create views:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | 
								WS_CLIPCHILDREN;
	
	if (!m_wndManageTree.Create (dwViewStyle, rectDummy, this, IDC_MANAGETREE))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_Images.Create (IDB_CLASS_VIEW, 16, 0, RGB (255, 0, 0));
	m_wndManageTree.SetImageList (&m_Images, TVSIL_NORMAL);

	return 0;
}

void CClassViewManageBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	//AdjustLayout ();
	int nBorderSize =1;
	m_wndManageTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}


void CClassViewManageBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndManageTree;
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
		}
	}

	pWndTree->SetFocus ();
	CMenu menu;
	menu.LoadMenu (IDR_POPUP_MODULE);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPFrameWnd)))
	{
	   CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	   
	   if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
		   return;
	   
	   ((CBCGPFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
	   UpdateDialogControls(this, FALSE);
	}
}


BOOL CClassViewManageBar::PreTranslateMessage(MSG* pMsg) 
{
	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}
void CClassViewManageBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndManageTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (1, 1);
	dc.Draw3dRect (rectTree, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
}
void  CClassViewManageBar::OnModuleProp()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndManageTree;
	ASSERT_VALID (pWndTree);

	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();

	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	ASSERT_VALID (pWS);
	CString str;
	str.Format("%s-%d-%s",pWS->m_strName,pWS->m_iItemType,pWS->m_strBlockType);
	AfxMessageBox(str);
}
//双击后的策略
//1.双击模型后，显示的是模型的属性：
// 模块名|描述|计算顺序
//2.双击模块后，显示：模块下定义的计算块列表，
// 计算块名|描述|类型   各个输入端属性：输入/输出变量名，中文描述；左侧是

void CClassViewManageBar::OnDblclkModelTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTreeItem = m_wndManageTree.GetSelectedItem();
	CTreeItemObj* pWS = (CTreeItemObj*) m_wndManageTree.GetItemData (hTreeItem);
	ASSERT_VALID (pWS);

	///////////////////////////////////////////////////////////////////////
	CString str;
	if(pWS->m_iItemType == 0) //如果为计算块被点击
	{
		
	}
	else if(pWS->m_iItemType==1) //实时测点视图
	{
	    theApp.m_pTreeItemObj->m_strName = pWS->m_strName;
		theApp.m_pTreeItemObj->m_iItemType = pWS->m_iItemType;
		theApp.m_pView->PostMessage(WM_TASKGUID,3);
	}
	else if(pWS->m_iItemType==2) //关系测点视图
	{
		theApp.m_pTreeItemObj->m_strName = pWS->m_strName;
		theApp.m_pTreeItemObj->m_iItemType = pWS->m_iItemType;
		theApp.m_pView->PostMessage(WM_TASKGUID,3);
	}
	else if(pWS->m_iItemType==3) //工作空间视图
	{
		theApp.m_pTreeItemObj->m_strName = pWS->m_strName;
		theApp.m_pTreeItemObj->m_iItemType = pWS->m_iItemType;
		theApp.m_pView->PostMessage(WM_TASKGUID,3);
	}
	//////////////////////////////////////////////////////////////////////
	*pResult = 0;
}



void CClassViewManageBar::RefreshTreeContent()
{
	m_wndManageTree.LoadManageNodes();
}
