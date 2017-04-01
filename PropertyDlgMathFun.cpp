// PropertyDlgMathFun.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMathFun dialog


CPropertyDlgMathFun::CPropertyDlgMathFun(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgMathFun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgMathFun)
	m_iInputCount = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgMathFun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgMathFun)
	DDX_Control(pDX, IDC_COMBO_FUNDESP, m_comboxFunDesp);
	DDX_CBString(pDX, IDC_COMBO_FUNDESP, m_strFunDesp);
	DDX_Text(pDX, IDC_EDIT_FUNNAME, m_strFunName);
	DDX_Text(pDX, IDC_STATIC_BLOCKDISP, m_strBlockDesp);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_iInputCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgMathFun, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgMathFun)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNDESP, OnSelchangeComboFundesp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMathFun message handlers

BOOL CPropertyDlgMathFun::OnInitDialog() 
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
	this->SetWindowText(m_strPropertityTableName);
	if((m_strPropertityTableName!="逻辑计算块")&&(m_strPropertityTableName!="数据选择块"))
	{
		GetDlgItem(IDC_STATIC_INPUT)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_INPUT)->ShowWindow(false);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgMathFun::OnSelchangeComboFundesp() 
{
	// TODO: Add your control notification handler code here
	this->m_strFunDesp="";
	int selIndex=this->m_comboxFunDesp.GetCurSel();
	this->m_strFunName=(*m_pStrArrFunName)[selIndex];
	if(m_strFunName=="!")
	{
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(false);
	}
	else
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(true);
	UpdateData(false);
}