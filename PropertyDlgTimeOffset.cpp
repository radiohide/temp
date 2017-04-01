// PropertyDlgTimeOffset.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTimeOffset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTimeOffset dialog


CPropertyDlgTimeOffset::CPropertyDlgTimeOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTimeOffset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTimeOffset)
	m_iOffsetType = 1;
	m_iSecond = 0;
	m_iDay = 0;
	m_iMonth = 1;
	m_iMonthDay = 0;
	m_DTime = CTime(2012,1,1,0,0,0);
	m_MTime = CTime(2012,1,1,0,0,0);
	m_iWeekDay = -1;
	m_iYearDay = -1;
	m_iYearMonth = -1;
	m_HTime = 0;
	m_WTime = 0;
	m_YTime = 0;
	m_iYear = 0;
	m_iWeek = 0;
	m_iHour = 0;
	
	//}}AFX_DATA_INIT
}


void CPropertyDlgTimeOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTimeOffset)
	DDX_Radio(pDX, IDC_RADIO01, m_iOffsetType);
	DDX_Text(pDX, IDC_EDIT_SSECOND, m_iSecond);
	DDX_Text(pDX, IDC_EDIT_SDAY, m_iDay);
	DDX_Text(pDX, IDC_EDIT_SMONTH, m_iMonth);
	DDX_CBIndex(pDX, IDC_COMBO_SMONTH, m_iMonthDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SDTIME, m_DTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SMTIME, m_MTime);
	DDX_CBIndex(pDX, IDC_COMBO_SWEEK_DAY, m_iWeekDay);
	DDX_CBIndex(pDX, IDC_COMBO_SYEAR_D, m_iYearDay);
	DDX_CBIndex(pDX, IDC_COMBO_SYEAR_M, m_iYearMonth);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SHTIME, m_HTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SWTIME, m_WTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SYTIME, m_YTime);
	DDX_Text(pDX, IDC_EDIT_SYEAR, m_iYear);
	DDX_Text(pDX, IDC_EDIT_WEEK, m_iWeek);
	DDX_Text(pDX, IDC_EDIT_SHOUR, m_iHour);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTimeOffset, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTimeOffset)
	ON_BN_CLICKED(IDC_RADIO01, OnRadio01)
	ON_BN_CLICKED(IDC_RADIO02, OnRadio02)
	ON_BN_CLICKED(IDC_RADIO04, OnRadio04)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTimeOffset message handlers

BOOL CPropertyDlgTimeOffset::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//initUI();
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgTimeOffset::initUI() 
{
	if(m_iOffsetType==0)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(true);

		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
	}
	else if(m_iOffsetType==1)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(true);

		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
	}
	else if(m_iOffsetType==2)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(true);
	}
}

void CPropertyDlgTimeOffset::OnRadio01() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//initUI();
}

void CPropertyDlgTimeOffset::OnRadio02() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//initUI();
}

void CPropertyDlgTimeOffset::OnRadio04() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//initUI();
}
