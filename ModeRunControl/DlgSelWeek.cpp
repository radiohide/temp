// DlgSelWeek.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgSelWeek.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelWeek dialog


CDlgSelWeek::CDlgSelWeek(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelWeek::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelWeek)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelWeek::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelWeek)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelWeek, CDialog)
	//{{AFX_MSG_MAP(CDlgSelWeek)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelWeek message handlers
void CDlgSelWeek::SetWeekNum(CArray<int,int> &wNumArr)
{
	m_wNumArr.Copy(wNumArr);
}
BOOL CDlgSelWeek::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(0);

	for(int i=0;i<m_wNumArr.GetSize();i++)
	{
		int wNum=m_wNumArr[i];
		if(wNum==0)
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
		else if(wNum==1)
			((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
		else if(wNum==2)
			((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
		else if(wNum==3)
			((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
		else if(wNum==4)
			((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);
		else if(wNum==5)
			((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1);
		else if(wNum==6)
			((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelWeek::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	m_wNumArr.RemoveAll();
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		m_wNumArr.Add(0);
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
		m_wNumArr.Add(1);
	if(((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
		m_wNumArr.Add(2);
	if(((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
		m_wNumArr.Add(3);
	if(((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck())
		m_wNumArr.Add(4);
	if(((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck())
		m_wNumArr.Add(5);
	if(((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck())
		m_wNumArr.Add(6);
	CDialog::OnOK();
}
