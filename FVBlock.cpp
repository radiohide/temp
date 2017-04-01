// FVBlock.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "ePerfManageDoc.h"
#include "FVBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFVBlock

IMPLEMENT_DYNCREATE(CFVBlock, CFormView)

CFVBlock::CFVBlock()
	: CFormView(CFVBlock::IDD)
{
	//{{AFX_DATA_INIT(CFVBlock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strModuleName = _T("");
	m_strBlockName = _T("");
}

CFVBlock::~CFVBlock()
{
}

void CFVBlock::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFVBlock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFVBlock, CFormView)
	//{{AFX_MSG_MAP(CFVBlock)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFVBlock diagnostics

#ifdef _DEBUG
void CFVBlock::AssertValid() const
{
	CFormView::AssertValid();
}

void CFVBlock::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFVBlock message handlers

void CFVBlock::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CRect rectpos;
	GetClientRect(rectpos);
	//m_wndGridLocation.GetClientRect (&rectpos);
	//m_wndGridLocation.MapWindowPoints (this, &rectpos);
    m_GridCtrl.Create (rectpos, this, IDC_GRID_BLOCK);
	m_GridCtrl.SetBkColor(RGB(125,125,125));
	m_GridCtrl.SetEditable(FALSE);
	m_GridCtrl.EnableDragAndDrop(FALSE);
	m_GridCtrl.SetFixedRowSelection(FALSE);
	m_GridCtrl.SetGridLines(TRUE);
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

//	UpdateBlock();
	
}
//更新显示本模块，以及相关联的模块，输入、输出
//根据计算块名称，可找到输入端所连接的
void CFVBlock::UpdateBlock()
{
	if(m_strBlockName.IsEmpty()) return;
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
    if(pDoc==NULL) return;
	//找到该计算块
	CCalcBlock* pBlock = pDoc->m_pModel->FindBlockByName(m_strModuleName,m_strBlockName);
	if(pBlock==NULL) return;
    //找到输入的对象，逐个进行显示
	int inNums = pBlock->GetInputNum();
	int OutNums = pBlock->GetOutputNum();
	int Nums =  inNums <= OutNums ? OutNums:inNums; //获得最大的输入输出个数
	//计算每个相连对象的位置，首先看输入----------------------------------------------------------
	int i=0;
	for( i=0;i<inNums;i++)
	{
		CCalcPort* pPort = pBlock->GetInputPortObj(i);
		CString strLink = pPort->GetPortLinkID(); //端口的linkID的格式：“BlockName.PortNameID”
		//得到

	}
	//计算输出的情况
	for(i=0;i<OutNums;i++)
	{
		CCalcPort* pPort = pBlock->GetOutputPortObj(i);
		CString strPortID = pPort->GetPortID();
		CString strPortDesp = pPort->GetPortDesp();
	}

}

void CFVBlock::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
