// DlgCoalFlowTagData.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgCoalFlowTagData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCoalFlowTagData dialog


CDlgCoalFlowTagData::CDlgCoalFlowTagData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCoalFlowTagData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCoalFlowTagData)
	m_strDesp = _T("");
	m_strTagName = _T("");
	m_strUnit = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCoalFlowTagData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCoalFlowTagData)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_TAGNAME, m_strTagName);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_strUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCoalFlowTagData, CDialog)
	//{{AFX_MSG_MAP(CDlgCoalFlowTagData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCoalFlowTagData message handlers

void CDlgCoalFlowTagData::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strTagName=="")
	{
		AfxMessageBox("ÇëÊäÈë±êÇ©Ãû³Æ£¡");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgCoalFlowTagData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_strWinName!="")
		this->SetWindowText(m_strWinName);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
