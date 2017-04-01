// DlgFormulaData.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "DlgFormulaData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaData dialog


CDlgFormulaData::CDlgFormulaData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormulaData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormulaData)
	m_EditFormula = _T("");
	m_EditVarName = _T("");
	m_StrType= _T("");
	//}}AFX_DATA_INIT
}


void CDlgFormulaData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormulaData)
	DDX_Text(pDX, IDC_EDIT_FORMULA, m_EditFormula);
	DDX_Text(pDX, IDC_EDITNAME, m_EditVarName);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_StrType);
	//DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_StrType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormulaData, CDialog)
	//{{AFX_MSG_MAP(CDlgFormulaData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaData message handlers
void CDlgFormulaData::SetType(int type)
{
	if(type==0)
	{
		this->m_StrType="数值";
	}
	else
		this->m_StrType="序列";
}

int CDlgFormulaData::GetType()
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

void CDlgFormulaData::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();
	if(this->m_EditVarName=="")
	{
		AfxMessageBox("变量名不能为空!");
		return;
	}
	if(this->m_EditFormula=="")
	{
		AfxMessageBox("公式不能为空!");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgFormulaData::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
