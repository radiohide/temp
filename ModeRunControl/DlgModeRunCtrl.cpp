// DlgModeRunCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgModeRunCtrl.h"
#include "DlgCfgSETime.h"
#include "DlgTaskSUpTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModeRunCtrl dialog


CDlgModeRunCtrl::CDlgModeRunCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModeRunCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModeRunCtrl)
	m_pMCalControl=NULL;
	//}}AFX_DATA_INIT
}


void CDlgModeRunCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModeRunCtrl)
	DDX_Control(pDX, IDC_COMBO_TASKTYPE, m_ComBoTaskType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModeRunCtrl, CDialog)
	//{{AFX_MSG_MAP(CDlgModeRunCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO_TASKTYPE, OnSelchangeComboTasktype)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SETRUNABSTIME, OnButtonSetRunABSTime)
	ON_BN_CLICKED(IDC_BUTTON_SETRUNDATATIME, OnButtonSetRunDataTime)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, OnButtonCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModeRunCtrl message handlers
void CDlgModeRunCtrl::SetModelCalcControl(ModelCalcControl *pMCalControl)
{
	this->m_pMCalControl=pMCalControl;
}

BOOL CDlgModeRunCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CRect rc;
	GetDlgItem(IDC_STATIC_CTRLDLG)->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.top += 13;
	rc.bottom -= 3;
	rc.left += 3;
	rc.right -= 3;

	CDlgMCtrlType *pDlg;

	pDlg=new CDlgMCtrlCycle();
	pDlg->SetMCtrlData(m_pMCalControl->m_ArrMCtrl[0]);
	pDlg->Create(IDD_DIALOG_MCTRL_CYCLE,this);//必须执行create
	pDlg->MoveWindow(rc);
	dlgAllTypeArr.Add(pDlg);

	pDlg=new CDlgMCtrlDay();
	pDlg->SetMCtrlData(m_pMCalControl->m_ArrMCtrl[1]);
	pDlg->Create(IDD_DIALOG_MCTRL_DAY,this);//必须执行create
	pDlg->MoveWindow(rc);
	dlgAllTypeArr.Add(pDlg);

	pDlg=new CDlgMCtrlWeek();
	pDlg->SetMCtrlData(m_pMCalControl->m_ArrMCtrl[2]);
	pDlg->Create(IDD_DIALOG_MCTRL_WEEK,this);//必须执行create
	pDlg->MoveWindow(rc);
	dlgAllTypeArr.Add(pDlg);

	pDlg=new CDlgMCtrlMonth();
	pDlg->SetMCtrlData(m_pMCalControl->m_ArrMCtrl[3]);
	pDlg->Create(IDD_DIALOG_MCTRL_MONTH,this);//必须执行create
	pDlg->MoveWindow(rc);
	dlgAllTypeArr.Add(pDlg);

	pDlg=new CDlgMCtrlDuty();
	pDlg->SetMCtrlData(m_pMCalControl->m_ArrMCtrl[4]);
	pDlg->Create(IDD_DIALOG_MCTRL_DUTY,this);//必须执行create
	pDlg->MoveWindow(rc);
	dlgAllTypeArr.Add(pDlg);

	m_pMCalControl->ReadCtrlData();
	GetDlgItem(IDC_STATIC_TASKLIFE)->SetWindowText(m_pMCalControl->GetMLifeStr());
	GetDlgItem(IDC_STATIC_CALCDATASETIME)->SetWindowText(m_pMCalControl->GetCalcDataSETimeStr());

	if(m_pMCalControl->m_iMode_RunType>-1)
	{
		m_ComBoTaskType.SetCurSel(m_pMCalControl->m_iMode_RunType);
		showMCtrlDlgByIndex(m_pMCalControl->m_iMode_RunType);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModeRunCtrl::OnSelchangeComboTasktype()
{
	// TODO: Add your control notification handler code here
	int selIndex=m_ComBoTaskType.GetCurSel ();
	showMCtrlDlgByIndex(selIndex);
}
void CDlgModeRunCtrl::showMCtrlDlgByIndex(int index)
{
	for(int i=0;i<dlgAllTypeArr.GetSize();i++)
	{
		if(i==index)
		{
			dlgAllTypeArr[i]->ShowCtrlData();
			dlgAllTypeArr[i]->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CTRLDLG)->SetWindowText(dlgAllTypeArr[i]->m_strTypeName);
		}
		else
		{
			dlgAllTypeArr[i]->ResetCtrlData();
			dlgAllTypeArr[i]->ShowWindow(SW_HIDE);
		}
	}
}
void CDlgModeRunCtrl::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDlgMCtrlType *pDlg;
	for(int i=0;i<dlgAllTypeArr.GetSize();i++)
	{
		pDlg=dlgAllTypeArr[i];
		if(pDlg!=NULL)
		{
			delete pDlg;
			pDlg=NULL;
		}
	}
	dlgAllTypeArr.RemoveAll();
	CDialog::OnClose();
}

void CDlgModeRunCtrl::OnButtonSetRunABSTime() 
{
	// TODO: Add your control notification handler code here
	CDlgTaskSUpTime dlg;
	dlg.SetSTime(this->m_pMCalControl->m_tRunAbs_STime);
	if(dlg.DoModal()==IDOK)
	{
		this->m_pMCalControl->m_tRunAbs_STime=dlg.GetStartTime();
		GetDlgItem(IDC_STATIC_TASKLIFE)->SetWindowText(m_pMCalControl->GetMLifeStr());
	}
}

void CDlgModeRunCtrl::OnButtonSetRunDataTime() 
{
	// TODO: Add your control notification handler code here
	CDlgCfgSETime dlg;
	dlg.SetSETime(this->m_pMCalControl->m_tCalcDataAll_STime,this->m_pMCalControl->m_tCalcDataAll_ETime);
	if(dlg.DoModal()==IDOK)
	{
		this->m_pMCalControl->m_tCalcDataAll_STime=dlg.GetStartTime();
		this->m_pMCalControl->m_tCalcDataAll_ETime=dlg.GetEndTime();
		GetDlgItem(IDC_STATIC_CALCDATASETIME)->SetWindowText(m_pMCalControl->GetCalcDataSETimeStr());
	}
}

void CDlgModeRunCtrl::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	int selIndex=m_ComBoTaskType.GetCurSel ();
	if(selIndex<0)
	{
		AfxMessageBox("请选择计划任务！");
		return;
	}
	m_pMCalControl->SetModeRunType(selIndex);
	m_pMCalControl->SaveCtrlDataToDB();

	for(int i=0;i<dlgAllTypeArr.GetSize();i++)
	{
		if(i==selIndex)
		{
			dlgAllTypeArr[i]->SaveCtrlDataToDB();
		}
		else
		{
			dlgAllTypeArr[i]->DelCtrlDataIoDB();
		}
	}
}

void CDlgModeRunCtrl::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	int selIndex=m_ComBoTaskType.GetCurSel ();
	if(selIndex<0)
	{
		AfxMessageBox("请选择计划任务！");
		return;
	}
	m_pMCalControl->SetModeRunType(selIndex);
	m_pMCalControl->SaveCtrlDataToDB();

	for(int i=0;i<dlgAllTypeArr.GetSize();i++)
	{
		if(i==selIndex)
		{
			dlgAllTypeArr[i]->SaveCtrlDataToDB();
		}
		else
		{
			dlgAllTypeArr[i]->DelCtrlDataIoDB();
		}
	}
	CDialog::OnOK();
}

void CDlgModeRunCtrl::OnButtonCancle() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
