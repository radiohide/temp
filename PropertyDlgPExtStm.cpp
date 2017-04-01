// PropertyDlgPExtStm.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgPExtStm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPExtStm dialog


CPropertyDlgPExtStm::CPropertyDlgPExtStm(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgPExtStm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgPExtStm)
	m_bBeIgnore = FALSE;
	m_StrWindowText="";
	//}}AFX_DATA_INIT
}


void CPropertyDlgPExtStm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgPExtStm)
	DDX_Check(pDX, IDC_CHECK_BEIGNOREPLOSS, m_bBeIgnore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgPExtStm, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgPExtStm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPExtStm message handlers

void CPropertyDlgPExtStm::SetDlgText(CString str)
{
	m_StrWindowText=str;
}

BOOL CPropertyDlgPExtStm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_StrWindowText);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
