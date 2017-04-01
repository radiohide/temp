// DlgIPPortData.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgIPPortData.h"
#include "ePerfManageDoc.h"
#include "SocketManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIPPortData dialog
extern enum RunState{Unknown=-1,Running=1,Starting,Stopped,Stopping,Paused,StartFail};
bool g_SocketInit(CSocketManage *&pSocket,const char* IP,int port);
void g_SocketExit(CSocketManage *&pSocket);

CDlgIPPortData::CDlgIPPortData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIPPortData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIPPortData)
	//}}AFX_DATA_INIT
}


void CDlgIPPortData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIPPortData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIPPortData, CDialog)
	//{{AFX_MSG_MAP(CDlgIPPortData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIPPortData message handlers

BOOL CDlgIPPortData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	str.Format("%s--IP和Port",m_strModeName);
	this->SetWindowText(str);
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(m_StrIP);
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(m_StrPort);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIPPortData::OnOK() 
{
	// TODO: Add extra validation here
	CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
	if(pDoc)
	{
		CSocketManage *pSocketManage;
		if(g_SocketInit(pSocketManage,m_StrIP,11650))
		{
			int iModeState=pDoc->GetNodeState(pSocketManage,m_strModeName);
			if((iModeState==Running)||(iModeState==Paused))
			{
				AfxMessageBox("当前模型正在运行,请先停止后再进行部署！");
				g_SocketExit(pSocketManage);
				return;
			}
		}
		g_SocketExit(pSocketManage);
	}
	UpdateData(true);
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(m_StrIP);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(m_StrPort);
	CDialog::OnOK();
}
