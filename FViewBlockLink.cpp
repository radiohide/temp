// FViewBlockLink.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "FViewBlockLink.h"

#include "FVModuleView.h"

#include "ePerfManageDoc.h"
#include "CalcModel.h"
#include "CalcModule.h"
#include "CalcBlock.H"
#include "CalcPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFViewBlockLink
const int nStartRow = 2;
const int BlockHeadRows = 2;

IMPLEMENT_DYNCREATE(CFViewBlockLink, CFormView)

CFViewBlockLink::CFViewBlockLink()
	: CFormView(CFViewBlockLink::IDD)
{
	//{{AFX_DATA_INIT(CFViewBlockLink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strSelModuleName.Empty();
	m_pSelBlock=NULL;
}

CFViewBlockLink::~CFViewBlockLink()
{
}

void CFViewBlockLink::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFViewBlockLink)
	DDX_Control(pDX, IDC_STATIC_LINKTOOLBAR, m_wndToolbarLocation);
	DDX_Control(pDX, IDC_STATIC_GRID, m_wndGridLocation);
	DDX_Control(pDX, IDC_COMBO_MODULE, m_combCtrl);
	DDX_Control(pDX, IDC_LIST_BLOCKS, m_lstBlocks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFViewBlockLink, CFormView)
	//{{AFX_MSG_MAP(CFViewBlockLink)
	ON_BN_CLICKED(IDC_BUTTON_AHEAD, OnButtonAhead)
	ON_BN_CLICKED(IDC_BUTTON_REAR, OnButtonRear)
	ON_CBN_SELCHANGE(IDC_COMBO_MODULE, OnSelchangeComboModule)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BLOCKS, OnDblclkListBlocks)
    ON_NOTIFY(NM_CLICK, IDC_LIST_BLOCKS, OnClickListBlocks)
	//ON_COMMAND(ID_FVLINK_LINK, OnBlockLink)
	//ON_UPDATE_COMMAND_UI(ID_FVLINK_LINK, OnUpdateBlockLink)
	//ON_COMMAND(ID_FVLINK_DISLINK, OnBlockDisLink)
	//ON_UPDATE_COMMAND_UI(ID_FVLINK_DISLINK, OnUpdateBlockDisLink)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_BLOCK1, OnGridDblClk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFViewBlockLink diagnostics

#ifdef _DEBUG
void CFViewBlockLink::AssertValid() const
{
	CFormView::AssertValid();
}

void CFViewBlockLink::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFViewBlockLink message handlers

void CFViewBlockLink::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	///////////////////////////
	// Create toolbar:
