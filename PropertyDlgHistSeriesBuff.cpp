// PropertyDlgHistSeriesBuff.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgHistSeriesBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeriesBuff dialog


CPropertyDlgHistSeriesBuff::CPropertyDlgHistSeriesBuff(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgHistSeriesBuff::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgHistSeriesBuff)
	m_ISnapStep = 5;
	m_iDataSource = -1;
	m_iReadModel = -1;
	m_strPointName = _T("");
	m_iOutputTime = 300;
	m_iBuffTime = 600;
	//}}AFX_DATA_INIT
}


void CPropertyDlgHistSeriesBuff::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgHistSeriesBuff)
	DDX_Text(pDX, IDC_EDIT_SNAPSTEP, m_ISnapStep);
	DDX_Radio(pDX, IDC_RADIO1, m_iDataSource);
	DDX_Radio(pDX, IDC_RADIO7, m_iReadModel);
	DDX_Text(pDX, IDC_EDIT_POINTNAME, m_strPointName);
	DDX_Text(pDX, IDC_EDIT_OUTPUTTIME, m_iOutputTime);
	DDX_Text(pDX, IDC_EDIT_BUFFTIME, m_iBuffTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgHistSeriesBuff, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgHistSeriesBuff)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgHistSeriesBuff message handlers

BOOL CPropertyDlgHistSeriesBuff::OnInitDialog() 
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

void CPropertyDlgHistSeriesBuff::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_iReadModel=0;
	GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(FALSE);
}

void CPropertyDlgHistSeriesBuff::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_iReadModel=1;
	GetDlgItem(IDC_EDIT_SNAPSTEP)->EnableWindow(TRUE);
}

void CPropertyDlgHistSeriesBuff::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_iOutputTime>m_iBuffTime)
	{
		AfxMessageBox("输出时间长度不得大于缓冲时间长度");
		return;
	}
	CDialog::OnOK();
}
