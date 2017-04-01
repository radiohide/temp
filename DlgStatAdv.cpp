// DlgStatAdv.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgStatAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatAdv dialog


CDlgStatAdv::CDlgStatAdv(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatAdv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatAdv)
	m_BCheckTimeMInt = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgStatAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatAdv)
	DDX_Check(pDX, IDC_CHECK_TIME_MINT, m_BCheckTimeMInt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatAdv, CDialog)
	//{{AFX_MSG_MAP(CDlgStatAdv)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStatAdv message handlers
