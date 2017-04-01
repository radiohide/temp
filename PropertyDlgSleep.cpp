// PropertyDlgSleep.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgSleep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSleep dialog


CPropertyDlgSleep::CPropertyDlgSleep(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgSleep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgSleep)
	m_iSleep = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgSleep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgSleep)
	DDX_Text(pDX, IDC_EDIT_SLEEP, m_iSleep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgSleep, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgSleep)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSleep message handlers
