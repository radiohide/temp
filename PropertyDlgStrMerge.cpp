// PropertyDlgStrMerge.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStrMerge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMerge dialog


CPropertyDlgStrMerge::CPropertyDlgStrMerge(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStrMerge::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStrMerge)
	m_strInputNum = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgStrMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStrMerge)
	DDX_Text(pDX, IDC_InputNum, m_strInputNum);
	DDV_MinMaxInt(pDX, m_strInputNum, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStrMerge, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStrMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMerge message handlers

BOOL CPropertyDlgStrMerge::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
