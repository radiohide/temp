// DlgProBatOutput.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgProBatOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatOutput dialog
extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;

CDlgProBatOutput::CDlgProBatOutput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProBatOutput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProBatOutput)
	m_strDesp = _T("");
	m_strOutput = _T("");
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgProBatOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProBatOutput)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProBatOutput, CDialog)
	//{{AFX_MSG_MAP(CDlgProBatOutput)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgProBatOutput::OnOK() 
{
	/*
	CCalcPort *pPortObjFind;
	if(!g_WorkSpace.Lookup(m_strOutput,pPortObjFind))
	{
		AfxMessageBox("变量标签重复不存在，请更换！");
		return;
	}
	*/

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgProBatOutput message handlers
