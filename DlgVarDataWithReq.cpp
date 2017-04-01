// DlgVarDataWithReq.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgVarDataWithReq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataWithReq dialog


CDlgVarDataWithReq::CDlgVarDataWithReq(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVarDataWithReq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVarDataWithReq)
	m_strDesp = _T("");
	m_iSnapshotT = 300;
	m_strUnit = _T("");
	m_strVarName = _T("");
	m_strOutName = _T("");
	//}}AFX_DATA_INIT
	m_iSnapType=0;
	m_bEnableReqType=true;
}


void CDlgVarDataWithReq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVarDataWithReq)
	DDX_Control(pDX, IDC_COMBO_REQ_TYPE, m_ComBo);
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_SNAP_T, m_iSnapshotT);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	DDX_Text(pDX, IDC_EDIT_OUTNAME, m_strOutName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVarDataWithReq, CDialog)
	//{{AFX_MSG_MAP(CDlgVarDataWithReq)
	ON_CBN_SELCHANGE(IDC_COMBO_REQ_TYPE, OnSelchangeComboReqType)
	ON_EN_CHANGE(IDC_EDIT_VARNAME, OnChangeEditVarName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVarDataWithReq message handlers

BOOL CDlgVarDataWithReq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_iSnapType==0)
	{
		this->m_ComBo.SetCurSel(0);
	}
	else
	{
		this->m_ComBo.SetCurSel(1);
		GetDlgItem(IDC_EDIT_SNAP_T)->EnableWindow(false);
	}
	if(!m_bEnableReqType)
	{
		GetDlgItem(IDC_COMBO_REQ_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SNAP_T)->EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVarDataWithReq::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(m_strVarName=="")
	{
		AfxMessageBox("变量名不能为空，请输入！");
		return;
	}
	if(m_strOutName=="")
	{
		AfxMessageBox("输出名称不能为空，请输入！");
		return;
	}
	if((m_iSnapType==0)&&(m_iSnapshotT<=0))
	{
		AfxMessageBox("取快照周期为正整数，请重新输入");
		return;
	}
	if(m_iSnapType==1)
	{
		m_iSnapshotT=0;
	}
	CDialog::OnOK();
}

void CDlgVarDataWithReq::OnSelchangeComboReqType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selIndex=m_ComBo.GetCurSel ();
	if(selIndex==0)
	{
		m_iSnapType=0;
		GetDlgItem(IDC_EDIT_SNAP_T)->EnableWindow(true);
	}
	else if(selIndex==1)
	{
		m_iSnapType=1;
		GetDlgItem(IDC_EDIT_SNAP_T)->EnableWindow(false);
	}
}

void CDlgVarDataWithReq::OnChangeEditVarName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(true);
	m_strOutName=m_strVarName;
	UpdateData(false);
	// TODO: Add your control notification handler code here
	
}
