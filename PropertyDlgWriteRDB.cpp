// PropertyDlgWriteRDB.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgWriteRDB.h"
#include "PropertyDlgRDBField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRDB dialog


CPropertyDlgWriteRDB::CPropertyDlgWriteRDB(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgWriteRDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgWriteRDB)
	m_TagTimeOffsetOrInput = -1;
	m_lOffset = 0;
	m_strTagName = _T("");
	m_strTagDesp = _T("");
	CString	m_strTableName=_T("");
	CString m_strDBDesp=_T("");
	m_iPointStatus = 0;
	m_iPointType = -1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgWriteRDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgWriteRDB)
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_iPointStatus);
	DDX_Text(pDX, IDC_EDIT_TAG, m_strTagName);
	DDX_Text(pDX, IDC_EDIT_TAGDESP, m_strTagDesp);
	DDX_Radio(pDX, IDC_RADIO1, m_iPointType);
	DDX_Radio(pDX, IDC_RADIO5, m_TagTimeOffsetOrInput);
	DDX_CBString(pDX, IDC_COMBO_DBSEL, m_strDBDesp);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgWriteRDB, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgWriteRDB)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio4)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_RDBFIELD, OnButtonRdbField)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWriteRDB message handlers
void CPropertyDlgWriteRDB::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
BOOL CPropertyDlgWriteRDB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_TagTimeOffsetOrInput==0)
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
			CString str1=m_ArrSourceRDBType[i];
			CString str2=m_ArrSourceRDBType[i];

			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBDesp);
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
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgWriteRDB::OnRadio3() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
	
}

void CPropertyDlgWriteRDB::OnRadio4() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
	
}

void CPropertyDlgWriteRDB::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBDesp=despArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBDesp="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);
	UpdateData(false);
}
void CPropertyDlgWriteRDB::OnButtonRdbField() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgRDBField dlg;	
	dlg.m_pStrFieldVarName=&m_strFieldVarName;
	dlg.m_pStrFieldDateTime=&m_strFieldDateTime;
	dlg.m_pStrFieldState=&m_strFieldState;
	dlg.m_pStrFieldValue=&m_strFieldValue;
	dlg.DoModal();
}