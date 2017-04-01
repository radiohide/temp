// PropertyDlgClock.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgClock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgClock dialog


CPropertyDlgClock::CPropertyDlgClock(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgClock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgClock)
	m_iBeHaveTime = FALSE;
	m_iBeHaveBeginEndTime = FALSE;
	//}}AFX_DATA_INIT
}


void CPropertyDlgClock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgClock)
	DDX_Check(pDX, IDC_CHECK_TIME, m_iBeHaveTime);
	DDX_Check(pDX, IDC_CHECK_SETIME, m_iBeHaveBeginEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgClock, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgClock)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgClock message handlers
