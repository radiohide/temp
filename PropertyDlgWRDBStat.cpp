// PropertyDlgWRDBStat.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgWRDBStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWRDBStat dialog


CPropertyDlgWRDBStat::CPropertyDlgWRDBStat(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgWRDBStat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgWRDBStat)
	m_BMaxValue = FALSE;
	m_BMinValue = FALSE;
	m_BStat = FALSE;
	m_BCount = FALSE;
	m_strVarDesp = _T("");
	m_strVarName = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgWRDBStat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgWRDBStat)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Check(pDX, IDC_CHECK_MAX_VALUE, m_BMaxValue);
	DDX_Check(pDX, IDC_CHECK_MIN_VALUE, m_BMinValue);
	DDX_Check(pDX, IDC_CHECK_STAT, m_BStat);
	DDX_Check(pDX, IDC_CHECK_COUNT, m_BCount);
	DDX_Text(pDX, IDC_EDIT_DESP, m_strVarDesp);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgWRDBStat, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgWRDBStat)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWRDBStat message handlers
void CPropertyDlgWRDBStat::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}

BOOL CPropertyDlgWRDBStat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBName);
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBName)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgWRDBStat::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBName=despArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBName="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);
	UpdateData(false);
}
