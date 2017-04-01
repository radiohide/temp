// DlgCfgFindBlock.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgCfgFindBlock.h"
//#include "ViewModuleFrm.h";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgFindBlock dialog
#include "ViewModuleFrm.h"

CDlgCfgFindBlock::CDlgCfgFindBlock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgFindBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgFindBlock)
	pDlgFindAndReplace=NULL;
	//}}AFX_DATA_INIT
}


void CDlgCfgFindBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgFindBlock)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_TAB1, m_TabList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgFindBlock, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgFindBlock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BTN_FINDNEXT, OnBtnFindNext)
	ON_BN_CLICKED(IDC_BTN_FINDALL, OnBtnFindAll)
	ON_BN_CLICKED(IDC_BTN_REPLACE, OnBtnReplace)
	ON_BN_CLICKED(IDC_BTN_REPLACEALL, OnBtnReplaceall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgFindBlock message handlers

int CDlgCfgFindBlock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}



void CDlgCfgFindBlock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect m_rect;
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_LIST1);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right=m_rect.Width()-10;
		rect.bottom=m_rect.Height()-10;
		pWnd->MoveWindow(rect);//设置控件大小
	}
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小
}

void CDlgCfgFindBlock::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI-> ptMinTrackSize.y   =   250;//高 
	lpMMI-> ptMinTrackSize.x   =   500;//宽 
	lpMMI->ptMaxTrackSize.x=500;//宽 

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDlgCfgFindBlock::initTabCtrl()
{
	CRect rc;
	m_TabList.GetClientRect(rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 1;

	if(pDlgFindAndReplace!=NULL)
	{
		delete pDlgFindAndReplace;
		pDlgFindAndReplace=NULL;
	}
	this->m_TabList.InsertItem(0,_T("查找"));
	this->m_TabList.InsertItem(1,_T("替换"));
	pDlgFindAndReplace=new CDlgTabFindBlock;
	
	pDlgFindAndReplace->Create(IDD_DIG_TAB_FINDBLOCK,&m_TabList);//必须执行create
	pDlgFindAndReplace->MoveWindow(rc.left,rc.top,rc.Width()-10,rc.Height()-10);
	pDlgFindAndReplace->SetSomeCtrlObjHide();
	GetDlgItem(IDC_BTN_REPLACEALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_REPLACE)->ShowWindow(SW_HIDE);
	pDlgFindAndReplace->ShowWindow(SW_SHOW);
	m_iTagFindOrReplace=0;//显示查找页面
}

BOOL CDlgCfgFindBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_iIndexFindNext=0;
	m_iTagFindOrReplace=0;//启动默认查找
	// TODO: Add extra initialization here
	initTabCtrl();
	InitListBoxHead();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCfgFindBlock::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnOK();
}

void CDlgCfgFindBlock::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(0==m_TabList.GetCurSel())
	{
		pDlgFindAndReplace->SetSomeCtrlObjHide();
		GetDlgItem(IDC_BTN_REPLACEALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_REPLACE)->ShowWindow(SW_HIDE);
		m_iTagFindOrReplace=0;
	}
	else
	{
		pDlgFindAndReplace->SetSomeCtrlObjShow();
		GetDlgItem(IDC_BTN_REPLACEALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_REPLACE)->ShowWindow(SW_SHOW);
		m_iTagFindOrReplace=1;
	}
	*pResult = 0;
}

