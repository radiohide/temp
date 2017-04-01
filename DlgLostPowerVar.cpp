// DlgLostPowerVar.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgLostPowerVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLostPowerVar dialog


CDlgLostPowerVar::CDlgLostPowerVar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLostPowerVar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLostPowerVar)
	m_strDesp = _T("");
	m_strInput = _T("");
	m_iValidMax = 0;
	m_iValidMin = 0;
	//}}AFX_DATA_INIT
}


void CDlgLostPowerVar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLostPowerVar)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_EDIT_IVALID_Max, m_iValidMax);
	DDX_Text(pDX, IDC_EDIT_IVALID, m_iValidMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLostPowerVar, CDialog)
	//{{AFX_MSG_MAP(CDlgLostPowerVar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLostPowerVar message handlers
