// PropertyDlgRTPoint.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRTPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPoint dialog


CPropertyDlgRTPoint::CPropertyDlgRTPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgRTPoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgRTPoint)
	m_lOffset = 0;
	m_strTagName = _T("");
	m_strTagDesp = _T("");
	m_TagTimeOffsetOrInput = -1;
	m_TagQueryTimeSpan = -1;
	m_lTimeLength = 0;
	m_iQueryType=0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgRTPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgRTPoint)
	DDX_Control(pDX, IDC_COMBO_QUERYTYPE, m_CComboBoxQueryType);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Text(pDX, IDC_EDIT_TAG1, m_strTagName);
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Radio(pDX, IDC_RADIO1, m_TagTimeOffsetOrInput);
	DDX_Radio(pDX, IDC_RADIO7, m_TagQueryTimeSpan);
	DDX_Text(pDX, IDC_EDIT_TIMELENGTH, m_lTimeLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgRTPoint, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgRTPoint)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRTPoint message handlers

void CPropertyDlgRTPoint::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
}

void CPropertyDlgRTPoint::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
}

BOOL CPropertyDlgRTPoint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_TagTimeOffsetOrInput==1)//系统时间加偏移
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
	}
	if(m_TagQueryTimeSpan==1)//取一段时间的统计值
	{
		GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_QUERYTYPE)->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(true);
	}
	else
	{
		GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_QUERYTYPE)->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(true);
	}
	m_CComboBoxQueryType.SetCurSel(m_iQueryType);//默认取均值
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgRTPoint::OnOK() 
{
	// TODO: Add extra validation here
	m_iQueryType=this->m_CComboBoxQueryType.GetCurSel();
	CDialog::OnOK();
}

void CPropertyDlgRTPoint::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_QUERYTYPE)->EnableWindow(false);
}

void CPropertyDlgRTPoint::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_QUERYTYPE)->EnableWindow(true);
}
