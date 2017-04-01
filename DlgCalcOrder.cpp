// DlgCalcOrder.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgCalcOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCalcOrder dialog


CDlgCalcOrder::CDlgCalcOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalcOrder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCalcOrder)
	m_iOrderID = 0;
	//}}AFX_DATA_INIT
}


void CDlgCalcOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCalcOrder)
	DDX_Control(pDX, IDC_EDIT_ORDER, m_edit);
	DDX_Text(pDX, IDC_EDIT_ORDER, m_iOrderID);
	DDV_MinMaxInt(pDX, m_iOrderID, -1, 9999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCalcOrder, CDialog)
	//{{AFX_MSG_MAP(CDlgCalcOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalcOrder message handlers

void CDlgCalcOrder::OnOK() 
{
	// TODO: Add extra validation here
	if(!UpdateData(TRUE)) return;
	CDialog::OnOK();
}

BOOL CDlgCalcOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_edit.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
