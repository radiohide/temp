// OutputBar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "OutputBar.h"

#include "CalcBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here

}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Font.CreateStockObject (DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.SetFlatFrame (FALSE, FALSE);

	// Create list windows.
	// TODO: create your own window here:
	const DWORD dwViewStyle =	
		LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_wndList1.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) )
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}
	if (!m_wndLog.Create (dwViewStyle, rectDummy, &m_wndTabs, 4) )
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}
    m_wndList1.SetFont(&m_Font);
	
	// Create output pane:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;

	if (!m_wndWatch.Create (dwStyle, rectDummy, &m_wndTabs, 3) )
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}
	m_wndWatch.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
						LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndWatch.InsertColumn (0, _T("模块端口"), LVCFMT_LEFT, 100);
	m_wndWatch.InsertColumn (1, _T("数据值"), LVCFMT_LEFT, 100);
	m_wndWatch.InsertColumn (2, _T("计算块名"), LVCFMT_LEFT, 150);

	//m_wndWatch.InsertItem (0, _T("m_nCount"));
	//m_wndWatch.SetItemText (0, 1, _T("100"));
	m_wndWatch.SetFont (&m_Font);

	// Attach list windows to tab:
	m_wndTabs.AddTab (&m_wndList1, _T("编译信息"), -1);
	m_wndTabs.AddTab (&m_wndWatch, _T("观察窗口"), -1);
	m_wndTabs.AddTab (&m_wndLog, _T("日志窗口"), -1);
	//m_wndTabs.AddTab (&m_wndList3, _T("Output 3"), -1);
	m_wndTabs.EnableTabSwap(FALSE);


	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputBar::Output_Add(int page, CString str)
{
   if(page ==0)
   {
	   m_wndList1.AddString(str);
   }
}
void COutputBar::Log_Add(int page, CString str)
{
   if(page ==2)
   {
	   m_wndLog.AddString(str);
   }
}
void COutputBar::Clear_Log(int page)
{
   if(page ==2)
   {
		m_wndLog.ResetContent();
   }
}
void COutputBar::Clear_Ouput(int page)
{
   if(page ==0)
   {
		m_wndList1.ResetContent();
   }
}

CListCtrl* COutputBar::GetWatchListCtrl()
{
      return &m_wndWatch;
}

void COutputBar::ActivePage(int pageID)
{
	m_wndTabs.SetActiveTab(pageID);
}
