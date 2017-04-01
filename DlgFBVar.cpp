// DlgFBVar.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgFBVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFBVar dialog


CDlgFBVar::CDlgFBVar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFBVar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFBVar)
	pNameArr=NULL;
	pDespArr=NULL;
	pTagBeInArr=NULL;//参与规则推导的前件变量标记,1:参与,0:不参与
	//}}AFX_DATA_INIT
}


void CDlgFBVar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFBVar)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFBVar, CDialog)
	//{{AFX_MSG_MAP(CDlgFBVar)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFBVar message handlers

BOOL CDlgFBVar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgFBVar::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[4]={"序号","前后件名称","前后件描述","选择"};						//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,160);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,80);
	m_List.SetHeaderCheckedState(3,1);
}
void CDlgFBVar::FillListBoxBody()
{
	CString strIndex;
	for(int i=0;i<(*pNameArr).GetSize();i++)
	{
		strIndex.Format("%d",i+1);
		m_List.InsertItem(i,strIndex);
		this->m_List.SetItemText(i,1,(*pNameArr)[i]);
		this->m_List.SetItemText(i,2,(*pDespArr)[i]);
		if((*pTagBeInArr)[i]==1)
		{
			m_List.SetCheckbox(i,3,1);
		}
		else
		{
			m_List.SetCheckbox(i,3,0);
		}
	}
	UpdateData(false);
}

void CDlgFBVar::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	saveCurListSel();
	*pResult = 0;
}

void CDlgFBVar::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	saveCurListSel();
	*pResult = 0;
}
void CDlgFBVar::saveCurListSel()
{
	int rCount=m_List.GetItemCount();
   	for(int i=0; i<rCount; i++)
	{
		if(m_List.GetCheckbox(i,3))
			(*pTagBeInArr)[i]=1;
		else
			(*pTagBeInArr)[i]=0;
	}
}