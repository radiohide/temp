// PropertyDlgCoal.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgCoal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoal dialog


CPropertyDlgCoal::CPropertyDlgCoal(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgCoal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgCoal)
	m_dMf = 0.0;
	m_bFixMf = FALSE;
	m_iH = -1;
	m_iLwCoalAr = -1;
	m_iMs = -1;
	m_iS = -1;
	m_iAshAr = -1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgCoal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgCoal)
	DDX_Text(pDX, IDC_EDIT_MF, m_dMf);
	DDX_Check(pDX, IDC_CHECK_FIXMF, m_bFixMf);
	DDX_Radio(pDX, IDC_RADIOH1, m_iH);
	DDX_Radio(pDX, IDC_RADIOLwCoalAr1, m_iLwCoalAr);
	DDX_Radio(pDX, IDC_RADIOMS1, m_iMs);
	DDX_Radio(pDX, IDC_RADIOS1, m_iS);
	DDX_Radio(pDX, IDC_RADIOAshAr1, m_iAshAr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgCoal, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgCoal)
	ON_BN_CLICKED(IDC_CHECK_FIXMF, OnCheckFixmf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoal message handlers

void CPropertyDlgCoal::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CPropertyDlgCoal::OnCheckFixmf() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(this->m_bFixMf)
		GetDlgItem(IDC_EDIT_MF)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_MF)->EnableWindow(false);
}

BOOL CPropertyDlgCoal::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_bFixMf)
		GetDlgItem(IDC_EDIT_MF)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_MF)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