void CDlgCfgFindBlock::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CDlgCfgFindBlock::OnBtnFindNext() 
{
	// TODO: Add your control notification handler code here
	if(pDlgFindAndReplace->m_bTagEdit)
	{
		findAllBlock();
		InitListBoxHead();
		m_iIndexFindNext=0;
	}
	else
	{
		if(m_iIndexFindNext==m_blockFindArr.GetSize())
		{
			m_iIndexFindNext=0;
		}
	}
	if(m_blockFindArr.GetSize()==0)
	{
		AfxMessageBox("找不到正在搜索的数据.");
		return;
	}
	//跳转并选中
	selBlockInView(m_iIndexFindNext);
	setListBeSel(m_iIndexFindNext);
	m_iIndexFindNext++;
}
void CDlgCfgFindBlock::selBlockInView(int index)
{
	for(int i=0;i<m_blockFindArr.GetSize();i++)
	{
		CCalcBlock *pBlock=m_blockFindArr.GetAt(i);
		if(i==index)//在页面只选择该计算块
		{
			theApp.m_pTreeItemObj->m_strName=pBlock->GetModulePtr()->GetModuleName();
			if(theApp.m_pView!=NULL)
			{
				theApp.m_pView->SendMessage(WM_TASKGUID,1);
			}
			CBCGPTabWnd &wndTabs=(((CBCGPTabView*)(theApp.m_pView))->GetTabControl());
			int tabCount=wndTabs.GetTabsNum();
			for(int i=0;i<tabCount;i++)
			{
				CViewModuleFrm *pView=(CViewModuleFrm *)wndTabs.GetTabWnd(i);
				if(pView->m_strModuleName==pBlock->GetModulePtr()->GetModuleName())
				{
					pView->SetUnselAllObj();
					pView->SetSelBlockInVeiwByName(pBlock->GetBlockName());
					pView->RefreshWindow();
					break;
				}
			}
			break;
		}
	}
}
void CDlgCfgFindBlock::setListBeSel(int index)
{
	for(int i=0;i<this->m_List.GetItemCount();i++)
	{
		m_List.SetFocus();
		if(index==i)
		{
			m_List.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED|LVIS_FOCUSED); 
		}
		else
			m_List.SetItemState(i, 0,LVIS_FOCUSED|LVIS_SELECTED); 
	}
	UpdateData(false);
}
void CDlgCfgFindBlock::findAllBlock()
{
	m_blockFindArr.RemoveAll();
	if(theApp.m_pView==NULL)
		return;
	pDlgFindAndReplace->UpdateData();
	if(pDlgFindAndReplace->m_strRange=="当前打开模块")//查找当前活跃
	{
		CViewModuleFrm *pView = (CViewModuleFrm *)(((CBCGPTabView*)(theApp.m_pView))->GetActiveView());
		if(pDlgFindAndReplace->m_strType=="计算块名称")
		{
			pView->FindBlockByStrName(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
		}
		else if(pDlgFindAndReplace->m_strType=="计算块描述")
		{
			pView->FindBlockByStrDesp(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
		}
		else if(pDlgFindAndReplace->m_strType=="属性")
		{
			pView->FindBlockByStrPro(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
		}
	}
	else//查找所有模块
	{
		CBCGPTabWnd &wndTabs=(((CBCGPTabView*)(theApp.m_pView))->GetTabControl());
		int tabCount=wndTabs.GetTabsNum();
		for(int i=0;i<tabCount;i++)
		{
			CViewModuleFrm *pView=(CViewModuleFrm *)wndTabs.GetTabWnd(i);
			if(pDlgFindAndReplace->m_strType=="计算块名称")
			{
				pView->FindBlockByStrName(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
			}
			else if(pDlgFindAndReplace->m_strType=="计算块描述")
			{
				pView->FindBlockByStrDesp(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
			}
			else if(pDlgFindAndReplace->m_strType=="属性")
			{
				pView->FindBlockByStrPro(pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_bAllSame,m_blockFindArr);
			}
		}
	}
	pDlgFindAndReplace->m_bTagEdit=false;
}
void CDlgCfgFindBlock::OnBtnFindAll() 
{
	// TODO: Add your control notification handler code here
	findAllBlock();
	InitListBoxHead();
	InitListBoxBody();
}

void CDlgCfgFindBlock::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","模块名","计算块名","计算块描述","属性"};
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,36);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,100);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,100);
}
void CDlgCfgFindBlock::InitListBoxBody()
{
	CString str;
	for(int i=0;i<m_blockFindArr.GetSize();i++)
	{
		str.Format("%d",i+1);
		m_List.InsertItem(i,str);
		CCalcBlock *pBlock=m_blockFindArr.GetAt(i);
		CString strModuleName=pBlock->GetModulePtr()->GetModuleName();
		this->m_List.SetItemText(i,1,strModuleName);
		CString strBName=pBlock->GetBlockName();
		this->m_List.SetItemText(i,2,strBName);
		CString strBDesp=pBlock->GetBlockDesp();
		this->m_List.SetItemText(i,3,strBDesp);
		CString strBPro=pBlock->GetBlockStrPro();
		this->m_List.SetItemText(i,4,strBPro);
	}
}

void CDlgCfgFindBlock::OnBtnReplace() 
{
	// TODO: Add your control notification handler code here
	if(pDlgFindAndReplace->m_bTagEdit)
	{
		findAllBlock();
		InitListBoxHead();
		m_iIndexFindNext=0;
	}
	else
	{
		if(m_iIndexFindNext==m_blockFindArr.GetSize())
		{
			m_iIndexFindNext=0;
			m_blockFindArr.RemoveAll();
		}
	}
	if(m_blockFindArr.GetSize()==0)
	{
		AfxMessageBox("找不到匹配项.");
		return;
	}
	//跳转并选中
	replaceBlockInView(m_iIndexFindNext);
	selBlockInView(m_iIndexFindNext+1);
	replaceListByIndex(m_iIndexFindNext);
	m_iIndexFindNext++;
}
void CDlgCfgFindBlock::replaceBlockInView(int index)
{
	for(int i=0;i<m_blockFindArr.GetSize();i++)
	{
		CCalcBlock *pBlock=m_blockFindArr.GetAt(i);
		if(i==index)//在页面只选择该计算块
		{
			theApp.m_pTreeItemObj->m_strName=pBlock->GetModulePtr()->GetModuleName();
			if(theApp.m_pView!=NULL)
			{
				theApp.m_pView->SendMessage(WM_TASKGUID,1);
			}
			CBCGPTabWnd &wndTabs=(((CBCGPTabView*)(theApp.m_pView))->GetTabControl());
			int tabCount=wndTabs.GetTabsNum();
			pDlgFindAndReplace->UpdateData();
			for(int i=0;i<tabCount;i++)
			{
				CViewModuleFrm *pView=(CViewModuleFrm *)wndTabs.GetTabWnd(i);
				if(pView->m_strModuleName==pBlock->GetModulePtr()->GetModuleName())
				{
					if(pDlgFindAndReplace->m_strType=="计算块名称")
					{
						pView->ReplaceBlockName(pBlock->GetBlockName(),pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_strReplace);
					}
					else if(pDlgFindAndReplace->m_strType=="计算块描述")
					{
						pView->ReplaceBlockDesp(pBlock->GetBlockName(),pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_strReplace);
					}
					else if(pDlgFindAndReplace->m_strType=="属性")
					{
						pView->ReplaceBlockPro(pBlock->GetBlockName(),pDlgFindAndReplace->m_strFind,pDlgFindAndReplace->m_strReplace);
					}
					pView->RefreshWindow();
					break;
				}
			}
			break;
		}
	}
}
void CDlgCfgFindBlock::replaceListByIndex(int index)
{
	for(int i=0;i<this->m_List.GetItemCount();i++)
	{
		if(index==i)
		{
			CCalcBlock *pBlock=m_blockFindArr.GetAt(i);
			CString strModuleName=pBlock->GetModulePtr()->GetModuleName();
			this->m_List.SetItemText(i,1,strModuleName);
			CString strBName=pBlock->GetBlockName();
			this->m_List.SetItemText(i,2,strBName);
			CString strBDesp=pBlock->GetBlockDesp();
			this->m_List.SetItemText(i,3,strBDesp);
			CString strBPro=pBlock->GetBlockStrPro();
			this->m_List.SetItemText(i,4,strBPro);
			break;
		}
	}
	UpdateData(false);
}
void CDlgCfgFindBlock::OnBtnReplaceall() 
{
	// TODO: Add your control notification handler code here
	findAllBlock();
	if(m_blockFindArr.GetSize()==0)
	{
		AfxMessageBox("找不到匹配项.");
		return;
	}
	for(int i=0;i<m_blockFindArr.GetSize();i++)
	{
		replaceBlockInView(i);
	}
	InitListBoxHead();
	InitListBoxBody();
	CString str;
	str.Format("已完成搜索并进行了%d处替换",m_blockFindArr.GetSize());
	AfxMessageBox(str);
	m_blockFindArr.RemoveAll();
}

void CDlgCfgFindBlock::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos=this->m_List.GetFirstSelectedItemPosition( );
	if(pos)
	{
		int iSelIndex=this->m_List.GetNextSelectedItem(pos);
		selBlockInView(iSelIndex);
	}
	*pResult = 0;
}
