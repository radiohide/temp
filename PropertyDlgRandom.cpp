// PropertyDlgRandom.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRandom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRandom dialog


CPropertyDlgRandom::CPropertyDlgRandom(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgRandom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgRandom)
	m_dMin = 0;
	m_dMax = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgRandom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgRandom)
	DDX_Text(pDX, IDC_EDIT1, m_dMin);
	DDX_Text(pDX, IDC_EDIT2, m_dMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgRandom, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgRandom)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRandom message handlers
