// PropertyDlgBackCalc.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgBackCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgBackCalc dialog


CPropertyDlgBackCalc::CPropertyDlgBackCalc(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgBackCalc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgBackCalc)
	m_iBackCalc = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgBackCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgBackCalc)
	DDX_Text(pDX, IDC_EDIT_BACKCALC, m_iBackCalc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgBackCalc, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgBackCalc)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgBackCalc message handlers
