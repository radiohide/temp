// ClassViewModelBar.cpp: implementation of the CClassViewModelBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ClassViewModelBar.h"
#include "TreeItemObj.h"

#include "MainFrm.h"
#include "ePerfManageView.h"
#include "ePerfManageDoc.h"
#include "DlgMRename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassViewModelBar::CClassViewModelBar()
{

}

CClassViewModelBar::~CClassViewModelBar()
{

}
BEGIN_MESSAGE_MAP(CClassViewModelBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CClassViewModelBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	//ON_COMMAND(ID_LIBBLOCK_OPEN, OnOpenBlock)
	ON_NOTIFY(NM_DBLCLK, IDC_MODELTREE, OnDblclkModelTree)

	ON_COMMAND(ID_MODULE_ADDBLOCK, OnModuleAddBlock)
	
	ON_COMMAND(ID_MODULE_PROP, OnModuleProp)
	ON_COMMAND(ID_MODULE_RENAME, OnMRename)
	
	ON_UPDATE_COMMAND_UI(ID_MODULE_ADDBLOCK, OnUpdateModuleAddBlock)
	ON_UPDATE_COMMAND_UI(ID_MODULE_PROP, OnUpdateModuleProp)
	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassViewModelBar message handlers

int CClassViewModelBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create views:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | 
								WS_CLIPCHILDREN;
	
	if (!m_wndClassView.Create (dwViewStyle, rectDummy, this, IDC_MODELTREE))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_ClassViewImages.Create (IDB_CLASS_VIEW, 16, 0, RGB (255, 0, 0));
	m_wndClassView.SetImageList (&m_ClassViewImages, TVSIL_NORMAL);

	return 0;
}

void CClassViewModelBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	//AdjustLayout ();
	int nBorderSize =1;
	m_wndClassView.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}


void CClassViewModelBar::OnContextMenu(CWnd* pWnd, CPoint point) 
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

void CClassViewModelBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}
}

BOOL CClassViewModelBar::PreTranslateMessage(MSG* pMsg) 
{
	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}
void CClassViewModelBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndClassView.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (1, 1);
	dc.Draw3dRect (rectTree, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
}
void CClassViewModelBar::RefreshModelTree(CCalcModel* pModel)
{
	  m_wndClassView.RefreshModelTree(pModel);
}
//添加计算块，得到是什么模块，然后弹出对话框来选择哪种计算块
void  CClassViewModelBar::OnModuleAddBlock()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID (pWndTree);
	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	//
	
}
void CClassViewModelBar::OnUpdateModuleAddBlock(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CClassViewModelBar::OnUpdateModuleProp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
void CClassViewModelBar::OnMRename()
{
	bool bRename=false;//是否执行了重命名
	CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID (pWndTree);

	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();

	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	ASSERT_VALID (pWS);
	CString strOldName,strOldDesp;
	CString strSQL,strTable;
	if(pWS->m_iItemType==2)//模型重命名
	{
		CDlgMRename dlg;
		dlg.m_strWinText="重命名模型";
		dlg.m_strName=pDoc->m_pModel->m_strModelName;
		dlg.m_strDesp=pDoc->m_pModel->m_strModelDesp;
		strOldName=pDoc->m_pModel->m_strModelName;
		strOldDesp=pDoc->m_pModel->m_strModelDesp;
		if(IDOK==dlg.DoModal())
		{
			//修改m_pModel内存中的值
			pDoc->m_pModel->m_strModelName=dlg.m_strName;
			pDoc->m_pModel->m_strModelDesp=dlg.m_strDesp;
			//修改m_pMCalControl内存中的值
			pDoc->m_pMCalControl->m_strModelName=dlg.m_strName;
			//更新记录modellist中的记录
			strTable.Format("ModelList");
			strSQL.Format("Update %s SET model_name = '%s',model_desp='%s' Where model_name = '%s'",
				strTable,dlg.m_strName,dlg.m_strDesp,strOldName);
			try
			{
				theApp.m_pConLocalModel->Execute((const char*)strSQL,0,adCmdText); 
			}
			catch(_com_error e)
			{
				CString str;
				str.Format("更新表失败.SQL:%s",strSQL);
				AfxMessageBox(str);
				return;
			}
			//更新记录mode_runcontrol中的记录
			try
			{
				CStringArray strTableNameArr;
				strTableNameArr.Add("mode_runcontrol");
				strTableNameArr.Add("mode_runcontrol_cycle");
				strTableNameArr.Add("mode_runcontrol_day");
				strTableNameArr.Add("mode_runcontrol_duty");
				strTableNameArr.Add("mode_runcontrol_month");
				strTableNameArr.Add("mode_runcontrol_week");
				for(int i=0;i<strTableNameArr.GetSize();i++)
				{
					strSQL.Format("Update %s SET Mode_Name = '%s' Where Mode_Name = '%s'",
						strTableNameArr[i],dlg.m_strName,strOldName);
					theApp.m_pConLocalModel->Execute((const char*)strSQL,0,adCmdText); 
				}
			}
			catch(_com_error e)
			{
				CString str;
				str.Format("更新表失败.SQL:%s",strSQL);
				AfxMessageBox(str);
				return;
			}
			//更改所有表名称
			CString strTOldName,strTNewName;
			strTOldName.Format("%s_ModuleList",strOldName);
			strTNewName.Format("%s_ModuleList",dlg.m_strName);
			CDBOperator::RenameTable(theApp.m_pConLocalModel,strTOldName,strTNewName);
			for(int i=0;i<pDoc->m_pModel->GetModuleCount();i++)
			{
				CCalcModule *pModule=pDoc->m_pModel->GetModuleByIndex(i);
				strTOldName.Format("%s_%s_BlockList",strOldName,pModule->m_Name);
				strTNewName.Format("%s_%s_BlockList",dlg.m_strName,pModule->m_Name);
				CDBOperator::RenameTable(theApp.m_pConLocalModel,strTOldName,strTNewName);

				strTOldName.Format("%s_%s_BlockProp",strOldName,pModule->m_Name);
				strTNewName.Format("%s_%s_BlockProp",dlg.m_strName,pModule->m_Name);
				CDBOperator::RenameTable(theApp.m_pConLocalModel,strTOldName,strTNewName);
			}
			bRename=true;
		}
	}
	else if(pWS->m_iItemType==1)//模块重命名
	{
		CDlgMRename dlg;
		dlg.m_strWinText="重命名模块";
		CCalcModule *pModule=pDoc->m_pModel->FindModuleByName(pWS->m_strName);
		dlg.m_strName=pModule->m_Name;
		dlg.m_strDesp=pModule->m_Desp;
		strOldName=pModule->m_Name;
		strOldDesp=pModule->m_Desp;
		if(IDOK==dlg.DoModal())
		{
			pModule->SetModuleName(dlg.m_strName);
			pModule->SetModuleDesp(dlg.m_strDesp);
			//更新记录xxx_ModuleList中的记录
			strTable.Format("%s_ModuleList",pDoc->m_pModel->m_strModelName);
			strSQL.Format("Update %s SET module_name = '%s',module_desp='%s' Where module_name = '%s'",
				strTable,dlg.m_strName,dlg.m_strDesp,strOldName);
			try
			{
				theApp.m_pConLocalModel->Execute((const char*)strSQL,0,adCmdText); 
			}
			catch(_com_error e)
			{
				CString str;
				str.Format("更新表失败.SQL:%s",strSQL);
				AfxMessageBox(str);
				return;
			}
			//有关表名称所有表名称
			CString strTOldName,strTNewName;
			strTOldName.Format("%s_%s_BlockList",pDoc->m_pModel->m_strModelName,strOldName);
			strTNewName.Format("%s_%s_BlockList",pDoc->m_pModel->m_strModelName,dlg.m_strName);
			CDBOperator::RenameTable(theApp.m_pConLocalModel,strTOldName,strTNewName);
			
			strTOldName.Format("%s_%s_BlockProp",pDoc->m_pModel->m_strModelName,strOldName);
			strTNewName.Format("%s_%s_BlockProp",pDoc->m_pModel->m_strModelName,dlg.m_strName);
			CDBOperator::RenameTable(theApp.m_pConLocalModel,strTOldName,strTNewName);
			((CEPerfManageView*)(theApp.m_pView))->CloseUserTabByName(strOldName);
			bRename=true;
		}
	}
	//刷新列表树
	if(bRename)
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->m_LastShortCutID = -1;
		pMain->RefreshModelTree(pDoc->m_pModel);
	}
}
void  CClassViewModelBar::OnModuleProp()
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
//双击后的策略
//1.双击模型后，显示的是模型的属性：
// 模块名|描述|计算顺序
//2.双击模块后，显示：模块下定义的计算块列表，
// 计算块名|描述|类型   各个输入端属性：输入/输出变量名，中文描述；左侧是

