// PropertyDlgIntegrate.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgIntegrate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIntegrate dialog


CPropertyDlgIntegrate::CPropertyDlgIntegrate(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgIntegrate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgIntegrate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgIntegrate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgIntegrate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgIntegrate, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgIntegrate)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIntegrate message handlers
