// DlgModuleOrder.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgModuleOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModuleOrder dialog


CDlgModuleOrder::CDlgModuleOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModuleOrder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModuleOrder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgModuleOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModuleOrder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModuleOrder, CDialog)
	//{{AFX_MSG_MAP(CDlgModuleOrder)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModuleOrder message handlers
