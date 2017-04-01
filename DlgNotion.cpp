// DlgNotion.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgNotion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNotion dialog


CDlgNotion::CDlgNotion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNotion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNotion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNotion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNotion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNotion, CDialog)
	//{{AFX_MSG_MAP(CDlgNotion)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNotion message handlers
