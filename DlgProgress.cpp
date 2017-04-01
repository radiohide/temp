// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	m_bCancel=FALSE;
	//}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PROGRESS1, m_progTotal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_MESSAGE(WM_MSG_PROGESS,OnProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

void CDlgProgress::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bCancel=TRUE;
	//CDialog::OnCancel();
}
void CDlgProgress::SetRange(int Begin, int End)
{
	m_progTotal.SetRange(Begin,End);
}
void CDlgProgress::OnProgress(WPARAM wParam, LPARAM lParam)
{
	float persent = (float)wParam;
	m_progTotal.SetPos(int(persent));
	UpdateData(false);
}

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetRange(0,100);
	GetDlgItem(IDC_STATIC1)->ShowWindow(true);
	GetDlgItem(IDCANCEL)->ShowWindow(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
