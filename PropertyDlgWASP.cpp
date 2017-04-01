// PropertyDlgWASP.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgWASP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWASP dialog


CPropertyDlgWASP::CPropertyDlgWASP(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgWASP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgWASP)
	m_strFunDesp = _T("");
	m_strFunName = _T("");
	m_strBlockDesp = _T("");

	m_pStrArrFunDisp=NULL;//所有方法描述
	m_pStrArrFunName=NULL;//所有方法名称
	//}}AFX_DATA_INIT
}


void CPropertyDlgWASP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgWASP)
	DDX_Control(pDX, IDC_COMBO_FUNDESP, m_comboxFunDesp);
	DDX_CBString(pDX, IDC_COMBO_FUNDESP, m_strFunDesp);
	DDX_Text(pDX, IDC_EDIT_FUNNAME, m_strFunName);
	DDX_Text(pDX, IDC_STATIC_BLOCKDISP, m_strBlockDesp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgWASP, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgWASP)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNDESP, OnSelchangeComboFundesp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWASP message handlers

BOOL CPropertyDlgWASP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	for(int i=0;i<(*m_pStrArrFunDisp).GetSize();i++)
	{
		this->m_comboxFunDesp.AddString((*m_pStrArrFunDisp)[i]);
	}
	int index=-1;
	for(i=0;i<(*m_pStrArrFunName).GetSize();i++)
	{
		if(m_strFunName==(*m_pStrArrFunName)[i])
		{
			index=i;
			break;
		}
	}
	if(index!=-1)
		this->m_strFunDesp=(*m_pStrArrFunDisp)[index];

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgWASP::OnSelchangeComboFundesp() 
{
	// TODO: Add your control notification handler code here
	this->m_strFunDesp="";
	int selIndex=this->m_comboxFunDesp.GetCurSel();
	this->m_strFunName=(*m_pStrArrFunName)[selIndex];
	UpdateData(false);
}

void CPropertyDlgWASP::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
