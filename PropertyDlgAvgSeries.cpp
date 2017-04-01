// PropertyDlgAvgSeries.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgAvgSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAvgSeries dialog


CPropertyDlgAvgSeries::CPropertyDlgAvgSeries(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgAvgSeries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgAvgSeries)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgAvgSeries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgAvgSeries)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgAvgSeries, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgAvgSeries)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAvgSeries message handlers
