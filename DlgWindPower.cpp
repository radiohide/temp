// DlgWindPower.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgWindPower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWindPower dialog


CDlgWindPower::CDlgWindPower(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWindPower::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWindPower)
	m_dPower = 0.0;
	m_dWindSpeed = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgWindPower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWindPower)
	DDX_Text(pDX, IDC_EDIT_POWER, m_dPower);
	DDX_Text(pDX, IDC_EDIT_WIND, m_dWindSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWindPower, CDialog)
	//{{AFX_MSG_MAP(CDlgWindPower)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWindPower message handlers
