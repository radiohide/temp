// PropertyDlgWriteRTDB.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgWriteRTDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRTDB dialog


CPropertyDlgWriteRTDB::CPropertyDlgWriteRTDB(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgWriteRTDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgWriteRTDB)
	m_iPointStatus = 0;
	m_strTagName = _T("");
	m_strTagDesp = _T("");
	m_iPointType = -1;
	m_TagTimeOffsetOrInput = -1;//1:采用系统时间+偏移。0:采用外部偏移。
	m_lOffset = 0;
	m_BCheckPort = FALSE;
	//}}AFX_DATA_INIT
}


void CPropertyDlgWriteRTDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgWriteRTDB)
	DDX_Text(pDX, IDC_EDIT_STATUS, m_iPointStatus);
	DDX_Text(pDX, IDC_EDIT_TAG, m_strTagName);
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Radio(pDX, IDC_RADIO1, m_iPointType);
	DDX_Radio(pDX, IDC_RADIO3, m_TagTimeOffsetOrInput);
	DDX_Text(pDX, IDC_EDIT_OFFSET2, m_lOffset);
	DDX_Check(pDX, IDC_CHECK_ENABLE_PORT, m_BCheckPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgWriteRTDB, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgWriteRTDB)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRTDB message handlers

BOOL CPropertyDlgWriteRTDB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgWriteRTDB::OnRadio3() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(true);
	}
	
}

void CPropertyDlgWriteRTDB::OnRadio4() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET2)->EnableWindow(true);
	}
}