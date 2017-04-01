// DlgSetTimeAndSETime.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgSetTimeAndSETime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgSetTimeAndSETime dialog


DlgSetTimeAndSETime::DlgSetTimeAndSETime(CWnd* pParent /*=NULL*/)
	: CDialog(DlgSetTimeAndSETime::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgSetTimeAndSETime)
	m_iDutySOrE=1;//默认是值的结束时刻
	m_iDay=-1;
	m_bBeDuty=false;
	m_bBeMonth=false;
	m_Time = CTime::GetCurrentTime();
	m_EDTime = CTime::GetCurrentTime();
	m_EMTime = CTime::GetCurrentTime();
	m_EWTime = CTime::GetCurrentTime();
	m_SDTime = CTime::GetCurrentTime();
	m_SMTime = CTime::GetCurrentTime();
	m_SWTime = CTime::GetCurrentTime();
	m_lEDay = 0;
	m_lEDuty = 0;
	m_lEMonth = 0;
	m_lESecond = 0;
	m_lEWeek = 0;
	m_lSDay = 0;
	m_lSDuty = 0;
	m_lSMonth = 0;
	m_lSSecond = 0;
	m_lSWeek = 0;
	m_iSOffset_Type = -1;
	m_iEOffset_Type = -1;
	//}}AFX_DATA_INIT
}


