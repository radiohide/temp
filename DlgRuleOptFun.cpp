// DlgRuleOptFun.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgRuleOptFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleOptFun dialog


CDlgRuleOptFun::CDlgRuleOptFun(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRuleOptFun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRuleOptFun)
	m_iCGID = 0;
	m_strDesp = _T("");
	m_strFormula = _T("");
	//}}AFX_DATA_INIT
}


void CDlgRuleOptFun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRuleOptFun)
	DDX_Text(pDX, IDC_EDIT_CGID, m_iCGID);
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_FUNFORMULA, m_strFormula);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRuleOptFun, CDialog)
	//{{AFX_MSG_MAP(CDlgRuleOptFun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleOptFun message handlers

void CDlgRuleOptFun::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strFormula=="")
	{
		AfxMessageBox("公式不能为空");
		return;
	}
	if(m_iCGID<0)
	{
		AfxMessageBox("复合组ID不能为负数！");
		return;
	}
	CDialog::OnOK();
}
