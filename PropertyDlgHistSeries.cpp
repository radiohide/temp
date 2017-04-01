// PropertyDlgHistSeries.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgHistSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeries dialog


CPropertyDlgHistSeries::CPropertyDlgHistSeries(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgHistSeries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgHistSeries)
	m_strPointName = _T("");
	m_iDataSource = -1;
	m_iReadModel = -1;
	m_ISnapStep = 5;
	//}}AFX_DATA_INIT
}


void CPropertyDlgHistSeries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgHistSeries)
	DDX_Text(pDX, IDC_EDIT1, m_strPointName);
	DDX_Radio(pDX, IDC_RADIO1, m_iDataSource);
	DDX_Radio(pDX, IDC_RADIO7, m_iReadModel);
	DDX_Text(pDX, IDC_EDIT_SNAPSTEP, m_ISnapStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgHistSeries, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgHistSeries)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeries message handlers

BOOL CPropertyDlgHistSeries::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	if(m_iReadModel==0)
	{
		GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(FALSE);
	}
	else if(m_iReadModel==1)
	{
		GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgHistSeries::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_iReadModel=0;
	GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(FALSE);
}

void CPropertyDlgHistSeries::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_iReadModel=1;
	GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(TRUE);	
}
