// PropertyDlgStableJudge.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStableJudge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStableJudge dialog


CPropertyDlgStableJudge::CPropertyDlgStableJudge(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStableJudge::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStableJudge)
	m_dDown2 = 90.0;
	m_dDown1 = 95.0;
	m_dFazhi1 = 5.0;
	m_dFazhi2 = 2.0;
	m_iRadio = 0;
	m_BCalcNoStable = FALSE;
	//}}AFX_DATA_INIT
}


void CPropertyDlgStableJudge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStableJudge)
	DDX_Text(pDX, IDC_EDIT_DOWN2, m_dDown2);
	DDV_MinMaxDouble(pDX, m_dDown2, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_DOWN1, m_dDown1);
	DDV_MinMaxDouble(pDX, m_dDown1, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_F1, m_dFazhi1);
	DDV_MinMaxDouble(pDX, m_dFazhi1, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_F2, m_dFazhi2);
	DDV_MinMaxDouble(pDX, m_dFazhi2, 0., 100.);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	DDX_Check(pDX, IDC_CHECK_CALC_NOSTABLE, m_BCalcNoStable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStableJudge, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStableJudge)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStableJudge message handlers

void CPropertyDlgStableJudge::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_iRadio=0;
}

void CPropertyDlgStableJudge::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_iRadio=1;
}
void CPropertyDlgStableJudge::OnOK() 
{
	// TODO: Add extra validation here
	m_BCalcNoStable=((CButton*)GetDlgItem(IDC_CHECK_CALC_NOSTABLE))->GetCheck();
	CDialog::OnOK();
}
