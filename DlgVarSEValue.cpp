// DlgVarSEValue.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgVarSEValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVarSEValue dialog


CDlgVarSEValue::CDlgVarSEValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVarSEValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVarSEValue)
	m_dEValue = 0.0;
	m_iState = 0;
	m_dSValue = 0.0;
	m_iUpOrDown = 1;//上限区间
	//}}AFX_DATA_INIT
}


void CDlgVarSEValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVarSEValue)
	DDX_Text(pDX, IDC_EDIT_EVALUE, m_dEValue);
	DDX_Text(pDX, IDC_EDIT_STATE, m_iState);
	DDX_Text(pDX, IDC_EDIT_SVALUE, m_dSValue);
	DDX_Text(pDX, IDC_EDIT_UPORDOWM, m_iUpOrDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVarSEValue, CDialog)
	//{{AFX_MSG_MAP(CDlgVarSEValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVarSEValue message handlers

void CDlgVarSEValue::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_dSValue>=m_dEValue)
	{
		AfxMessageBox("区间开始应小于区间结束，请重新设置！");
		return;
	}
	CDialog::OnOK();
}
