// DlgDiscreteValue.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgDiscreteValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDiscreteValue dialog


CDlgDiscreteValue::CDlgDiscreteValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDiscreteValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiscreteValue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDiscreteValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiscreteValue)
	DDX_Control(pDX, IDC_LIST, m_ctrListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiscreteValue, CDialog)
	//{{AFX_MSG_MAP(CDlgDiscreteValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiscreteValue message handlers

BOOL CDlgDiscreteValue::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetLstCtrlStyle();
	InitList();
	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiscreteValue::SetLstCtrlStyle()
{
	DWORD dwStyle;
	dwStyle = ::GetWindowLong(m_ctrListCtrl.m_hWnd,GWL_STYLE);
	dwStyle |= LVS_REPORT|LVS_SHOWSELALWAYS|LVS_EDITLABELS|LVS_SINGLESEL;
	::SetWindowLong(m_ctrListCtrl.m_hWnd,GWL_STYLE,dwStyle);
	dwStyle = m_ctrListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;
	m_ctrListCtrl.SetExtendedStyle(dwStyle);
}

void CDlgDiscreteValue::InitList()
{
	m_ctrListCtrl.InsertColumn(0,"division_code",LVCFMT_LEFT,100);
	m_ctrListCtrl.InsertColumn(1,"code_desp",LVCFMT_LEFT,100);
	m_ctrListCtrl.InsertColumn(2,"low_value",LVCFMT_LEFT,100);
	m_ctrListCtrl.InsertColumn(3,"high_value",LVCFMT_LEFT,100);	

}

void CDlgDiscreteValue::FillList()
{
	m_ctrListCtrl.DeleteAllItems();
	for(int i=0; i<m_division_code->GetSize(); i++)
	{
		m_ctrListCtrl.InsertItem(i,m_division_code->GetAt(i));
		m_ctrListCtrl.SetItemText(i,1,m_code_desp->GetAt(i));
		m_ctrListCtrl.SetItemText(i,2,m_low_value->GetAt(i));
		m_ctrListCtrl.SetItemText(i,3,m_high_value->GetAt(i));
	}

}
