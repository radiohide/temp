// FoldersTree.cpp : implementation file
//

#include "stdafx.h"
//#include "BCGPOutlookDemo.h"
#include "ModuleTree.h"
#include "MainFrm.h"

#include "CalcModel.h"
#include "CalcModule.h"
#include "CalcBlock.H"

#include "TreeItemObj.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleTree

/*CModuleTree::CModuleTree(const CObList& lstWorkspaces, CImageList& images) :
	m_lstWorkspaces (lstWorkspaces),
	m_images (images)
{
}
*/
CModuleTree::CModuleTree()
{
}
CModuleTree::~CModuleTree()
{
	while (!m_lstWorkspaces.IsEmpty ())
	{
		delete m_lstWorkspaces.RemoveHead ();
	}
}


BEGIN_MESSAGE_MAP(CModuleTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CModuleTree)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleTree message handlers

BOOL CModuleTree::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
				TVS_HASLINES | TVS_HASBUTTONS | 
				TVS_DISABLEDRAGDROP | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeCtrl::PreCreateWindow(cs);
}
//*************************************************************************************
int CModuleTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CModuleTree::RefreshModelTree(CCalcModel* pModel)
{
	if(pModel==NULL) return;

	while (!m_lstWorkspaces.IsEmpty ())
	{
		delete m_lstWorkspaces.RemoveHead ();
	}
    DeleteAllItems();
	m_Items.RemoveAll();
	//////////////////////////////////////////////////
    CTreeItemObj* pWS = NULL;
	//当前模型的描述
	CString strTreeItem;
	strTreeItem.Format("%s[%s]",pModel->m_strModelDesp,pModel->m_strModelName);
	HTREEITEM hRoot = InsertItem (strTreeItem,0,0);
	SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
	
	pWS = new CTreeItemObj(pModel->m_strModelName,2,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hRoot, (DWORD) pWS);
    //循环各个模块
	POSITION pos = pModel->m_ModuleList.GetHeadPosition();
	while(pos)
	{
		//生成模块节点
		CCalcModule* pModule=  (CCalcModule*)pModel->m_ModuleList.GetNext(pos);

		//strTreeItem = pModule->GetModuleDesp();
        strTreeItem.Format("%s[%s]",pModule->GetModuleDesp(),pModule->GetModuleName());
        HTREEITEM hClass =InsertItem (strTreeItem,1,1, hRoot);
		strTreeItem =  pModule->GetModuleName();
		pWS = new CTreeItemObj(strTreeItem,1,"","",false);
        m_lstWorkspaces.AddTail(pWS);
	    SetItemData (hClass, (DWORD) pWS);
		//////////////////////////////////////////////////////////
		/*//在treeView中显示计算块
		POSITION pos1 = pModule->m_BlockList.GetHeadPosition(); 
        while(pos1)
		{
			CCalcBlock* pBlock =  (CCalcBlock*)pModule->m_BlockList.GetNext(pos1);
            //strTreeItem = pBlock->GetBlockDesp();
			strTreeItem.Format("%s[%s]",pBlock->GetBlockDesp(),pBlock->GetBlockName());
			HTREEITEM hItem = InsertItem (strTreeItem, 4, 4, hClass);
			pWS = new CTreeItemObj(pBlock->GetBlockName(),0,pBlock->GetBlockType(),pBlock->GetBlockType(),false);
			m_lstWorkspaces.AddTail(pWS);
			SetItemData (hItem, (DWORD) pWS);
		}
		*/
	}
    Expand (hRoot, TVE_EXPAND);

	//////////////////////////////////////////////////
}