void DlgSetTimeAndSETime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgSetTimeAndSETime)
	DDX_Control(pDX, IDC_COMBO_DUTYSORE, m_ComboDutySOrE);
	DDX_Control(pDX, IDC_COMBO_DUTYNUM, m_ComboDutyNum);
	DDX_Control(pDX, IDC_COMBO_DAY, m_ComboMonthDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_Time);
	DDX_Control(pDX, IDC_COMBO_EWEEK, m_ComboEWeek);
	DDX_Control(pDX, IDC_COMBO_EMONTH, m_ComboEMonth);
	DDX_Control(pDX, IDC_COMBO_EDUTY, m_ComboEDuty);
	DDX_Control(pDX, IDC_COMBO_SDUTY, m_ComboSDuty);
	DDX_Control(pDX, IDC_COMBO_SMONTH, m_ComBoSMonth);
	DDX_Control(pDX, IDC_COMBO_SWEEK, m_ComBoSWeek);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EDTIME, m_EDTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EMTIME, m_EMTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EWTIME, m_EWTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SDTIME, m_SDTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SMTIME, m_SMTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SWTIME, m_SWTime);
	DDX_Text(pDX, IDC_EDIT_EDAY, m_lEDay);
	DDX_Text(pDX, IDC_EDIT_EDUTY, m_lEDuty);
	DDX_Text(pDX, IDC_EDIT_EMONTH, m_lEMonth);
	DDX_Text(pDX, IDC_EDIT_ESECOND, m_lESecond);
	DDX_Text(pDX, IDC_EDIT_EWEEK, m_lEWeek);
	DDX_Text(pDX, IDC_EDIT_SDAY, m_lSDay);
	DDX_Text(pDX, IDC_EDIT_SDUTY, m_lSDuty);
	DDX_Text(pDX, IDC_EDIT_SMONTH, m_lSMonth);
	DDX_Text(pDX, IDC_EDIT_SSECOND, m_lSSecond);
	DDX_Text(pDX, IDC_EDIT_SWEEK, m_lSWeek);
	DDX_Radio(pDX, IDC_RADIO01, m_iSOffset_Type);
	DDX_Radio(pDX, IDC_RADIO11, m_iEOffset_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgSetTimeAndSETime, CDialog)
	//{{AFX_MSG_MAP(DlgSetTimeAndSETime)
	ON_BN_CLICKED(IDC_RADIO01, OnRadio01)
	ON_BN_CLICKED(IDC_RADIO02, OnRadio02)
	ON_BN_CLICKED(IDC_RADIO03, OnRadio03)
	ON_BN_CLICKED(IDC_RADIO04, OnRadio04)
	ON_BN_CLICKED(IDC_RADIO05, OnRadio05)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	ON_BN_CLICKED(IDC_RADIO14, OnRadio14)
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgSetTimeAndSETime message handlers
void DlgSetTimeAndSETime::SetTimeAndOffSetSETime(CString strTime,MCtrlOffsetSE *pMCtrlOffsetSE)
{
	this->m_strTime=strTime;
	m_MCtrlOffsetSE=*pMCtrlOffsetSE;
}
void DlgSetTimeAndSETime::SetMonthTheDay(int day)
{
	this->m_iDay=day;
}
void DlgSetTimeAndSETime::SetBeMonthStyle()
{
	m_bBeMonth=true;
}
void DlgSetTimeAndSETime::SetBeDutyStyle()
{
	m_bBeDuty=true;
}
void DlgSetTimeAndSETime::SetAllDuty(CStringArray &m_ArrAllDuty)
{
	this->m_ArrAllDuty.Copy(m_ArrAllDuty);
}
void DlgSetTimeAndSETime::SetDutyTimeData(CString strDutyName,int dutySOrE)
{
	m_strDutyName=strDutyName;
	m_iDutySOrE=dutySOrE;
}
BOOL DlgSetTimeAndSETime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(m_iDay==-1)
	{
		this->m_ComboMonthDay.SetCurSel(0);//当模式为月时，默认为每月1号
	}
	else
		this->m_ComboMonthDay.SetCurSel(m_iDay);//当模式为月时，默认为每月1号

	this->m_ComboEDuty.SetCurSel(0);
	this->m_ComboEMonth.SetCurSel(0);
	this->m_ComboEWeek.SetCurSel(0);
	this->m_ComboSDuty.SetCurSel(0);
	this->m_ComBoSMonth.SetCurSel(0);
	this->m_ComBoSWeek.SetCurSel(0);
	// TODO: Add extra initialization here
	if(m_strTime!="")
	{
		int   nHour,   nMin,   nSec;   
		sscanf(m_strTime,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
		CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
		this->m_Time=t;
	}
	else
		this->m_Time=CTime::GetCurrentTime();
	this->m_iSOffset_Type=m_MCtrlOffsetSE.m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
	this->m_iEOffset_Type=m_MCtrlOffsetSE.m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
	
	if(!m_bBeMonth)
	{
		GetDlgItem(IDC_STATIC_MDAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HAO)->ShowWindow(SW_HIDE);
	}
	if(!m_bBeDuty)
	{
		GetDlgItem(IDC_STATIC_DUTYNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DUTYNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DUTYSORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DUTYSORE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME)->ShowWindow(SW_HIDE);
		
		CRect rc;
		GetDlgItem(IDC_STATIC_TIME)->GetWindowRect(rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_STATIC_DUTYNUM)->MoveWindow(rc);

		GetDlgItem(IDC_DATETIMEPICKER_TIME)->GetWindowRect(rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_COMBO_DUTYNUM)->MoveWindow(rc);
		
		GetDlgItem(IDC_STATIC_MDAY)->GetWindowRect(rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_STATIC_DUTYSORE)->MoveWindow(rc);

		GetDlgItem(IDC_COMBO_DAY)->GetWindowRect(rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_COMBO_DUTYSORE)->MoveWindow(rc);
		
		for(int i=0;i<this->m_ArrAllDuty.GetSize();i++)
		{
			this->m_ComboDutyNum.AddString(m_ArrAllDuty[i]);
		}
		if(m_strDutyName!="")//"名称格式为：duty1"
		{
			int dutyNum=atoi(m_strDutyName.Right(1));
			m_ComboDutyNum.SetCurSel(dutyNum-1);
		}
		else
			m_ComboDutyNum.SetCurSel(0);
		this->m_ComboDutySOrE.SetCurSel(m_iDutySOrE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void DlgSetTimeAndSETime::enbaleWindowBySOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE)
{
	if(pMCtrlOffsetSE->m_iSOffset_Type==0)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SDUTY)->EnableWindow(false);
		
		this->m_lSSecond=pMCtrlOffsetSE->m_iSOffset_Value1;
	}
	else if(pMCtrlOffsetSE->m_iSOffset_Type==1)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SDUTY)->EnableWindow(false);

		this->m_lSDay=pMCtrlOffsetSE->m_iSOffset_Value1;
		if(pMCtrlOffsetSE->m_strS_Time!="")
		{
			int   nHour,   nMin,   nSec;   
			sscanf(pMCtrlOffsetSE->m_strS_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_SDTime=t;
		}
		else
			this->m_SDTime=CTime::GetCurrentTime();
	}
	else if(pMCtrlOffsetSE->m_iSOffset_Type==2)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SWEEK)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SWEEK)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_SWTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SDUTY)->EnableWindow(false);
		this->m_lSWeek=pMCtrlOffsetSE->m_iSOffset_Value1;
		this->m_ComBoSWeek.SetCurSel(pMCtrlOffsetSE->m_iSOffset_Value2);
		if(pMCtrlOffsetSE->m_strS_Time!="")
		{
			int   nHour,   nMin,   nSec;   
			sscanf(pMCtrlOffsetSE->m_strS_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_SWTime=t;
		}
		else
			this->m_SWTime=CTime::GetCurrentTime();
	}
	else if(pMCtrlOffsetSE->m_iSOffset_Type==3)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_SDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SDUTY)->EnableWindow(false);

		this->m_lSMonth=pMCtrlOffsetSE->m_iSOffset_Value1;
		this->m_ComBoSWeek.SetCurSel(pMCtrlOffsetSE->m_iSOffset_Value2);
		if(pMCtrlOffsetSE->m_strS_Time!="")
		{
			int   nHour,   nMin,   nSec;   
			sscanf(pMCtrlOffsetSE->m_strS_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_SMTime=t;
		}
		else
			this->m_SMTime=CTime::GetCurrentTime();
	}
	else if(pMCtrlOffsetSE->m_iSOffset_Type==4)
	{
		GetDlgItem(IDC_EDIT_SSECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_SMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SDUTY)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SDUTY)->EnableWindow(true);

		this->m_lSDuty=pMCtrlOffsetSE->m_iSOffset_Value1;
		this->m_ComboSDuty.SetCurSel(pMCtrlOffsetSE->m_iSOffset_Value2);
	}
	UpdateData(false);
}
void DlgSetTimeAndSETime::enbaleWindowByEOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE)
{
	if(pMCtrlOffsetSE->m_iEOffset_Type==0)
	{
		GetDlgItem(IDC_EDIT_ESECOND)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_EDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EDUTY)->EnableWindow(false);
		
		this->m_lESecond=pMCtrlOffsetSE->m_iEOffset_Value1;
	}
	else if(pMCtrlOffsetSE->m_iEOffset_Type==1)
	{
		GetDlgItem(IDC_EDIT_ESECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDAY)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_EDTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EDUTY)->EnableWindow(false);
		
		this->m_lEDay=pMCtrlOffsetSE->m_iEOffset_Value1;
		int   nHour,   nMin,   nSec; 
		if(pMCtrlOffsetSE->m_strE_Time=="")
			this->m_EDTime=CTime::GetCurrentTime();
		else
		{
			sscanf(pMCtrlOffsetSE->m_strE_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_EDTime=t;
		}
	}
	else if(pMCtrlOffsetSE->m_iEOffset_Type==2)
	{
		GetDlgItem(IDC_EDIT_ESECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EWEEK)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_EWEEK)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_EWTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EDUTY)->EnableWindow(false);
		
		this->m_lEWeek=pMCtrlOffsetSE->m_iEOffset_Value1;
		this->m_ComboEWeek.SetCurSel(pMCtrlOffsetSE->m_iEOffset_Value2);
		if(pMCtrlOffsetSE->m_strE_Time=="")
			this->m_EWTime=CTime::GetCurrentTime();
		else
		{
			int   nHour,   nMin,   nSec;   
			sscanf(pMCtrlOffsetSE->m_strE_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_EWTime=t;
		}
	}
	else if(pMCtrlOffsetSE->m_iEOffset_Type==3)
	{
		GetDlgItem(IDC_EDIT_ESECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EMONTH)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_EMONTH)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_EMTIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_EDUTY)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EDUTY)->EnableWindow(false);
		
		this->m_lEMonth=pMCtrlOffsetSE->m_iEOffset_Value1;
		this->m_ComboEMonth.SetCurSel(pMCtrlOffsetSE->m_iEOffset_Value2);
		if(pMCtrlOffsetSE->m_strE_Time!="")
		{
			int   nHour,   nMin,   nSec;   
			sscanf(pMCtrlOffsetSE->m_strE_Time,"%d:%d:%d",&nHour,   &nMin,   &nSec);   
			CTime   t(CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),nHour,   nMin,   nSec);
			this->m_EMTime=t;
		}
		else
		{
			this->m_EMTime=CTime::GetCurrentTime();
		}
	}
	else if(pMCtrlOffsetSE->m_iEOffset_Type==4)
	{
		GetDlgItem(IDC_EDIT_ESECOND)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDAY)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EDTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EWEEK)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EWTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_EMONTH)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_EMTIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_EDUTY)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_EDUTY)->EnableWindow(true);
		
		this->m_lEDuty=pMCtrlOffsetSE->m_iEOffset_Value1;
		this->m_ComboEDuty.SetCurSel(pMCtrlOffsetSE->m_iEOffset_Value2);
	}
	UpdateData(false);
}
void DlgSetTimeAndSETime::setCtrlData()
{
	UpdateData(true);

	m_strDutyName.Format("duty%d",this->m_ComboDutyNum.GetCurSel()+1);
	m_iDutySOrE=this->m_ComboDutySOrE.GetCurSel();

	this->m_strTime=this->m_Time.Format("%H:%M:%S");
	this->m_iDay=this->m_ComboMonthDay.GetCurSel();

	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	if(this->m_iSOffset_Type==0)
	{
		m_MCtrlOffsetSE.m_iSOffset_Value1=this->m_lSSecond;
		m_MCtrlOffsetSE.m_strS_Time="";
	}
	else if(this->m_iSOffset_Type==1)
	{
		m_MCtrlOffsetSE.m_iSOffset_Value1=this->m_lSDay;
		m_MCtrlOffsetSE.m_strS_Time=this->m_SDTime.Format("%H:%M:%S");
	}
	else if(this->m_iSOffset_Type==2)
	{
		m_MCtrlOffsetSE.m_iSOffset_Value1=this->m_lSWeek;
		m_MCtrlOffsetSE.m_iSOffset_Value2=this->m_ComBoSWeek.GetCurSel();
		m_MCtrlOffsetSE.m_strS_Time=this->m_SWTime.Format("%H:%M:%S");
	}
	else if(this->m_iSOffset_Type==3)
	{
		m_MCtrlOffsetSE.m_iSOffset_Value1=this->m_lSMonth;
		m_MCtrlOffsetSE.m_iSOffset_Value2=this->m_ComBoSMonth.GetCurSel();
		m_MCtrlOffsetSE.m_strS_Time=this->m_SMTime.Format("%H:%M:%S");
	}
	else if(this->m_iSOffset_Type==4)
	{
		m_MCtrlOffsetSE.m_iSOffset_Value1=this->m_lSDuty;
		m_MCtrlOffsetSE.m_iSOffset_Value2=this->m_ComboSDuty.GetCurSel();
		m_MCtrlOffsetSE.m_strS_Time="";
	}
	if(this->m_iEOffset_Type==0)
	{
		m_MCtrlOffsetSE.m_iEOffset_Value1=this->m_lESecond;
		m_MCtrlOffsetSE.m_strE_Time="";
	}
	else if(this->m_iEOffset_Type==1)
	{
		m_MCtrlOffsetSE.m_iEOffset_Value1=this->m_lEDay;
		m_MCtrlOffsetSE.m_strE_Time=this->m_EDTime.Format("%H:%M:%S");
	}
	else if(this->m_iEOffset_Type==2)
	{
		m_MCtrlOffsetSE.m_iEOffset_Value1=this->m_lEWeek;
		m_MCtrlOffsetSE.m_iEOffset_Value2=this->m_ComboEWeek.GetCurSel();
		m_MCtrlOffsetSE.m_strE_Time=this->m_EWTime.Format("%H:%M:%S");
	}
	else if(this->m_iEOffset_Type==3)
	{
		m_MCtrlOffsetSE.m_iEOffset_Value1=this->m_lEMonth;
		m_MCtrlOffsetSE.m_iEOffset_Value2=this->m_ComboEMonth.GetCurSel();
		m_MCtrlOffsetSE.m_strE_Time=this->m_EMTime.Format("%H:%M:%S");
	}
	else if(this->m_iEOffset_Type==4)
	{
		m_MCtrlOffsetSE.m_iEOffset_Value1=this->m_lEDuty;
		m_MCtrlOffsetSE.m_iEOffset_Value2=this->m_ComboEDuty.GetCurSel();
		m_MCtrlOffsetSE.m_strE_Time="";
	}
}
void DlgSetTimeAndSETime::OnRadio01() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio02() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio03() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio04() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio05() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iSOffset_Type=this->m_iSOffset_Type;
	enbaleWindowBySOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio11() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio12() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio13() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio14() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
}
void DlgSetTimeAndSETime::OnRadio15() 
{
	UpdateData(true);
	m_MCtrlOffsetSE.m_iEOffset_Type=this->m_iEOffset_Type;
	enbaleWindowByEOffsetType(&m_MCtrlOffsetSE);
}

void DlgSetTimeAndSETime::OnOK() 
{
	// TODO: Add extra validation here
	setCtrlData();
	CDialog::OnOK();
}
