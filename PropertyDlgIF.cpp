// PropertyDlgIF.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIF dialog


CPropertyDlgIF::CPropertyDlgIF(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgIF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgIF)
	m_StrType = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgIF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgIF)
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_StrType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgIF, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgIF)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIF message handlers
void CPropertyDlgIF::SetType(int type)
{
	if(type==0)
	{
		this->m_StrType="数值";
	}
	else
		this->m_StrType="序列";
}
int CPropertyDlgIF::GetType()
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
void CPropertyDlgIF::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
BOOL CPropertyDlgIF::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}