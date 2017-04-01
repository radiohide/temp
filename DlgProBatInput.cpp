// DlgProBatInput.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgProBatInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatInput dialog

extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;

CDlgProBatInput::CDlgProBatInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProBatInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProBatInput)
	m_strDesp = _T("");
	m_strInput = _T("");
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgProBatInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProBatInput)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProBatInput, CDialog)
	//{{AFX_MSG_MAP(CDlgProBatInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatInput message handlers

void CDlgProBatInput::OnOK() 
{
	// TODO: Add extra validation here
	CCalcPort *pPortObjFind;
	if(g_WorkSpace.Lookup(m_strInput,pPortObjFind))
	{
		AfxMessageBox("变量标签重复，请更换！");
		return;
	}
	
	CDialog::OnOK();
}
