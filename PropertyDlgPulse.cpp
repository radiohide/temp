// PropertyDlgPulse.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgPulse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPulse dialog


CPropertyDlgPulse::CPropertyDlgPulse(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgPulse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgPulse)
	m_dFengzhi = 3.0;
	m_dFengzhiNegative = 0.0;
	m_dZhouqi = 2.0;
	m_dYanchi = 0.0;
	m_dMaichongKD = 0.5;

	m_dInitValue = 0.0;
	m_dStep = 3.0;
	
	//}}AFX_DATA_INIT
}


void CPropertyDlgPulse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgPulse)
	DDX_Control(pDX, IDC_EDIT8, m_EditStep);
	DDX_Control(pDX, IDC_EDIT7, m_EditInitValue);
	DDX_Control(pDX, IDC_Comb, m_CombSource);
	DDX_Text(pDX, IDC_EDIT1, m_dFengzhi);
	DDX_Text(pDX, IDC_EDIT2, m_dZhouqi);
	DDX_Text(pDX, IDC_EDIT3, m_dYanchi);
	DDX_Text(pDX, IDC_EDIT4, m_dMaichongKD);
	DDV_MinMaxDouble(pDX, m_dMaichongKD, 0., 1.);
	DDX_Text(pDX, IDC_EDIT7, m_dInitValue);
	DDX_Text(pDX, IDC_EDIT8, m_dStep);
	DDX_Text(pDX, IDC_EDIT6, m_dFengzhiNegative);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgPulse, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgPulse)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnKillfocusEdit6)
	ON_CBN_SELCHANGE(IDC_Comb, OnSelchangeComb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPulse message handlers

void CPropertyDlgPulse::OnKillfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	CString str1,str6;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT6)->GetWindowText(str6);
	if(atof(str1)<atof(str6))
	{
		AfxMessageBox("最大值和最小值不匹配!");
	}
	return;
}

void CPropertyDlgPulse::OnKillfocusEdit6() 
{
	// TODO: Add your control notification handler code here
	CString str1,str6;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT6)->GetWindowText(str6);
	if(atof(str1)<atof(str6))
	{
		AfxMessageBox("最大值和最小值不匹配!");
	}
	return;
}

BOOL CPropertyDlgPulse::OnInitDialog() 
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

void CPropertyDlgPulse::OnSelchangeComb() 
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
