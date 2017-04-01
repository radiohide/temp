// ModuleLibTree.cpp : implementation file
//

#include "stdafx.h"
#include "ModuleLibTree.h"
#include "TreeItemObj.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleLibTree

CModuleLibTree::CModuleLibTree()
{
}

CModuleLibTree::~CModuleLibTree()
{
	while (!m_lstWorkspaces.IsEmpty ())
	{
		delete m_lstWorkspaces.RemoveHead ();
	}
}


BEGIN_MESSAGE_MAP(CModuleLibTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CModuleLibTree)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleLibTree message handlers

int CModuleLibTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
   m_menu.LoadMenu(IDR_POPUP_LIBBLOCK);
	return 0;
}

BOOL CModuleLibTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
		cs.style |= WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
				TVS_HASLINES | TVS_HASBUTTONS | 
				TVS_DISABLEDRAGDROP | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeCtrl::PreCreateWindow(cs);
}
//加载
void CModuleLibTree::LoadLibBlocks(_ConnectionPtr &pCon)
{
    ///////////////////////////////////////////////////////
    //添加系统计算块
	//1.首先获得系统级的类别
    CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("sysblockClass");
	strSQL.Format("SELECT * FROM %s ORDER BY id ",strTable);
    try	{
		 pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
    CStringArray strClasses;
	while(!pRecord->adoEOF)
	{
		CString strItem; //块
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("class");
			if(Tempvalue.vt != VT_NULL) strItem = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) strClasses.Add(strItem);
		pRecord->MoveNext();
	}
	pRecord->Close();
    //根据类型检索并系统计算块生成子节点
	//CString strName,int ItemType,CString BlockType,CString BlockTypeDesp,bool bSystBlock);
	CTreeItemObj* pWS = NULL;
	CString strTreeItem=_T("系统计算块");
	HTREEITEM hRoot = InsertItem (strTreeItem,0,0);
	SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
	
	pWS = new CTreeItemObj(strTreeItem,2,"","",false);
    m_lstWorkspaces.AddTail(pWS);
	SetItemData (hRoot, (DWORD) pWS);

	strTable=_T("sysblockList");
	CString strField = _T("class");
	int Count = strClasses.GetSize();
	for(int i=0;i<Count;i++)
	{
		//生成类别节点
		strTreeItem = strClasses.GetAt(i);
        HTREEITEM hClass =InsertItem (strTreeItem,1,1, hRoot);
		pWS = new CTreeItemObj(strTreeItem,1,"","",false);
        m_lstWorkspaces.AddTail(pWS);
	    SetItemData (hClass, (DWORD) pWS);
		//////////////////////////////////////////////////////////
		strSQL.Format("SELECT * FROM %s WHERE %s = '%s'",strTable,strField,strClasses.GetAt(i));
		try	
		{
			 pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		}
   		catch(_com_error e)
		{
			continue ;
		}	
		//添加该类的各个子计算块
		if(!pRecord->GetBOF())  pRecord->MoveFirst();
		while(!pRecord->adoEOF)
		{
			CString strType; //块
			CString strTypeDesp;
			BOOL bValid =TRUE;
			try
			{
				Tempvalue = pRecord->GetCollect("blocktype");
				if(Tempvalue.vt != VT_NULL) strType = (LPCSTR)_bstr_t(Tempvalue);
				
				Tempvalue = pRecord->GetCollect("desp");
				if(Tempvalue.vt != VT_NULL) strTypeDesp = (LPCSTR)_bstr_t(Tempvalue);
			}
			catch(_com_error e) {bValid=FALSE;}

			if(bValid) 
			{
				HTREEITEM hItem = InsertItem (strTypeDesp, 3, 3, hClass);
				pWS = new CTreeItemObj(strTypeDesp,0,strType,strTypeDesp,false);
				m_lstWorkspaces.AddTail(pWS);
				SetItemData (hItem, (DWORD) pWS);
			}
			pRecord->MoveNext();
		}
		pRecord->Close();
	}
    Expand (hRoot, TVE_EXPAND);
	////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////
    //添加自定义计算块

	//////////////////////////////////////////////////////
	/*
 	HTREEITEM hRoot1 = InsertItem (_T("系统计算块"),0,0);
	SetItemState (hRoot1, TVIS_BOLD, TVIS_BOLD);
	//循环读取类别号，根据类别号，载入该类的各个计算块

	HTREEITEM hClass1 =InsertItem (_T("数据源"),1,1, hRoot1);
	InsertItem (_T("测点源"), 3, 3, hClass1);
	InsertItem (_T("常值源"), 3, 3, hClass1);

	HTREEITEM hClass2 =InsertItem (_T("通用计算公式"),1,1,hRoot1);
	InsertItem (_T("公式1"), 3, 3, hClass2);
	InsertItem (_T("公式2"), 3, 3, hClass2);

	Expand (hRoot1, TVE_EXPAND);


   	HTREEITEM hRoot = InsertItem (_T("自定义计算块"), 0, 0);
	SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = InsertItem (_T("CAboutDlg"), 1, 1, hRoot);
	InsertItem (_T("CAboutDlg()"), 5, 5, hClass);
	InsertItem (_T("DoDataExchange(CDataExchange* pDX)"), 5, 5, hClass);

	Expand (hRoot, TVE_EXPAND);

	hClass = InsertItem (_T("CHelloBCGApp"), 1, 1, hRoot);
	InsertItem (_T("CHelloBCGApp()"), 3, 3, hClass);
	InsertItem (_T("InitInstance()"), 3, 3, hClass);
	InsertItem (_T("OnAppAbout()"), 3, 3, hClass);

	hClass = InsertItem (_T("CHelloBCGDoc"), 1, 1, hRoot);
	InsertItem (_T("AssertValid()"), 3, 3, hClass);
	InsertItem (_T("CHelloBCGDoc()"), 4, 4, hClass);
	InsertItem (_T("~CHelloBCGDoc()"), 3, 3, hClass);
	InsertItem (_T("Dump(CDumpContext& dc)"), 3, 3, hClass);
	InsertItem (_T("OnNewDocument()"), 3, 3, hClass);
	InsertItem (_T("Serialize()"), 3, 3, hClass);

	hClass = InsertItem (_T("CHelloBCGView"), 1, 1, hRoot);
	InsertItem (_T("AssertValid()"), 3, 3, hClass);
	InsertItem (_T("CHelloBCGView()"), 4, 4, hClass);
	InsertItem (_T("~CHelloBCGView()"), 3, 3, hClass);
	InsertItem (_T("Dump(CDumpContext& dc)"), 3, 3, hClass);
	InsertItem (_T("GetDocument()"), 3, 3, hClass);
	InsertItem (_T("OnDraw(CDC* pDC)"), 3, 3, hClass);
	Expand (hClass, TVE_EXPAND);

	hClass = InsertItem (_T("CMainFrame"), 1, 1, hRoot);
	InsertItem (_T("AssertValid()"), 3, 3, hClass);
	InsertItem (_T("CMainFrame()"), 3, 3, hClass);
	InsertItem (_T("~CMainFrame()"), 3, 3, hClass);
	InsertItem (_T("Dump(CDumpContext& dc)"), 3, 3, hClass);
	InsertItem (_T("OnCreate(LPCREATESTRUCT lpCreateStruct)"), 4, 4, hClass);
	InsertItem (_T("PreCreateWindow(CREATESTRUCT& cs)"), 3, 3, hClass);
	InsertItem (_T("m_wndMenuBar"), 6, 6, hClass);
	InsertItem (_T("m_wndToolBar"), 6, 6, hClass);
	InsertItem (_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = InsertItem (_T("Globals"), 2, 2, hRoot);
	InsertItem (_T("theApp"), 5, 5, hClass);
	//Expand (hClass, TVE_EXPAND);*/
}

void CModuleLibTree::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (point != CPoint (-1, -1))
	{
		//---------------------
		// Select clicked item:
		//---------------------
		CPoint ptTree = point;
		ScreenToClient (&ptTree);

		HTREEITEM hTreeItem = HitTest (ptTree);
		if (hTreeItem != NULL)
		{
			SelectItem (hTreeItem);

		
		}
	}
		POINT curPos;
			GetCursorPos(&curPos);
			CMenu* pSumMenu = m_menu.GetSubMenu(0);
			pSumMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,curPos.x,curPos.y,this);

/*
	
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_LIBBLOCK);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPFrameWnd)))
	{
	   CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	   
	   if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
		   return;
	   
	   ((CBCGPMDIFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
	   UpdateDialogControls(this, FALSE);
	}*/	
}
