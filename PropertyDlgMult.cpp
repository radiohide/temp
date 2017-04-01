// PropertyDlgMult.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgMult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMult dialog


CPropertyDlgMult::CPropertyDlgMult(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgMult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgMult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgMult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgMult)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgMult, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgMult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgMult message handlers

BOOL CPropertyDlgMult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(m_strInputEdit);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgMult::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(m_strInputEdit);
	m_strInputEdit.TrimLeft();
	m_strInputEdit.TrimRight();
	m_strInputEdit.Remove(' ');
	char oneChar;
	for(int i=0;i<m_strInputEdit.GetLength();i++)
	{
		oneChar=m_strInputEdit.GetAt(i);
		if((oneChar!='*')&&(oneChar!='/'))
		{
			AfxMessageBox("ÊäÈëÅäÖÃ×Ö·û´®ÓÐÎó£¬ÇëÖØÐÂÊäÈë£¡");
			return;
		}
	}
	CDialog::OnOK();
}
