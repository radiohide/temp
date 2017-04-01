// PropertyDlgAdd.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAdd dialog


CPropertyDlgAdd::CPropertyDlgAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgAdd)
	m_strInputEdit="";
	//}}AFX_DATA_INIT
}


void CPropertyDlgAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgAdd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgAdd, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgAdd message handlers

BOOL CPropertyDlgAdd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(m_strInputEdit);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgAdd::OnOK() 
{
	// TODO: Add extra validation here
	//输入条件限制功能
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(m_strInputEdit);
	m_strInputEdit.TrimLeft();
	m_strInputEdit.TrimRight();
	m_strInputEdit.Remove(' ');
	char oneChar;
	for(int i=0;i<m_strInputEdit.GetLength();i++)
	{
		oneChar=m_strInputEdit.GetAt(i);
		if((oneChar!='+')&&(oneChar!='-'))
		{
			AfxMessageBox("输入配置字符串有误，请重新输入！");
			return;
		}
	}
	CDialog::OnOK();
}
