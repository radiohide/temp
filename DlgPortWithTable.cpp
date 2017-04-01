// DlgPortWithTable.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgPortWithTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPortWithTable dialog


CDlgPortWithTable::CDlgPortWithTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPortWithTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPortWithTable)
	m_strDesp = _T("");
	m_strUnit = _T("");
	m_strTableName = _T("");
	m_strVarName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPortWithTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPortWithTable)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_EDIT_TABLENAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPortWithTable, CDialog)
	//{{AFX_MSG_MAP(CDlgPortWithTable)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPortWithTable message handlers
