// DlgVarDataBase.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgVarDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataBase dialog


CDlgVarDataBase::CDlgVarDataBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVarDataBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVarDataBase)
	m_strDesp = _T("");
	m_strVarName = _T("");
	m_strUnit = _T("");
	//}}AFX_DATA_INIT
}


void CDlgVarDataBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVarDataBase)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVarDataBase, CDialog)
	//{{AFX_MSG_MAP(CDlgVarDataBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataBase message handlers

BOOL CDlgVarDataBase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVarDataBase::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(m_strVarName=="")
	{
		AfxMessageBox("变量名不能为空，请输入！");
		return;
	}
	CDialog::OnOK();
}
