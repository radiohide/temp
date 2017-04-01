// PropertyDlgStrToNumData.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStrToNumData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrToNumData dialog


CPropertyDlgStrToNumData::CPropertyDlgStrToNumData(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStrToNumData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStrToNumData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgStrToNumData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStrToNumData)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStrToNumData, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStrToNumData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrToNumData message handlers

BOOL CPropertyDlgStrToNumData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	InitListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgStrToNumData::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[4]={"序号","字符值","数字值","备注"};
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,36);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,100);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
}
void CPropertyDlgStrToNumData::InitListBoxBody()
{
	CString str;
	for(int i=0;i<(*pStrValueArr).GetSize();i++)
	{
		str.Format("%d",i+1);
		m_List.InsertItem(i,str);
		this->m_List.SetItemText(i,1,(*pStrValueArr)[i]);
		str.Format("%.1f",(*pDValueArr)[i]);
		this->m_List.SetItemText(i,2,str);
		this->m_List.SetItemText(i,3,(*pStrDespArr)[i]);
	}
}