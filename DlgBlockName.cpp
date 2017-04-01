// DlgBlockName.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgBlockName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockName dialog


CDlgBlockName::CDlgBlockName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBlockName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBlockName)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_strWndTitle =  _T(" Ù–‘");
}


void CDlgBlockName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockName)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlockName, CDialog)
	//{{AFX_MSG_MAP(CDlgBlockName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockName message handlers

BOOL CDlgBlockName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_strWndTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