/*	m_wndToolBar.Create (this);
	m_wndToolBar.LoadToolBar (IDR_TOOLBAR_LINK, 0, 0, TRUE );

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	CSize sizeToolBar = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);

	// Set ToolBar position and size:
	CRect rectToolBar;
	m_wndToolbarLocation.GetWindowRect (&rectToolBar);
	ScreenToClient (&rectToolBar);

	m_wndToolBar.SetWindowPos (&wndTop, rectToolBar.left, rectToolBar.top, 
		sizeToolBar.cx, sizeToolBar.cy, SWP_NOACTIVATE);

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this dialog, not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);
*/
	/////////////////////////////
	m_BlockImages.Create (IDB_BLOCKS_PAGE, 32, 0, RGB (255, 0, 0));
	//m_BlockImages.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 0));
	
    m_lstBlocks.SetImageList(&m_BlockImages,LVSIL_NORMAL);

	//////////////////////////////
	CRect rectpos;
	m_wndGridLocation.GetClientRect (&rectpos);
	m_wndGridLocation.MapWindowPoints (this, &rectpos);
    m_GridCtrl.Create (rectpos, this, IDC_GRID_BLOCK1);
	m_GridCtrl.SetBkColor(RGB(255,255,255));
	m_GridCtrl.SetEditable(FALSE);
	m_GridCtrl.EnableDragAndDrop(FALSE);
	m_GridCtrl.SetFixedRowSelection(FALSE);
	m_GridCtrl.SetGridLines(FALSE);
	m_GridCtrl.SetListMode();	
	m_GridCtrl.SetSingleRowSelection(TRUE);
	m_GridCtrl.EnableSelection(FALSE);
	m_GridCtrl.SetDoubleBuffering();
	m_GridCtrl.SetHeaderSort(FALSE);

	m_GridCtrl.SetRowCount(10);
	m_GridCtrl.SetColumnCount(6);
	m_GridCtrl.SetFixedRowCount(0);
	m_GridCtrl.SetFixedColumnCount(0);
	m_GridCtrl.SetFixedColumnSelection(FALSE);
	m_GridCtrl.SetFixedRowSelection(TRUE);
	m_GridCtrl.SetDefCellHeight(15);
	m_GridCtrl.SetRowResize(FALSE);
	m_GridCtrl.SetColumnResize(FALSE);
	m_GridCtrl.ExpandLastColumn();

	m_GridCtrl.SetColumnWidth(0,100);
	m_GridCtrl.SetColumnWidth(1,20);
	m_GridCtrl.SetColumnWidth(2,100);
	m_GridCtrl.SetColumnWidth(3,30);
	m_GridCtrl.SetColumnWidth(4,100);
	m_GridCtrl.SetColumnWidth(5,20);

	/////////////////////////////

	RefreshModuleCombCtrl();
}


void CFViewBlockLink::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("CFViewBlockLink Update");
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		//Invalidate(FALSE);
		break;

	case HINT_UPDATE_MODULE:   // a single object has changed
		RefreshModuleCombCtrl();
		break;
	}
	
}

void CFViewBlockLink::OnButtonAhead() 
{
	// TODO: Add your control notification handler code here
	
}

void CFViewBlockLink::OnButtonRear() 
{
	// TODO: Add your control notification handler code here
	
}
//选择
void CFViewBlockLink::OnSelchangeComboModule() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_combCtrl.GetCurSel();
	if(nSel<0 ) {return;}

	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
    if(pDoc==NULL) return;

	CString str;
	m_combCtrl.GetWindowText(str);
	//解析出名称来
	int pos1 = str.Find("[");
	int pos2 = str.Find("]");
	int len = str.GetLength();
	m_strSelModuleName = str.Mid(pos1+1,pos2-pos1-1);

	CCalcModule* pModule = pDoc->m_pModel->FindModuleByName(m_strSelModuleName);
	if(pModule==NULL) return;
	//显示第1个元素
	m_lstBlocks.DeleteAllItems();
	POSITION pos = pModule->m_BlockList.GetHeadPosition(); 
	int nItem=0;
    while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)pModule->m_BlockList.GetNext(pos);
		str.Format("%s[%s]",pBlock->GetBlockDesp(),pBlock->GetBlockName());
		m_lstBlocks.InsertItem(nItem,str,1);
		nItem++;
	}

}

void CFViewBlockLink::RefreshModuleCombCtrl()
{
   CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
   if(pDoc==NULL) return;
   m_combCtrl.ResetContent();
   /////////////////////////////
   POSITION pos = pDoc->m_pModel->m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CString strTreeItem;
		//生成模块节点
		CCalcModule* pModule=  (CCalcModule*)pDoc->m_pModel->m_ModuleList.GetNext(pos);
		strTreeItem.Format("%s[%s]",pModule->GetModuleDesp(),pModule->GetModuleName());
		m_combCtrl.AddString(strTreeItem);
	}
   

}

/*void CFViewBlockLink::OnItemchangedListBlocks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	int nSelected = pListView->iItem;
	if(nSelected>=0)
	{
		CString str;
		str.Format("%d",nSelected);
		AfxMessageBox(str);
	}
	
	*pResult = 0;
}*/

