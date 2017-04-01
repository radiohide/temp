// ViewDebugFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "ViewDebugFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDebugFrm

IMPLEMENT_DYNCREATE(CViewDebugFrm, CFormView)

CViewDebugFrm::CViewDebugFrm()
	: CFormView(CViewDebugFrm::IDD)
{
	//{{AFX_DATA_INIT(CViewDebugFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lFocusID =-1;
	m_pModel = NULL;
}

CViewDebugFrm::~CViewDebugFrm()
{
}

void CViewDebugFrm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDebugFrm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDebugFrm, CFormView)
	//{{AFX_MSG_MAP(CViewDebugFrm)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_DEBUG, OnKeydownList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEBUG, OnClickListDebug)
	ON_COMMAND(ID_DEBUG_SINGLESTEP, OnDebugSingleStep)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_SINGLESTEP, OnUpdateDebugSingleStep)
	ON_COMMAND(ID_DEBUG_GO, OnDebugGo)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_GO, OnUpdateDebugGo)
	ON_COMMAND(ID_DEBUG_GOTOLINE, OnDebugGotoLine)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_GOTOLINE, OnUpdateDebugGotoLine)
	ON_COMMAND(ID_DEBUG_CANCEL, OnDebugCancel)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_CANCEL, OnUpdateDebugCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDebugFrm diagnostics

#ifdef _DEBUG
void CViewDebugFrm::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewDebugFrm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDebugFrm message handlers
void CViewDebugFrm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	m_Font.CreateStockObject (DEFAULT_GUI_FONT);
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT|LVS_SHOWSELALWAYS;

	if (!m_wndList.Create(dwStyle, rectDummy, this,IDC_LIST_DEBUG) )
	{
		TRACE0("Failed to create LIST view\n");
		return ;      // fail to create
	}
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
						LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	GetClientRect(rectDummy);
	m_wndList.MoveWindow(rectDummy);

	m_wndList.InsertColumn (0, _T("标识"), LVCFMT_LEFT, 70);
	m_wndList.InsertColumn (1, _T("状态"), LVCFMT_LEFT, 70);
	m_wndList.InsertColumn (2, _T("块名"), LVCFMT_LEFT, 120);
	m_wndList.InsertColumn (3, _T("块描述"), LVCFMT_LEFT, 150);
	m_wndList.InsertColumn (4, _T("所属模块"), LVCFMT_LEFT, 150);

	m_wndList.SetFont (&m_Font);	
}

void CViewDebugFrm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_wndList.m_hWnd )
		m_wndList.MoveWindow(0,0,cx,cy);	
	
}
void CViewDebugFrm::LoadModel(CCalcModel* pModel)
{
	m_pModel = pModel;
	/////////////////////////////////////////////////////////////////////////
	m_wndList.DeleteAllItems();
	//填充到列表中
	CString strItem;
	int nItem =0;
	POSITION pos = pModel->m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)pModel->m_ModuleList.GetNext(pos);
		POSITION pos1 = pModule->m_BlockList.GetHeadPosition();
		while(pos1)
		{
			CCalcBlock* pBlock =  (CCalcBlock*)pModule->m_BlockList.GetNext(pos1);
			m_wndList.InsertItem(nItem,_T(""));

            strItem.Format("%d",nItem+1);
			m_wndList.SetItemText (nItem, 1, strItem);
			m_wndList.SetItemText (nItem, 2, pBlock->GetBlockName());
			m_wndList.SetItemText (nItem, 3, pBlock->GetBlockDesp());
			m_wndList.SetItemText (nItem, 4, pModule->GetModuleName());
			nItem++;
		}
	}
	m_lFocusID = 0;
	m_wndList.SetItemText(m_lFocusID,0,_T("-->"));
	m_wndList.SetItemState(m_lFocusID,   LVIS_SELECTED|LVIS_FOCUSED,   LVIS_SELECTED|LVIS_FOCUSED);
    //让观察窗口自动显示
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->ActiveOutPutPage(1);

}

