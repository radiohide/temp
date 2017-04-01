// DlgVarWithPort.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgVarWithPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVarWithPort dialog


CDlgVarWithPort::CDlgVarWithPort(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVarWithPort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVarWithPort)
	m_strDesp = _T("");
	m_strUnit = _T("");
	m_strVarName = _T("");
	m_strPortName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgVarWithPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVarWithPort)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	DDX_Text(pDX, IDC_EDIT_PORTNAME, m_strPortName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVarWithPort, CDialog)
	//{{AFX_MSG_MAP(CDlgVarWithPort)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVarWithPort message handlers