void CFViewBlockLink::OnDblclkListBlocks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	int nSelected = pListView->iItem;
	if(nSelected<0) return;

  	if(nSelected>=0)
	{
		CString str;
		str.Format("%d",nSelected);
		AfxMessageBox(str);
	}

	*pResult = 0;
}

void CFViewBlockLink::OnClickListBlocks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	int nSelected = pListView->iItem;
	if(nSelected<0) return;

    CString str = m_lstBlocks.GetItemText(nSelected,0);
	//解析出名称来
	int pos1 = str.Find("[");
	int pos2 = str.Find("]");
	int len = str.GetLength();
	CString strBlockName = str.Mid(pos1+1,pos2-pos1-1);
    
	UpdateBlockProp(strBlockName);

	*pResult = 0;
}

void CFViewBlockLink::UpdateBlockProp(CString strBlockName)
{
    CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
    if(pDoc==NULL) return;
	if(strBlockName.IsEmpty()) return;
	//显示模块的类别
	int nItem=0;
	CString strItem;
	CCalcModule* pModule = pDoc->m_pModel->FindModuleByName(m_strSelModuleName);
	if(pModule==NULL) return;
	CCalcBlock* pBlock = pModule->FindBlockByName(strBlockName);
	if(pBlock==NULL) return;
	m_pSelBlock = pBlock;
	//根据pBlock进行属性页的更新
	//ShowInOutPage(pBlock);
	m_GridCtrl.ShowBlock(pBlock);
}

void CFViewBlockLink::ShowInOutPage(CCalcBlock *pBlock)
{
	int inNums = pBlock->GetInputNum();
	int OutNums = pBlock->GetOutputNum();
	int NodeNums =  inNums <= OutNums ? OutNums:inNums;

	int RowCount = NodeNums*4 +(nStartRow + BlockHeadRows);
	int ColCount = 6;
	/////////////////////////////////////
	m_GridCtrl.DeleteAllItems();
	m_GridCtrl.SetRowCount(RowCount);
	m_GridCtrl.SetColumnCount(ColCount);
	m_GridCtrl.SetFixedRowCount(0);
	m_GridCtrl.SetFixedColumnCount(0);
	m_GridCtrl.SetColumnWidth(0,100);
	m_GridCtrl.SetColumnWidth(1,20);
	m_GridCtrl.SetColumnWidth(2,100);
	m_GridCtrl.SetColumnWidth(3,30);
	m_GridCtrl.SetColumnWidth(4,100);
	m_GridCtrl.SetColumnWidth(5,20);

	//for header
	int row=0;
	int col=2;
	for(row=0;row<RowCount;row++)
	{
		COLORREF bkColor;
		COLORREF ftColor;
		if(row<nStartRow)
		{
			continue;
		}
		else if(row<nStartRow+BlockHeadRows)
		{
			bkColor = BLOCK_TITLE_BKCOLOR;
			ftColor = BLOCK_TITLE_FTCOLOR;
		}
		else
		{
			bkColor = BLOCK_BODY_BKCOLOR;
			ftColor = BLOCK_BODY_FTCOLOR;
		}
		for(col=2;col<5;col++)
		{
			m_GridCtrl.SetItemBkColour(row,col,bkColor);
			m_GridCtrl.SetItemFgColour(row,col,ftColor);
		}
	}
	m_GridCtrl.SetItemText(nStartRow+0,2,pBlock->GetBlockName());
	m_GridCtrl.SetItemText(nStartRow+1,2,pBlock->GetBlockDesp());
	///////////////////////////////////////////////////
	//for input
	int i=0;
	int j=0;
	for(i=0;i<inNums;i++)
	{
		CCalcPort* pPort = pBlock->GetInputPortObj(i);
        row = i*4+(nStartRow + BlockHeadRows); //该端口的行位置开始
		m_GridCtrl.SetItemText(row,0,pPort->GetPortLinkID());//from block

		CString strLink = pPort->GetPortLinkID();
		if(strLink.IsEmpty())
		{
		    //m_GridCtrl.SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[0]);//设置输入的端口颜色
		}
		else
		{
			m_GridCtrl.SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[1]);//设置输入的端口颜色
		}

		m_GridCtrl.SetItemText(row,  2,pPort->GetPortDesp());
		m_GridCtrl.SetItemText(row+1,2,pPort->GetPortID());
		
		m_GridCtrl.SetItemFgColour(row+1,1,BLOCK_INPUT_FTCOLOR);
	}
	//for output 
	for(i=0;i<OutNums;i++)
	{
		CCalcPort* pPort = pBlock->GetOutputPortObj(i);
        row = i*4+(nStartRow + BlockHeadRows); //该端口的行位置开始
		m_GridCtrl.SetItemText(row,  4,pPort->GetPortDesp()); //desp
		m_GridCtrl.SetItemText(row+1,4,pPort->GetPortID());
		m_GridCtrl.SetItemBkColour(row+1,5,BLOCK_OUPUT_BKCOLOR[1]);//设置输出的端口颜色
		m_GridCtrl.SetItemFgColour(row+1,5,BLOCK_INPUT_FTCOLOR);
	}
}



