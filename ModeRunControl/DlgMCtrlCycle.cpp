// DlgMCtrlCycle.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlCycle dialog


CDlgMCtrlCycle::CDlgMCtrlCycle(CWnd* pParent /*=NULL*/)
	: CDlgMCtrlType(CDlgMCtrlCycle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMCtrlCycle)
	m_strTypeName="定时计划任务";
	m_iUnit = 0;
	m_lCycleLength = 5;
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


void CDlgMCtrlCycle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMCtrlCycle)
	DDX_Control(pDX, IDC_COMBO_EWEEK, m_ComboEWeek);
	DDX_Control(pDX, IDC_COMBO_EMONTH, m_ComboEMonth);
	DDX_Control(pDX, IDC_COMBO_EDUTY, m_ComboEDuty);
	DDX_Control(pDX, IDC_COMBO_SDUTY, m_ComboSDuty);
	DDX_Control(pDX, IDC_COMBO_SMONTH, m_ComBoSMonth);
	DDX_Control(pDX, IDC_COMBO_SWEEK, m_ComBoSWeek);
	DDX_CBIndex(pDX, IDC_COMBO_UNIT, m_iUnit);
	DDX_Text(pDX, IDC_EDIT_CYCLELENGTH, m_lCycleLength);
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


BEGIN_MESSAGE_MAP(CDlgMCtrlCycle, CDialog)
	//{{AFX_MSG_MAP(CDlgMCtrlCycle)
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
// CDlgMCtrlCycle message handlers

BOOL CDlgMCtrlCycle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->m_ComboEDuty.SetCurSel(0);
	this->m_ComboEMonth.SetCurSel(0);
	this->m_ComboEWeek.SetCurSel(0);
	this->m_ComboSDuty.SetCurSel(0);
	this->m_ComBoSMonth.SetCurSel(0);
	this->m_ComBoSWeek.SetCurSel(0);
	// TODO: Add extra initialization here
	this->m_iUnit=((MCtrlCycle*)m_pMCtrlType)->m_iUnit;
	this->m_lCycleLength=((MCtrlCycle*)m_pMCtrlType)->m_lCycle_Length;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()==0)
	{
		pMCtrlOffsetSE=new MCtrlOffsetSE();
		((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
	}
	initOffsetSETime();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMCtrlCycle::ShowCtrlData()
{
	this->m_iUnit=((MCtrlCycle*)m_pMCtrlType)->m_iUnit;
	this->m_lCycleLength=((MCtrlCycle*)m_pMCtrlType)->m_lCycle_Length;
	initOffsetSETime();
	UpdateData(false);
}

void CDlgMCtrlCycle::SetCtrlData()
{
	UpdateData(true);
	((MCtrlCycle*)m_pMCtrlType)->m_iUnit=this->m_iUnit;
	((MCtrlCycle*)m_pMCtrlType)->m_lCycle_Length=this->m_lCycleLength;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		if(this->m_iSOffset_Type==0)
		{
			pMCtrlOffsetSE->m_iSOffset_Value1=this->m_lSSecond;
			pMCtrlOffsetSE->m_strS_Time="";
		}
		else if(this->m_iSOffset_Type==1)
		{
			pMCtrlOffsetSE->m_iSOffset_Value1=this->m_lSDay;
			pMCtrlOffsetSE->m_strS_Time=this->m_SDTime.Format("%H:%M:%S");
		}
		else if(this->m_iSOffset_Type==2)
		{
			pMCtrlOffsetSE->m_iSOffset_Value1=this->m_lSWeek;
			pMCtrlOffsetSE->m_iSOffset_Value2=this->m_ComBoSWeek.GetCurSel();
			pMCtrlOffsetSE->m_strS_Time=this->m_SWTime.Format("%H:%M:%S");
		}
		else if(this->m_iSOffset_Type==3)
		{
			pMCtrlOffsetSE->m_iSOffset_Value1=this->m_lSMonth;
			pMCtrlOffsetSE->m_iSOffset_Value2=this->m_ComBoSMonth.GetCurSel();
			pMCtrlOffsetSE->m_strS_Time=this->m_SMTime.Format("%H:%M:%S");
		}
		else if(this->m_iSOffset_Type==4)
		{
			pMCtrlOffsetSE->m_iSOffset_Value1=this->m_lSDuty;
			pMCtrlOffsetSE->m_iSOffset_Value2=this->m_ComboSDuty.GetCurSel();
			pMCtrlOffsetSE->m_strS_Time="";
		}
		if(this->m_iEOffset_Type==0)
		{
			pMCtrlOffsetSE->m_iEOffset_Value1=this->m_lESecond;
			pMCtrlOffsetSE->m_strE_Time="";
		}
		else if(this->m_iEOffset_Type==1)
		{
			pMCtrlOffsetSE->m_iEOffset_Value1=this->m_lEDay;
			pMCtrlOffsetSE->m_strE_Time=this->m_EDTime.Format("%H:%M:%S");
		}
		else if(this->m_iEOffset_Type==2)
		{
			pMCtrlOffsetSE->m_iEOffset_Value1=this->m_lEWeek;
			pMCtrlOffsetSE->m_iEOffset_Value2=this->m_ComboEWeek.GetCurSel();
			pMCtrlOffsetSE->m_strE_Time=this->m_EWTime.Format("%H:%M:%S");
		}
		else if(this->m_iEOffset_Type==3)
		{
			pMCtrlOffsetSE->m_iEOffset_Value1=this->m_lEMonth;
			pMCtrlOffsetSE->m_iEOffset_Value2=this->m_ComboEMonth.GetCurSel();
			pMCtrlOffsetSE->m_strE_Time=this->m_EMTime.Format("%H:%M:%S");
		}
		else if(this->m_iEOffset_Type==4)
		{
			pMCtrlOffsetSE->m_iEOffset_Value1=this->m_lEDuty;
			pMCtrlOffsetSE->m_iEOffset_Value2=this->m_ComboEDuty.GetCurSel();
			pMCtrlOffsetSE->m_strE_Time="";
		}
	}
}

void CDlgMCtrlCycle::initOffsetSETime()
{
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		this->m_iSOffset_Type=pMCtrlOffsetSE->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
		this->m_iEOffset_Type=pMCtrlOffsetSE->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}	
}

void CDlgMCtrlCycle::enbaleWindowBySOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE)
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

void CDlgMCtrlCycle::enbaleWindowByEOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE)
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

void CDlgMCtrlCycle::OnRadio01() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio02() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio03() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio04() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio05() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iSOffset_Type=this->m_iSOffset_Type;
		enbaleWindowBySOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio11() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio12() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio13() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio14() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}
}

void CDlgMCtrlCycle::OnRadio15() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=((MCtrlCycle*)m_pMCtrlType)->m_ArrOffsetSE[0];
		pMCtrlOffsetSE->m_iEOffset_Type=this->m_iEOffset_Type;
		enbaleWindowByEOffsetType(pMCtrlOffsetSE);
	}
}
