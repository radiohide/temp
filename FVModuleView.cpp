// FVModuleView.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "FVModuleView.h"

#include "ePerfManageDoc.h"
#include "MainFrm.h"
#include "FViewBlockLink.h"

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
// CFVModuleView
const int nStartRow = 2;
const int BlockHeadRows = 2;

IMPLEMENT_DYNCREATE(CFVModuleView, CFormView)
//////////////////////////////////////////////////////////////////////
COLORREF CMyBlockListCtrl::OnGetCellTextColor (int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CBCGPListCtrl::OnGetCellTextColor (nRow, nColum);
	}

	return (nRow % 2) == 0 ? RGB (128, 37, 0) : RGB (0, 0, 0);
}

COLORREF CMyBlockListCtrl::OnGetCellBkColor (int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CBCGPListCtrl::OnGetCellBkColor (nRow, nColum);
	}

	if (m_bMarkSortedColumn && nColum == m_iSortedColumn)
	{
		return (nRow % 2) == 0 ? RGB (233, 221, 229) : RGB (176, 218, 234);
	}

	return (nRow % 2) == 0 ? RGB (253, 241, 249) : RGB (196, 238, 254);
}

HFONT CMyBlockListCtrl::OnGetCellFont (int nRow, int nColum, DWORD dwData/* = 0*/)
{
	if (!m_bModifyFont)
	{
		return NULL;
	}

	if (nColum == 2 && (nRow >= 4 && nRow <= 8))
	{
		return globalData.fontBold;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
CFVModuleView::CFVModuleView()
	: CFormView(CFVModuleView::IDD)
{
	//{{AFX_DATA_INIT(CFVModuleView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strModuleName = _T("");
	m_pSelBlock = NULL;
}

CFVModuleView::~CFVModuleView()
{
}

void CFVModuleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFVModuleView)
//	DDX_Control(pDX, IDC_GRID_LOCATION, m_wndGridLocation);
	DDX_Control(pDX, IDC_LIST_BLOCK, m_lstBlocks);
	//DDX_Control(pDX, IDC_PROPLIST_LOCATION, m_wndPropListLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFVModuleView, CFormView)
	//{{AFX_MSG_MAP(CFVModuleView)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BLOCK, OnClickListBlock)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFVModuleView diagnostics
////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CFVModuleView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFVModuleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEPerfManageDoc* CFVModuleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEPerfManageDoc)));
	return (CEPerfManageDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFVModuleView message handlers

void CFVModuleView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	//计算块的列表初始化
	m_BlockImages.Create (IDB_BLOCKS_PAGE, 32, 0, RGB (255, 0, 0));
	//m_BlockImages.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 0));
	
    m_lstBlocks.SetImageList(&m_BlockImages,LVSIL_NORMAL);

	//m_lstBlocks.InsertColumn (0, _T("序号"), LVCFMT_LEFT, 80);
	//m_lstBlocks.InsertColumn (1, _T("块名"), LVCFMT_LEFT, 100);
	//m_lstBlocks.InsertColumn (2, _T("块描述"), LVCFMT_LEFT, 300);
	//m_lstBlocks.InsertColumn (3, _T("块类型"), LVCFMT_LEFT, 300);
	//m_lstBlocks.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
	//					LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//对属性列表的初始化
	CRect rectpos;
	//m_wndPropListLocation.GetClientRect (&rectpos);
	//m_wndPropListLocation.MapWindowPoints (this, &rectpos);
    //m_wndPropList.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectpos, this, (UINT)-1);
	
	//m_wndPropList.EnableHeaderCtrl ();
	//m_wndPropList.EnableDesciptionArea ();

	///////////////////////////////////////
	/*m_wndGridLocation.GetClientRect (&rectpos);
	m_wndGridLocation.MapWindowPoints (this, &rectpos);
    m_GridCtrl.Create (rectpos, this, IDC_GRID_BLOCK);
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
*/
	///////////////////////////////////////
	UpdateBlockList();
	
}
//该函数是获得
void CFVModuleView::UpdateBlockList()
{
	if(m_strModuleName.IsEmpty()) m_lstBlocks.DeleteAllItems();

	else
	{
		//找到模块对象并且显示
		CEPerfManageDoc* pDoc = GetDocument();
        //显示模块的类别
		m_lstBlocks.DeleteAllItems();
		int nItem=0;
		CString strItem;
		CCalcModule* pModule = pDoc->m_pModel->FindModuleByName(m_strModuleName);
		if(pModule==NULL) return;
		POSITION pos = pModule->m_BlockList.GetHeadPosition();
		while(pos)
		{
			//生成模块节点
			CCalcBlock* pBlock=  (CCalcBlock*)pModule->m_BlockList.GetNext(pos);
			//strItem.Format("%d",nItem+1);
			//m_lstBlocks.InsertItem(nItem,strItem);

			//m_lstBlocks.SetItemText (nItem, 1, pBlock->GetBlockName());
			//m_lstBlocks.SetItemText (nItem, 2, pBlock->GetBlockDesp());
			//m_lstBlocks.SetItemText (nItem, 3, pBlock->GetBlockType());
			strItem.Format("%s[%s]",pBlock->GetBlockDesp(),pBlock->GetBlockName());
		    m_lstBlocks.InsertItem(nItem,strItem,1);
			nItem++;

		}
	}
}

void CFVModuleView::OnClickListBlock(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	int nSelected = pListView->iItem;
	if(nSelected<0) return;

    CString str = m_lstBlocks.GetItemText(nSelected,0);
	//解析出名称来
	int pos1 = str.Find("[");
	int pos2 = str.Find("]");
	int len = str.GetLength();
	CString strBlockName = str.Mid(pos1+1,pos2-pos1-1);
    
	UpdateBlockProp(strBlockName);

    */
	*pResult = 0;
}

void CFVModuleView::UpdateBlockProp(CString strBlockName)
{
	/*
	CEPerfManageDoc* pDoc = GetDocument();
	if(strBlockName.IsEmpty()) return;
	//显示模块的类别
	int nItem=0;
	CString strItem;
	CCalcModule* pModule = pDoc->m_pModel->FindModuleByName(m_strModuleName);
	if(pModule==NULL) return;
	CCalcBlock* pBlock = pModule->FindBlockByName(strBlockName);
	if(pBlock==NULL) return;
	m_pSelBlock = pBlock;
	//根据pBlock进行属性页的更新
    //pBlock->ShowPropPage(&m_wndPropList);
	//ShowInOutPage(pBlock);
	m_GridCtrl.ShowBlock(pBlock);
	*/
}

void CFVModuleView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//if(m_lstBlocks.m_hWnd) m_lstBlocks.RedrawWindow ();
	//if(m_wndPropList.m_hWnd) m_wndPropList.RedrawWindow ();
	// Do not call CFormView::OnPaint() for painting messages
	//TRACE("ON paint\n");
}

