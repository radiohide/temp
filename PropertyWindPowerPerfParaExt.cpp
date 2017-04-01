// PropertyWindPowerPerfParaExt.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyWindPowerPerfParaExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyWindPowerPerfParaExt dialog


CPropertyWindPowerPerfParaExt::CPropertyWindPowerPerfParaExt(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyWindPowerPerfParaExt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyWindPowerPerfParaExt)
		// NOTE: the ClassWizard will add member initialization here
	m_strTableName = _T("");
	m_strTurbine = _T("");
	m_iPowerPorTag = 0;
	m_dMinParWindSpeed = 0.0;
	m_dMaxParWindSpeed = 0.0;
	m_dPowerLimit = 0.0;
	m_iPowerPorTagMin = 0;
	m_iPowerPorTagMax = 0;
	//}}AFX_DATA_INIT
}


void CPropertyWindPowerPerfParaExt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyWindPowerPerf)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_TABLE_NAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_Turbine, m_strTurbine);
	DDX_Text(pDX, IDC_EDIT_MIN_SPEED, m_dMinParWindSpeed);
	DDX_Text(pDX, IDC_EDIT_MAXSPEED, m_dMaxParWindSpeed);
	DDX_Text(pDX, IDC_EDIT_LIMIT, m_dPowerLimit);
	DDX_Text(pDX, IDC_EDIT_Product_MIN, m_iPowerPorTagMin);
	DDX_Text(pDX, IDC_EDIT_Product_MAX, m_iPowerPorTagMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyWindPowerPerfParaExt, CDialog)
//{{AFX_MSG_MAP(CPropertyWindPowerPerf)
ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyWindPowerPerf message handlers

BOOL CPropertyWindPowerPerfParaExt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
void CPropertyWindPowerPerfParaExt::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
											  CStringArray &m_ArrSourceRDBName,
											  CStringArray &m_ArrSourceRDBType
											  )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}

void CPropertyWindPowerPerfParaExt::OnSelchangeComboDbsel() 
{
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
