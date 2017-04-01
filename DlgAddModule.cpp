// DlgAddModule.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgAddModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddModule dialog


CDlgAddModule::CDlgAddModule(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddModule)
	m_strDesp = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAddModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddModule)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddModule, CDialog)
	//{{AFX_MSG_MAP(CDlgAddModule)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddModule message handlers
void CDlgAddModule::OnOK()
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strDesp=="")
	{
		AfxMessageBox("请输入描述信息！");
		return;
	}
	if(m_strName=="")
	{
		AfxMessageBox("请输入英文关键字！");
		return;
	}
	CDialog::OnOK();
}