// PropertyDlgTime.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTime dialog


CPropertyDlgTime::CPropertyDlgTime(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTime)
	m_Date = 0;
	m_Time = 0;
	m_iTheTimeOrCurrent = -1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTime)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATA, m_Date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_Time);
	DDX_Radio(pDX, IDC_RADIO1, m_iTheTimeOrCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTime, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTime)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTime message handlers

BOOL CPropertyDlgTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_strDateTime!="")
	{
		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;    
		sscanf(m_strDateTime, "%d-%d-%d %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		CTime tDate(nYear,   nMonth,   nDate,9,0,0);
		m_Date=tDate;
		CTime tTime(2011,1,1,nHour,nMin,nSec);
		m_Time=tTime;
	}
	else
	{
		m_Date=CTime::GetCurrentTime();
		m_Time=CTime::GetCurrentTime();
	}
	if(m_iTheTimeOrCurrent==0)
	{
		GetDlgItem(IDC_DATETIMEPICKER_DATA)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_TIME)->EnableWindow(false);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgTime::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	if(m_iTheTimeOrCurrent!=0)//指定时刻
	{
		CTime time(m_Date.GetYear(),m_Date.GetMonth(),m_Date.GetDay(),m_Time.GetHour(),m_Time.GetMinute(),m_Time.GetSecond());
		m_strDateTime=time.Format("%Y-%m-%d %H:%M:%S");
	}
	else//取当前时间置空字符串
		m_strDateTime="";
	CDialog::OnOK();
}

void CPropertyDlgTime::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_DATETIMEPICKER_DATA)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_TIME)->EnableWindow(false);
}

void CPropertyDlgTime::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_DATETIMEPICKER_DATA)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER_TIME)->EnableWindow(true);
}
