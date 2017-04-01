// PropertyDlgSumSeries.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgSumSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSumSeries dialog


CPropertyDlgSumSeries::CPropertyDlgSumSeries(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgSumSeries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgSumSeries)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgSumSeries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgSumSeries)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgSumSeries, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgSumSeries)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSumSeries message handlers