void CFVModuleView::ShowInOutPage(CCalcBlock *pBlock)
{
	/*
	int inNums = pBlock->GetInputNum();
	int OutNums = pBlock->GetOutputNum();
	int NodeNums =  inNums <= OutNums ? OutNums:inNums;

	//int nStartRow = 2;
	//int BlockHeadRows = 2;
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

		m_GridCtrl.SetItemText(row,  2,pPort->GetPortDesp());
		m_GridCtrl.SetItemText(row+1,2,pPort->GetPortID());
		CString strLink = pPort->GetPortLinkID();
		if(strLink.IsEmpty())
		{
		    m_GridCtrl.SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[0]);//设置输入的端口颜色
		}
		else
		{
			m_GridCtrl.SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[1]);//设置输入的端口颜色
		}
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

*/

}
//如何获得其他view
/*void CFVModuleView::OnGridDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW* )pNMHDR;

	if(m_pSelBlock==NULL) return;

	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
    if(pDoc==NULL) return;

	int iRow = pItem->iRow ;
	int iCol = pItem->iColumn;

	//获得是哪个被选中，哪个Block,输出端是哪个
	int nSelInputID=-1;
	int nSelOutputID=-1;
	CString strLinkID=_T("");
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
				//this is a INPUT NODE
				nSelInputID = RowReal/4;
				//pView->GetSelectedBlockInfo(CCalcBlock* pSrcBlock,);
				CCalcPort* pPort = m_pSelBlock->GetInputPortObj(nSelInputID);
				if(pPort->GetPortLinkID().IsEmpty())
				{
				
					CFViewBlockLink* pView = pDoc->GetBlockLinkView();
					if(pView==NULL) return;
					CCalcBlock* pSrcBlock=pView->m_pSelBlock;
					strLinkID = pView->GetSelBlockOutputLinkID();
					if(strLinkID.IsEmpty()==FALSE && pSrcBlock!=NULL)
					{
						if(m_pSelBlock->GetBlockName()!=pSrcBlock->GetBlockName())
						{
							m_pSelBlock->SetInputSourceID(nSelInputID,strLinkID);
							//AfxMessageBox("set the link");
							ShowInOutPage(m_pSelBlock);
						}
						else
						{
							AfxMessageBox("同一模块的输入输出不可连接！");
						}
					}
				}
				else//取消连接
				{
					strLinkID.Empty();
					m_pSelBlock->SetInputSourceID(nSelInputID,strLinkID);
					//AfxMessageBox("cancle the link");
					ShowInOutPage(m_pSelBlock);
				}
			}
		}

	}

	
}
*/
void CFVModuleView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	//m_GridCtrl.DestroyWindow();
}

void CFVModuleView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		//Invalidate(FALSE);
		break;

	case HINT_UPDATE_MODULE:   // a single object has changed
		UpdateBlockList();
		break;
	}
	
}
LRESULT CFVModuleView::OnPropertyChanged(WPARAM wpara,LPARAM lpara)
{

	return 0;

}

void CFVModuleView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
