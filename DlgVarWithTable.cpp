// DlgVarWithTable.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgVarWithTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVarWithTable dialog


CDlgVarWithTable::CDlgVarWithTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVarWithTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVarWithTable)
	m_strDesp = _T("");
	m_strUnit = _T("");
	m_strTableName = _T("");
	m_strVarName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgVarWithTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVarWithTable)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_EDIT_TABLENAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVarWithTable, CDialog)
	//{{AFX_MSG_MAP(CDlgVarWithTable)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVarWithTable message handlers
