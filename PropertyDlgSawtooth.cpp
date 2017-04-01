// PropertyDlgSawtooth.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgSawtooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSawtooth dialog


CPropertyDlgSawtooth::CPropertyDlgSawtooth(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgSawtooth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgSawtooth)
	m_iMin = 0;
	m_iMax = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgSawtooth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgSawtooth)
	DDX_Text(pDX, IDC_EDIT_MIN, m_iMin);
	DDX_Text(pDX, IDC_EDIT_MAX, m_iMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgSawtooth, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgSawtooth)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSawtooth message handlers
