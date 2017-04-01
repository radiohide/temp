// DlgDisData.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgDisData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDisData dialog


CDlgDisData::CDlgDisData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDisData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDisData)
	m_dDisValue = 0.0;
	m_dEValue = 0.0;
	m_dSValue = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgDisData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDisData)
	DDX_Text(pDX, IDC_EDIT_DIS_VALUE, m_dDisValue);
	DDX_Text(pDX, IDC_EDIT_EVALUE, m_dEValue);
	DDX_Text(pDX, IDC_EDIT_SVALUE, m_dSValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDisData, CDialog)
	//{{AFX_MSG_MAP(CDlgDisData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisData message handlers

void CDlgDisData::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_dEValue<=m_dSValue)
	{
		AfxMessageBox("区间终点应大于区间起点，请重新输入！");
		return;
	}
	CDialog::OnOK();
}
