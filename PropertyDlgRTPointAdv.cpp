// PropertyDlgRTPointAdv.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRTPointAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPointAdv dialog


CPropertyDlgRTPointAdv::CPropertyDlgRTPointAdv(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgRTPointAdv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgRTPointAdv)
	m_strTagDesp = _T("");
	m_strTag1 = _T("");
	m_strTag2 = _T("");
	m_strTag3 = _T("");
	m_strTag4 = _T("");
	m_strTag5 = _T("");	
	m_iOutMethod = -1;
	m_bChkLimit = FALSE;
	m_dHighLimit = 0.0;
	m_dLowLimit = 0.0;
	m_dConstWhenBad = 0.0;
	m_bForceEnable = FALSE;
	m_dForceValue = 0.0;
	m_lOffset = 0;
	m_iInOrOut = -1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgRTPointAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgRTPointAdv)
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Text(pDX, IDC_EDIT_TAG1, m_strTag1);
	DDX_Text(pDX, IDC_EDIT_TAG2, m_strTag2);
	DDX_Text(pDX, IDC_EDIT_TAG3, m_strTag3);
	DDX_Text(pDX, IDC_EDIT_TAG4, m_strTag4);
	DDX_Text(pDX, IDC_EDIT_TAG5, m_strTag5);
	DDX_CBIndex(pDX, IDC_COMBO_OUTMETHOD, m_iOutMethod);
	DDX_Check(pDX, IDC_CHECK_LIMIT, m_bChkLimit);
	DDX_Text(pDX, IDC_EDIT_HIGHLIMIT, m_dHighLimit);
	DDX_Text(pDX, IDC_EDIT_LOWLIMIT, m_dLowLimit);
	DDX_Text(pDX, IDC_EDIT_CONSTWHENBAD, m_dConstWhenBad);
	DDX_Check(pDX, IDC_CHECK_FORCEOUTPUT, m_bForceEnable);
	DDX_Text(pDX, IDC_EDIT_FORCEVALUE, m_dForceValue);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Radio(pDX, IDC_RADIO1, m_iInOrOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgRTPointAdv, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgRTPointAdv)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPointAdv message handlers
void CPropertyDlgRTPointAdv::OnRadio1()
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
}

BOOL CPropertyDlgRTPointAdv::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgRTPointAdv::OnRadio2() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
}