// DlgTaskSUpTime.cpp : implementation file
//

#include "stdafx.h"
#include "..\\eperfmanage.h"
#include "DlgTaskSUpTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskSUpTime dialog


CDlgTaskSUpTime::CDlgTaskSUpTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTaskSUpTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTaskSUpTime)
	m_StartDate = 0;
	m_StartTime = 0;
	//}}AFX_DATA_INIT
}


void CDlgTaskSUpTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTaskSUpTime)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTaskSUpTime, CDialog)
	//{{AFX_MSG_MAP(CDlgTaskSUpTime)
	ON_BN_CLICKED(IDC_RADIO_CURRENT, OnRadioCurrent)
	ON_BN_CLICKED(IDC_RADIO_SE, OnRadioSe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskSUpTime message handlers
void CDlgTaskSUpTime::SetSTime(CTime sTime)
{
	m_inStartTime=sTime;
}
BOOL CDlgTaskSUpTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_inStartTime.GetTime()==0)
	{
		m_StartDate=CTime(0);
		m_StartTime=CTime(0);

		GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(false);
		
		((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(true);
		m_StartDate=m_inStartTime;
		m_StartTime=m_inStartTime;
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
CTime CDlgTaskSUpTime::GetStartTime()
{
	return CTime(m_StartDate.GetYear(),m_StartDate.GetMonth(),m_StartDate.GetDay(),m_StartTime.GetHour(),m_StartTime.GetMinute(),m_StartTime.GetSecond());
}
void CDlgTaskSUpTime::OnRadioCurrent() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(false);
	GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(false);

	m_StartDate=CTime(0);
	m_StartTime=CTime(0);
	UpdateData(false);
}

void CDlgTaskSUpTime::OnRadioSe() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_CURRENT))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_SE))->SetCheck(true);
	GetDlgItem(IDC_DATETIMEPICKER_SDATE)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER_STIME)->EnableWindow(true);

	m_StartDate=CTime::GetCurrentTime();
	m_StartTime=CTime::GetCurrentTime();
	UpdateData(false);
}