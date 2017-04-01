// DlgSelMonth.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgSelMonth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelMonth dialog


CDlgSelMonth::CDlgSelMonth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelMonth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelMonth)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelMonth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelMonth)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelMonth, CDialog)
	//{{AFX_MSG_MAP(CDlgSelMonth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelMonth message handlers
void CDlgSelMonth::SetMonthNum(CArray<int,int> &mNumArr)
{
	m_mNumArr.Copy(mNumArr);
}
BOOL CDlgSelMonth::OnInitDialog() 
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
	((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(0);

	for(int i=0;i<m_mNumArr.GetSize();i++)
	{
		int mNum=m_mNumArr[i];
		if(mNum==0)
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
		else if(mNum==1)
			((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
		else if(mNum==2)
			((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
		else if(mNum==3)
			((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
		else if(mNum==4)
			((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);
		else if(mNum==5)
			((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1);
		else if(mNum==6)
			((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(1);
		else if(mNum==7)
			((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(1);
		else if(mNum==8)
			((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(1);
		else if(mNum==9)
			((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(1);
		else if(mNum==10)
			((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(1);
		else if(mNum==11)
			((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelMonth::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	m_mNumArr.RemoveAll();
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		m_mNumArr.Add(0);
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
		m_mNumArr.Add(1);
	if(((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
		m_mNumArr.Add(2);
	if(((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
		m_mNumArr.Add(3);
	if(((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck())
		m_mNumArr.Add(4);
	if(((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck())
		m_mNumArr.Add(5);
	if(((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck())
		m_mNumArr.Add(6);
	if(((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck())
		m_mNumArr.Add(7);
	if(((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck())
		m_mNumArr.Add(8);
	if(((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck())
		m_mNumArr.Add(9);
	if(((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck())
		m_mNumArr.Add(10);
	if(((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck())
		m_mNumArr.Add(11);
	CDialog::OnOK();
}