void CClassViewModelBar::OnDblclkModelTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTreeItem = m_wndClassView.GetSelectedItem();
	CTreeItemObj* pWS = (CTreeItemObj*) m_wndClassView.GetItemData (hTreeItem);
	ASSERT_VALID (pWS);

	///////////////////////////////////////////////////////////////////////
	if(pWS->m_iItemType == 0) //如果为计算块被点击
	{
	}
	else if(pWS->m_iItemType==1) //如果模块被点击
	{
		//if(theApp.m_pTreeItemObj->m_strName != pWS->m_strName)
		{
			theApp.m_pTreeItemObj->m_strName = pWS->m_strName;
			theApp.m_pTreeItemObj->m_iItemType = pWS->m_iItemType;
			theApp.m_pTreeItemObj->m_strBlockType = pWS->m_strBlockType;
			theApp.m_pTreeItemObj->m_strBlockTypeDesp = pWS->m_strBlockTypeDesp;
			theApp.m_pTreeItemObj->m_bSysBlock = pWS->m_bSysBlock;
			theApp.m_pView->PostMessage(WM_TASKGUID,1);
		}
	}
	else if(pWS->m_iItemType==2) //如果模型被选中
	{
        CString str;
        str.Format("你选择了模型%s",pWS->m_strName );
		AfxMessageBox(str);
		//显示模型视图
		/////////////////////////////////////////////////////////////////////////////////////
        
		////////////////////////////////////////////////////////////////////////////////////
		/*
		//if(theApp.m_pTreeItemObj->m_strName != pWS->m_strName)
		{
			theApp.m_pTreeItemObj->m_strName = pWS->m_strName;
			theApp.m_pTreeItemObj->m_iItemType = pWS->m_iItemType;
			theApp.m_pTreeItemObj->m_strBlockType = pWS->m_strBlockType;
			theApp.m_pTreeItemObj->m_strBlockTypeDesp = pWS->m_strBlockTypeDesp;
			theApp.m_pTreeItemObj->m_bSysBlock = pWS->m_bSysBlock;

			if(theApp.m_pView->IsKindOf(RUNTIME_CLASS(CEPerfManageView)))
			{
				CEPerfManageView* pView = (CEPerfManageView*)theApp.m_pView;
				CString strModuleName = pWS->m_strName;
				pView->ShowCurrentModel();
			}
 			
		}*/
	
	}


	//////////////////////////////////////////////////////////////////////
	*pResult = 0;
}

