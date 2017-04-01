// PropertyDlgSineWave.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgSineWave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSineWave dialog


CPropertyDlgSineWave::CPropertyDlgSineWave(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgSineWave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgSineWave)
	m_dFengzhi = 0.0;
	m_dZhouqi = 0.0;
	m_dLeftRight = 0.0;
	m_dUpDown = 0.0;
	m_dInitValue = 0.0;
	m_dStep = 0.0;
	//}}AFX_DATA_INIT
	m_index=0;
}


void CPropertyDlgSineWave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgSineWave)
	DDX_Control(pDX, IDC_EDIT8, m_EditStep);
	DDX_Control(pDX, IDC_EDIT7, m_EditInitValue);
	DDX_Control(pDX, IDC_Comb, m_CombSource);
	DDX_Text(pDX, IDC_EDIT1, m_dFengzhi);
	DDX_Text(pDX, IDC_EDIT2, m_dZhouqi);
	DDX_Text(pDX, IDC_EDIT3, m_dLeftRight);
	DDX_Text(pDX, IDC_EDIT4, m_dUpDown);
	DDX_Text(pDX, IDC_EDIT7, m_dInitValue);
	DDX_Text(pDX, IDC_EDIT8, m_dStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgSineWave, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgSineWave)
	ON_CBN_SELCHANGE(IDC_Comb, OnSelchangeComb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgSineWave message handlers

BOOL CPropertyDlgSineWave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData();

	if(0==m_index)
	{
		m_CombSource.SetCurSel(0);
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	}
	else if(1==m_index)
	{
		m_CombSource.SetCurSel(1);
        GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgSineWave::OnSelchangeComb() 
{
	// TODO: Add your control notification handler code here
	if(0==m_CombSource.GetCurSel())
	{	
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
		m_index=0;
	}
	else if(1==m_CombSource.GetCurSel())
	{
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
		m_index=1;
	}
	return ;
}