/*
void CFViewBlockLink::OnBlockLink()
{
	//AfxMessageBox("link");

}
void CFViewBlockLink::OnBlockDisLink()
{
	//AfxMessageBox("dislink");
}

void CFViewBlockLink::OnUpdateBlockLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
void CFViewBlockLink::OnUpdateBlockDisLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}*/
void CFViewBlockLink::OnGridDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW* )pNMHDR;

	if(m_pSelBlock==NULL) return;

    int iRow = pItem->iRow ;
	int iCol = pItem->iColumn;

	//检查是否是输入端口
	int nSelInputID=-1;
	int nSelOutputID=-1;
	if(iCol==1) //看是否双击输入端口
	{
		int inNums = m_pSelBlock->GetInputNum();
		int maxRow = inNums*4 +(nStartRow + BlockHeadRows);
		
		if(iRow >= nStartRow + BlockHeadRows && iRow< maxRow)
		{
			int RowReal = iRow - (nStartRow + BlockHeadRows);
			int inIndex = RowReal % 4;
			if(inIndex==1)
			{
				//this is a 
				nSelInputID = RowReal/4;
			}
		}

	}
	//看哪个输入端口
	else if(iCol==5) //看是否双击输入端口
	{
		int Nums = m_pSelBlock->GetOutputNum();
		int maxRow = Nums*4 +(nStartRow + BlockHeadRows);
		
		if(iRow >= nStartRow + BlockHeadRows && iRow< maxRow)
		{
			int RowReal = iRow - (nStartRow + BlockHeadRows);
			int inIndex = RowReal % 4;
			if(inIndex==1)
			{
				//this is a 
				nSelOutputID = RowReal/4;
				//AfxMessageBox("out is sel");
			
			}
		}

	}

}

CString CFViewBlockLink::GetSelBlockOutputLinkID()
{
	CString strRet=_T("");
	
	if(m_pSelBlock==NULL) return strRet;
   
    CCellID SelCellID= m_GridCtrl.GetFocusCell();
	int iRow = SelCellID.row;
	int iCol =SelCellID.col;

	int nSelOutputID=-1;
	//看哪个输出端口
	if(iCol==5) //看是否双击输入端口
	{
		int Nums = m_pSelBlock->GetOutputNum();
		int maxRow = Nums*4 +(nStartRow + BlockHeadRows);
		
		if(iRow >= nStartRow + BlockHeadRows && iRow< maxRow)
		{
			int RowReal = iRow - (nStartRow + BlockHeadRows);
			int inIndex = RowReal % 4;
			if(inIndex==1)
			{
				nSelOutputID = RowReal/4;
			}
		}
		if(nSelOutputID!=-1) //
		{
			strRet = m_pSelBlock->GetOutLinkID(nSelOutputID);
		}


	}


	return strRet;
}

void CFViewBlockLink::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	m_GridCtrl.DestroyWindow();
}
