// DlgCfgSETime.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgCfgSETime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgSETime dialog


CDlgCfgSETime::CDlgCfgSETime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgSETime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgSETime)
	m_EndDate = 0;
	m_EndTime = 0;
	m_StartDate = 0;
	m_StartTime = 0;
	//}}AFX_DATA_INIT
}


void CDlgCfgSETime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgSETime)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EDATE, m_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ETIME, m_EndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgSETime, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgSETime)
	ON_BN_CLICKED(IDC_CHECK_ETIME, OnCheckEtime)
	ON_BN_CLICKED(IDC_RADIO_CURRENT, OnRadioCurrent)
	ON_BN_CLICKED(IDC_RADIO_SE, OnRadioSe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgSETime message handlers
void CDlgCfgSETime::SetSETime(CTime sTime,CTime eTime)
{
	m_inStartTime=sTime;
	m_inEndTime=eTime;
}

BOOL CDlgCfgSETime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_inStartTime.GetTime()==0)
	{
		m_StartDate=CTime(0);
		m_StartTime=CTime(0);

		m_EndDate=CTime(0);
		m_EndTime=CTime(0);
		GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_ETIME)->EnableWindow(false);

		
		((CButton*)GetDlgItem(IDC_CHECK_ETIME))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(true);
		m_StartDate=m_inStartTime;
		m_StartTime=m_inStartTime;
		if(m_inEndTime.GetTime()==0)
		{
			m_EndDate=CTime(0);
			m_EndTime=CTime(0);
			GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(false);
			GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_CHECK_ETIME))->SetCheck(false);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_ETIME))->SetCheck(true);
			m_EndTime=m_inEndTime;
			m_EndDate=m_inEndTime;
		}
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCfgSETime::OnCheckEtime() 
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_CHECK_ETIME))->GetCheck())
	{
		GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(true);
		m_EndDate=m_StartDate+CTimeSpan( 1, 0, 0, 0 );
		m_EndTime=m_StartDate+CTimeSpan( 1, 0, 0, 0 );
	}
	else
	{
		m_EndDate=CTime(0);
		m_EndTime=CTime(0);
		GetDlgItem(IDC_DATETIMEPICKER_EDATE)->SetWindowText("");
		GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_ETIME)->SetWindowText("");
		GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_CHECK_ETIME))->SetCheck(false);
	}
	UpdateData(false);
}

CTime CDlgCfgSETime::GetStartTime()
{
	return CTime(m_StartDate.GetYear(),m_StartDate.GetMonth(),m_StartDate.GetDay(),m_StartTime.GetHour(),m_StartTime.GetMinute(),m_StartTime.GetSecond());
}

CTime CDlgCfgSETime::GetEndTime()
{
	return CTime(m_EndDate.GetYear(),m_EndDate.GetMonth(),m_EndDate.GetDay(),m_EndTime.GetHour(),m_EndTime.GetMinute(),m_EndTime.GetSecond());
}

void CDlgCfgSETime::OnRadioCurrent() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(false);
	GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_ETIME)->EnableWindow(false);
	m_StartDate=CTime(0);
	m_StartTime=CTime(0);
	
	m_EndDate=CTime(0);
	m_EndTime=CTime(0);
	UpdateData(false);
}

void CDlgCfgSETime::OnRadioSe() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(true);
	GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_ETIME)->EnableWindow(true);

	m_StartDate=CTime::GetCurrentTime();
	m_StartTime=CTime::GetCurrentTime();
	
	m_EndDate=CTime(0);
	m_EndTime=CTime(0);
	GetDlgItem(IDC_DATETIMEPICKER_EDATE)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_ETIME)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_ETIME))->SetCheck(false);
	UpdateData(false);
}
