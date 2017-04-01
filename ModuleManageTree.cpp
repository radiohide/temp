// ModuleManageTree.cpp: implementation of the CModuleManageTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ModuleManageTree.h"
#include "TreeItemObj.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModuleManageTree::CModuleManageTree()
{

}

CModuleManageTree::~CModuleManageTree()
{
	while (!m_lstWorkspaces.IsEmpty ())
	{
		delete m_lstWorkspaces.RemoveHead ();
	}
}
BEGIN_MESSAGE_MAP(CModuleManageTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CModuleLibTree)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CModuleManageTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
		cs.style |= WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
				TVS_HASLINES | TVS_HASBUTTONS | 
				TVS_DISABLEDRAGDROP | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeCtrl::PreCreateWindow(cs);
}

int CModuleManageTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
   //m_menu.LoadMenu(IDR_POPUP_LIBBLOCK);
	return 0;
}
//添加
void CModuleManageTree::LoadManageNodes()
{
	CTreeItemObj* pWS = NULL;
	CString strTreeItem=_T("模型维护");
	HTREEITEM hRoot = InsertItem (strTreeItem,0,0);
	SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
	
	pWS = new CTreeItemObj(strTreeItem,0,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hRoot, (DWORD) pWS);
	///////////////////////////////////////
	strTreeItem = _T("实时测点视图");
	HTREEITEM hClass =InsertItem (strTreeItem,1,1, hRoot);
	pWS = new CTreeItemObj(strTreeItem,1,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hClass, (DWORD) pWS);

	strTreeItem = _T("关系测点视图");
	hClass =InsertItem (strTreeItem,1,1, hRoot);
	pWS = new CTreeItemObj(strTreeItem,2,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hClass, (DWORD) pWS);

	strTreeItem = _T("工作空间视图");
	hClass =InsertItem (strTreeItem,1,1, hRoot);
	pWS = new CTreeItemObj(strTreeItem,3,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hClass, (DWORD) pWS);

	Expand (hRoot, TVE_EXPAND);
}
