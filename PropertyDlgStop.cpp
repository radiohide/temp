// PropertyDlgStop.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgStop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStop dialog


CPropertyDlgStop::CPropertyDlgStop(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStop)
	m_StrBlockDespText="当输入为非零值时该模型运行停止。";
	m_StrWinText="模型停止块";
	//}}AFX_DATA_INIT
}


void CPropertyDlgStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStop)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStop, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStop message handlers

BOOL CPropertyDlgStop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_STATIC1)->SetWindowText(m_StrBlockDespText);
	this->SetWindowText(m_StrWinText);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
