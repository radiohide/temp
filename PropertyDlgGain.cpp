// PropertyDlgGain.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgGain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgGain dialog


CPropertyDlgGain::CPropertyDlgGain(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgGain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgGain)
	m_dGainValue = 0.0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgGain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgGain)
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgGain, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgGain)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgGain message handlers
