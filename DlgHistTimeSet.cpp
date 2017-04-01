// DlgHistTimeSet.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgHistTimeSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistTimeSet dialog


CDlgHistTimeSet::CDlgHistTimeSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistTimeSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistTimeSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHistTimeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistTimeSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistTimeSet, CDialog)
	//{{AFX_MSG_MAP(CDlgHistTimeSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistTimeSet message handlers
