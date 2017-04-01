// DlgLocalRunSetting.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgLocalRunSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalRunSetting dialog


CDlgLocalRunSetting::CDlgLocalRunSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLocalRunSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLocalRunSetting)
	m_nCalcMode = -1;
	m_EndDate = 0;
	m_EndTime = 0;
	m_StartDate = 0;
	m_StartTime = 0;
	//}}AFX_DATA_INIT
}
void CDlgLocalRunSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLocalRunSetting)
	DDX_Control(pDX, IDC_COMBO_INTERVAL, m_combInterval);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STARTTIME, m_CtrlStartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STARTDATE, m_CtrlStartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ENDTTIME, m_CtrlEndTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ENDDATE, m_CtrlEndDate);
	DDX_Control(pDX, IDC_COMBO_CYCEL, m_combCycleT);
	DDX_CBIndex(pDX, IDC_COMBO_MODE, m_nCalcMode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDDATE, m_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDTTIME, m_EndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTDATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTTIME, m_StartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLocalRunSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgLocalRunSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_CYCEL, OnSelchangeComboCycel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_CBN_EDITCHANGE(IDC_COMBO_INTERVAL, OnEditchangeComboInterval)
	ON_CBN_EDITCHANGE(IDC_COMBO_CYCEL, OnEditchangeComboCycel)
	ON_BN_CLICKED(IDC_BUTTON_ALLONETIME, OnButtonAllonetime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalRunSetting message handlers

void CDlgLocalRunSetting::OnSelchangeComboCycel() 
{
	// TODO: Add your control notification handler code here

}

BOOL CDlgLocalRunSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
    CString str;
	str.Format("%d",m_nCycleT);
	m_combCycleT.SetWindowText(str);

	str.Format("%d",m_nInterval);
	m_combInterval.SetWindowText(str);

	OnSelchangeComboMode() ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLocalRunSetting::OnSelchangeComboMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	switch(m_nCalcMode)
	{
	case 0://
		{
			m_combCycleT.EnableWindow(TRUE);
			
			m_CtrlStartDate.EnableWindow(FALSE);
			m_CtrlStartTime.EnableWindow(FALSE);
			m_CtrlEndDate.EnableWindow(FALSE);
			m_CtrlEndTime.EnableWindow(FALSE);

			m_combInterval.EnableWindow(FALSE);
			break;
		}
	case 1:
		{
			m_combCycleT.EnableWindow(TRUE);
			
			m_CtrlStartDate.EnableWindow(TRUE);
			m_CtrlStartTime.EnableWindow(TRUE);
			m_CtrlEndDate.EnableWindow(TRUE);
			m_CtrlEndTime.EnableWindow(TRUE);
			m_combInterval.EnableWindow(TRUE);
			break;
		}
	case 2:
		{
			m_combCycleT.EnableWindow(FALSE);
			
			m_CtrlStartDate.EnableWindow(TRUE);
			m_CtrlStartTime.EnableWindow(TRUE);
			m_CtrlEndDate.EnableWindow(TRUE);
			m_CtrlEndTime.EnableWindow(TRUE);
			m_combInterval.EnableWindow(TRUE);
			break;
		}
	}
}

void CDlgLocalRunSetting::OnOK() 
{
	// TODO: Add extra validation here
	if(UpdateData()==FALSE) return;
	//------------------------------------------------
	CString str;
    m_combCycleT.GetWindowText(str);
	int aa = atoi(str);
	if(aa<=0) 
	{
		AfxMessageBox("定时周期为非数字不合法");
		return;
	}
	else  m_nCycleT =aa;
	//----------------------------------------------
	 m_combInterval.GetWindowText(str);
	aa = atoi(str);
	if(aa<=0) 
	{
		AfxMessageBox("时间间隔为非数字不合法");
		return;
	}
	else  m_nInterval =aa;

	CTime tm1 = CTime(m_StartDate.GetYear(),m_StartDate.GetMonth(),m_StartDate.GetDay(),m_StartTime.GetHour(),m_StartTime.GetMinute(),m_StartTime.GetSecond());
	CTime tm2 = CTime(m_EndDate.GetYear(),m_EndDate.GetMonth(),m_EndDate.GetDay(),m_EndTime.GetHour(),m_EndTime.GetMinute(),m_EndTime.GetSecond());
	if(tm2-tm1<m_nInterval)
	{
		AfxMessageBox("开始时间与结束时间小于时间间隔！");
		return;
	}

	CDialog::OnOK();
}

void CDlgLocalRunSetting::OnEditchangeComboInterval() 
{
	// TODO: Add your control notification handler code here
	CString str;
    m_combInterval.GetWindowText(str);
	int aa = atoi(str);
	if(aa<=0) 
	{
		AfxMessageBox("周期为非数字不合法");
		return;
	}
	else  m_nInterval =aa;
	
}

void CDlgLocalRunSetting::OnEditchangeComboCycel() 
{
	CString str;
    m_combCycleT.GetWindowText(str);
	int aa = atoi(str);
	if(aa<=0) 
	{
		AfxMessageBox("周期为非数字不合法");
		return;
	}
	else  m_nCycleT =aa;	
}
//以整段时间为间隔
void CDlgLocalRunSetting::OnButtonAllonetime() 
{
	// TODO: Add your control notification handler code here
	if(UpdateData()==FALSE) return;
	CTime tm1 = CTime(m_StartDate.GetYear(),m_StartDate.GetMonth(),m_StartDate.GetDay(),m_StartTime.GetHour(),m_StartTime.GetMinute(),m_StartTime.GetSecond());
	CTime tm2 = CTime(m_EndDate.GetYear(),m_EndDate.GetMonth(),m_EndDate.GetDay(),m_EndTime.GetHour(),m_EndTime.GetMinute(),m_EndTime.GetSecond());
	if(tm2-tm1<m_nInterval)
	{
		AfxMessageBox("开始时间与结束时间小于时间间隔！");
		return;
	}
	CString str;
	str.Format("%d",tm2.GetTime()-tm1.GetTime());
	m_combInterval.SetWindowText(str);
}