//调试的时候，模型的总体控制信息如何处理？
void CViewDebugFrm::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pLVKeyDow-> wVKey == VK_RIGHT)
	{
		debugSingleStep();
	}
	*pResult = 0;
}
void CViewDebugFrm::OnClickListDebug(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
	{
		nSelectedItem = m_wndList.GetNextSelectedItem(pos);
	}
	if(nSelectedItem<0)
	{ 
		return;
	}
	if(m_pModel!=NULL)
	{
		CString strBlockName = m_wndList.GetItemText(nSelectedItem,2);
		CString strModule = m_wndList.GetItemText(nSelectedItem,4);
		CCalcBlock* pBlock = m_pModel->FindBlockByName(strModule,strBlockName);
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->ShowOutputWatch(pBlock);
	}
	*pResult = 0;
}


void CViewDebugFrm::SetFocusListCtrl()
{
	m_wndList.SetFocus();
}

void CViewDebugFrm::OnDebugSingleStep() 
{
	// TODO: Add your command handler code here
	debugSingleStep();
}

void CViewDebugFrm::OnUpdateDebugSingleStep(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CViewDebugFrm::OnDebugGo() 
{
	// TODO: Add your command handler code here
	if(m_pModel!=NULL)
	{
		for(int i=0;i<m_wndList.GetItemCount();i++)
		{
			if(m_lFocusID<m_wndList.GetItemCount() && m_lFocusID>-1)
			{
				debugSingleStep();
			}
		}
	}
}

void CViewDebugFrm::OnUpdateDebugGo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewDebugFrm::OnDebugGotoLine() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_wndList.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("请选择要执行到哪一行！");
		 return;
	}
	if(m_pModel!=NULL)
	{
		for(int i=0;i<m_wndList.GetItemCount();i++)
		{
			if(m_lFocusID<=nSelectedItem)
			{
				if(m_lFocusID<m_wndList.GetItemCount() && m_lFocusID>-1)
				{
					debugSingleStep();
				}
			}
			else//执行到选中行
				break;
		}
	}
	
}

void CViewDebugFrm::OnUpdateDebugGotoLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
void CViewDebugFrm::debugSingleStep()
{
	if(m_pModel!=NULL)
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		if(m_lFocusID<m_wndList.GetItemCount() && m_lFocusID>-1)
		{
			//运行第几个对象
			m_wndList.SetItemText(m_lFocusID,0,_T("@"));
			m_wndList.SetItemState(m_lFocusID,   0,   LVIS_SELECTED);
			//执行第m_lFocusID个块
			//////////////////////////////////////////////////////////////////////
			CString strBlockName = m_wndList.GetItemText(m_lFocusID,2);
			CString strModule = m_wndList.GetItemText(m_lFocusID,4);
			//CString str;
			//str.Format("执行M:(%s)B(%s)",strBlockName,strModule);
			//AfxMessageBox(str);
			CCalcBlock* pBlock = m_pModel->FindBlockByName(strModule,strBlockName);
			if(pBlock)
			{
				pBlock->Calc();
			}
			//将当前的计算块的信息均显示在，显示的信息包括
			//输入1：数据值
			//输入2：
			//输出1：数据
			pMain->ShowOutputWatch(pBlock);
			//////////////////////////////////////////////////////////////////////
			m_lFocusID++;
			m_wndList.SetItemText(m_lFocusID,0,_T("-->>"));
			m_wndList.SetItemState(m_lFocusID,   LVIS_SELECTED|LVIS_FOCUSED,   LVIS_SELECTED|LVIS_FOCUSED);
			m_wndList.EnsureVisible(m_lFocusID,   FALSE);   
		}
		if(m_lFocusID>=m_wndList.GetItemCount())
		{
			m_pModel->WriteDataToRTDB();
			AfxMessageBox("执行完毕");
		}
	}
}

void CViewDebugFrm::OnDebugCancel() 
{
	// TODO: Add your command handler code here
	if(theApp.m_pView!=NULL)
	{
		theApp.m_pView->SendMessage(WM_TASKGUID,20);
	}
}

void CViewDebugFrm::OnUpdateDebugCancel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
