// PropertyDlgDBPointAdv.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgDBPointAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBPointAdv dialog


CPropertyDlgDBPointAdv::CPropertyDlgDBPointAdv(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgDBPointAdv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgDBPointAdv)
	m_strTagDesp = _T("");
	m_strTag1 = _T("");
	m_strTag2 = _T("");
	m_strTag3 = _T("");
	m_strTag4 = _T("");
	m_strTag5 = _T("");	
	m_iOutMethod = -1;
	m_bChkLimit = FALSE;
	m_dHighLimit = 0.0;
	m_dLowLimit = 0.0;
	m_dConstWhenBad = 0.0;
	m_bForceEnable = FALSE;
	m_dForceValue = 0.0;
	m_lOffset = 0;
	m_iInOrOut = -1;
	//}}AFX_DATA_INIT
	//}}AFX_DATA_INIT
}


void CPropertyDlgDBPointAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgDBPointAdv)
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Text(pDX, IDC_EDIT_TAG1, m_strTag1);
	DDX_Text(pDX, IDC_EDIT_TAG2, m_strTag2);
	DDX_Text(pDX, IDC_EDIT_TAG3, m_strTag3);
	DDX_Text(pDX, IDC_EDIT_TAG4, m_strTag4);
	DDX_Text(pDX, IDC_EDIT_TAG5, m_strTag5);
	DDX_CBIndex(pDX, IDC_COMBO_OUTMETHOD, m_iOutMethod);
	DDX_Check(pDX, IDC_CHECK_LIMIT, m_bChkLimit);
	DDX_Text(pDX, IDC_EDIT_HIGHLIMIT, m_dHighLimit);
	DDX_Text(pDX, IDC_EDIT_LOWLIMIT, m_dLowLimit);
	DDX_Text(pDX, IDC_EDIT_CONSTWHENBAD, m_dConstWhenBad);
	DDX_Check(pDX, IDC_CHECK_FORCEOUTPUT, m_bForceEnable);
	DDX_Text(pDX, IDC_EDIT_FORCEVALUE, m_dForceValue);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Radio(pDX, IDC_RADIO1, m_iInOrOut);

	DDX_Text(pDX, IDC_EDIT_DBTYPE, m_strDBType);
	DDX_CBString(pDX, IDC_COMBO_DBSEL2, m_strDBDesp);
	DDX_Control(pDX, IDC_COMBO_DBSEL2, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	DDX_Radio(pDX, IDC_RADIO_FORWARD, m_iForwardOrBack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgDBPointAdv, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgDBPointAdv)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL2, OnSelchangeComboDbsel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBPointAdv message handlers
void CPropertyDlgDBPointAdv::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgDBPointAdv::OnRadio1() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
	}
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBDesp)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		m_strDBType=typeArr[iBeFind];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}
	UpdateData(false);
}

BOOL CPropertyDlgDBPointAdv::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}

	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBDesp)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		m_strDBType=typeArr[iBeFind];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgDBPointAdv::OnRadio2() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_iInOrOut==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
}

void CPropertyDlgDBPointAdv::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBDesp=despArr[selIndex];
		m_strDBType=typeArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBDesp=str;
		m_strDBType="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	UpdateData(false);
}