// PropertyDlgDBPoint.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgDBPoint.h"
#include "PropertyDlgRDBField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBPoint dialog


CPropertyDlgDBPoint::CPropertyDlgDBPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgDBPoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgDBPoint)
	m_lOffset = 0;
	m_strTagName = _T("");
	m_strTagDesp = _T("");
	m_TagTimeOffsetOrInput = -1;
	m_iForwardOrBack = -1;
	m_strTableName = _T("");
	m_strDBType = _T("");
	m_strSelDBDesp = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgDBPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgDBPoint)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Text(pDX, IDC_EDIT_TAG1, m_strTagName);
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Radio(pDX, IDC_RADIO1, m_TagTimeOffsetOrInput);
	DDX_Radio(pDX, IDC_RADIO_FORWARD, m_iForwardOrBack);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_DBTYPE, m_strDBType);
	DDX_CBString(pDX, IDC_COMBO_DBSEL, m_strSelDBDesp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgDBPoint, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgDBPoint)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_RDBFIELD, OnButtonRdbField)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDBPoint message handlers
void CPropertyDlgDBPoint::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
}

void CPropertyDlgDBPoint::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
}

BOOL CPropertyDlgDBPoint::OnInitDialog() 
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

void CPropertyDlgDBPoint::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgDBPoint::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strSelDBDesp=despArr[selIndex];
		m_strDBDesp=despArr[selIndex];
		m_strDBType=typeArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBDesp=str;
		m_strSelDBDesp=str;
		m_strDBType="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	UpdateData(false);
}

void CPropertyDlgDBPoint::OnButtonRdbField() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgRDBField dlg;	
	dlg.m_pStrFieldVarName=&m_strFieldVarName;
	dlg.m_pStrFieldDateTime=&m_strFieldDateTime;
	dlg.m_pStrFieldState=&m_strFieldState;
	dlg.m_pStrFieldValue=&m_strFieldValue;
	dlg.DoModal();
}
