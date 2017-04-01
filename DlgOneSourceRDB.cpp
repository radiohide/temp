// DlgOneSourceRDB.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgOneSourceRDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOneSourceRDB dialog


CDlgOneSourceRDB::CDlgOneSourceRDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOneSourceRDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOneSourceRDB)
	m_strFileName = _T("");
	m_strIP = _T("");
	m_strRDBName = _T("");
	m_strRDBPass = _T("");
	m_strSourceName = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgOneSourceRDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOneSourceRDB)
	DDX_Control(pDX, IDC_COMBO1, m_ComBo);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_RDBNAME, m_strRDBName);
	DDX_Text(pDX, IDC_EDIT_RDBPASS, m_strRDBPass);
	DDX_Text(pDX, IDC_EDIT_SOURCENAME, m_strSourceName);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOneSourceRDB, CDialog)
	//{{AFX_MSG_MAP(CDlgOneSourceRDB)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOneSourceRDB message handlers

void CDlgOneSourceRDB::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(m_strSourceName=="")
	{
		AfxMessageBox("数据源名称未填写,请填写后保存!");
		return;
	}
	int index=this->m_ComBo.GetCurSel();
	if(index>=0)
	{
		m_ComBo.GetLBText(index,m_strRDBType);
	}
	CString upType=m_strRDBType;
	upType.MakeUpper();
	if(upType=="MYSQL")
	{
		m_strFileName="";
	}
	else if(upType=="MDB")
	{	
		m_strIP="";
		m_strRDBName="";
		m_strRDBPass="";
		m_strUserName="";
		if(m_strFileName=="")
		{
			AfxMessageBox("MDB类型的数据源必须填写文件名,请填写后保存!");
			return;
		}
	}
	UpdateData(false);
	CDialog::OnOK();
}

BOOL CDlgOneSourceRDB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_strRDBType.MakeUpper();
	for(int i=0;i<m_ComBo.GetCount();i++)
	{
		CString str;
		m_ComBo.GetLBText(i,str);
		str.MakeUpper();
		if(str==m_strRDBType)
		{
			if(str=="MYSQL")
			{
				GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(false);
			}
			else if(str=="MDB")
			{
				GetDlgItem(IDC_EDIT_IP)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_RDBNAME)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_RDBPASS)->EnableWindow(false);
			}
			m_ComBo.SetCurSel(i);
			break;
		}
	}
	if(m_ComBo.GetCurSel()<0)
	{
		m_ComBo.SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOneSourceRDB::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	int selIndex=m_ComBo.GetCurSel ();
	if(selIndex==0)
	{
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_RDBNAME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_RDBPASS)->EnableWindow(true);
	}
	else if(selIndex==1)
	{
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_RDBNAME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_RDBPASS)->EnableWindow(false);
	}
}
