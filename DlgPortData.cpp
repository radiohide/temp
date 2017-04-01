// DlgPortData.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgPortData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPortData dialog


CDlgPortData::CDlgPortData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPortData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPortData)
	m_StrType = _T("");
	m_StrName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPortData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPortData)
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_StrType);
	DDX_Text(pDX, IDC_EDITNAME, m_StrName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPortData, CDialog)
	//{{AFX_MSG_MAP(CDlgPortData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPortData message handlers
void CDlgPortData::SetType(int type)
{
	if(type==0)
	{
		this->m_StrType="数值";
	}
	else
		this->m_StrType="序列";
}
int CDlgPortData::GetType()
{
	int type=0;
	//UpdateData(true);
	if(this->m_StrType=="数值")
	{
		type=0;
	}
	else
	{
		type=1;
	}
	return type;
}

void CDlgPortData::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();
	if(this->m_StrName=="")
	{
		AfxMessageBox("变量名不能为空!");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgPortData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
