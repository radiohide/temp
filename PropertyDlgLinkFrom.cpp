// PropertyDlgLinkFrom.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgLinkFrom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFrom dialog


CPropertyDlgLinkFrom::CPropertyDlgLinkFrom(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgLinkFrom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgLinkFrom)
	m_VarTag1 = _T("");
	m_VarTag2 = _T("");
	m_VarTag3 = _T("");
	m_VarTag4 = _T("");
	m_VarTag5 = _T("");

	m_StrType1 = _T("");
	m_StrType2 = _T("");
	m_StrType3 = _T("");
	m_StrType4 = _T("");
	m_StrType5 = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgLinkFrom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgLinkFrom)
	DDX_Text(pDX, IDC_EDIT1, m_VarTag1);
	DDX_Text(pDX, IDC_EDIT2, m_VarTag2);
	DDX_Text(pDX, IDC_EDIT3, m_VarTag3);
	DDX_Text(pDX, IDC_EDIT4, m_VarTag4);
	DDX_Text(pDX, IDC_EDIT5, m_VarTag5);

	DDX_CBString(pDX, IDC_COMBO_TYPE1, m_StrType1);
	DDX_CBString(pDX, IDC_COMBO_TYPE2, m_StrType2);
	DDX_CBString(pDX, IDC_COMBO_TYPE3, m_StrType3);
	DDX_CBString(pDX, IDC_COMBO_TYPE4, m_StrType4);
	DDX_CBString(pDX, IDC_COMBO_TYPE5, m_StrType5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgLinkFrom, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgLinkFrom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFrom message handlers

void CPropertyDlgLinkFrom::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CPropertyDlgLinkFrom::SetType(int num,int type)
{
	switch(num)
	{
	case 1:
		if(type==0)
		{
			this->m_StrType1="数值";
		}
		else
			this->m_StrType1="序列";
		break;
	case 2:
		if(type==0)
		{
			this->m_StrType2="数值";
		}
		else
			this->m_StrType2="序列";
		break;
	case 3:
		if(type==0)
		{
			this->m_StrType3="数值";
		}
		else
			this->m_StrType3="序列";
		break;
	case 4:
		if(type==0)
		{
			this->m_StrType4="数值";
		}
		else
			this->m_StrType4="序列";
		break;
	case 5:
		if(type==0)
		{
			this->m_StrType5="数值";
		}
		else
			this->m_StrType5="序列";
		break;
	}
}
int CPropertyDlgLinkFrom::GetType(int num)
{
	int type=0;
	switch(num)
	{
	case 1:
		if(this->m_StrType1=="数值")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 2:
		if(this->m_StrType2=="数值")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 3:
		if(this->m_StrType3=="数值")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 4:
		if(this->m_StrType4=="数值")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 5:
		if(this->m_StrType5=="数值")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	}
	return type;
}