// PropertyDlgNumToStr.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgNumToStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgNumToStr dialog


CPropertyDlgNumToStr::CPropertyDlgNumToStr(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgNumToStr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgNumToStr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgNumToStr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgNumToStr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgNumToStr, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgNumToStr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgNumToStr message handlers
