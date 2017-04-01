// DlgTabFindBlock.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgTabFindBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTabFindBlock dialog


CDlgTabFindBlock::CDlgTabFindBlock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTabFindBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTabFindBlock)
	m_bAllSame = FALSE;
	m_strRange = _T("当前打开模块");
	m_strType = _T("计算块名称");
	m_strFind = _T("");
	m_strReplace = _T("");
	//}}AFX_DATA_INIT
}


void CDlgTabFindBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTabFindBlock)
	DDX_Check(pDX, IDC_CHECK_ALLSAME, m_bAllSame);
	DDX_CBString(pDX, IDC_COMBO_RANGE, m_strRange);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_FIND, m_strFind);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_strReplace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTabFindBlock, CDialog)
	//{{AFX_MSG_MAP(CDlgTabFindBlock)
	ON_EN_CHANGE(IDC_EDIT_FIND, OnChangeEditFind)
	ON_EN_CHANGE(IDC_EDIT_REPLACE, OnChangeEditReplace)
	ON_BN_CLICKED(IDC_CHECK_ALLSAME, OnCheckAllsame)
	ON_CBN_SELCHANGE(IDC_COMBO_RANGE, OnSelchangeComboRange)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTabFindBlock message handlers

BOOL CDlgTabFindBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bTagEdit=FALSE;
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTabFindBlock::SetSomeCtrlObjHide()
{
	GetDlgItem(IDC_STATIC_REPLACE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_REPLACE)->ShowWindow(SW_HIDE);
}
void CDlgTabFindBlock::SetSomeCtrlObjShow()
{
	GetDlgItem(IDC_STATIC_REPLACE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_REPLACE)->ShowWindow(SW_SHOW);
}

void CDlgTabFindBlock::OnChangeEditFind() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bTagEdit=true;
}

void CDlgTabFindBlock::OnChangeEditReplace() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bTagEdit=true;
}

void CDlgTabFindBlock::OnCheckAllsame() 
{
	// TODO: Add your control notification handler code here
	m_bTagEdit=true;
}

void CDlgTabFindBlock::OnSelchangeComboRange() 
{
	// TODO: Add your control notification handler code here
	m_bTagEdit=true;
}

void CDlgTabFindBlock::OnSelchangeComboType() 
{
	// TODO: Add your control notification handler code here
	m_bTagEdit=true;
}
