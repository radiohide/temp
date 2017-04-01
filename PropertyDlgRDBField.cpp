// PropertyDlgRDBField.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRDBField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBField dialog


CPropertyDlgRDBField::CPropertyDlgRDBField(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgRDBField::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgRDBField)
	m_strName = _T("");
	m_strState = _T("");
	m_strTime = _T("");
	m_strValue = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgRDBField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgRDBField)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_STATE, m_strState);
	DDX_Text(pDX, IDC_EDIT_TIME, m_strTime);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgRDBField, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgRDBField)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBField message handlers

BOOL CPropertyDlgRDBField::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strName = *m_pStrFieldVarName;
	m_strTime = *m_pStrFieldDateTime;
	m_strState = *m_pStrFieldState;
	m_strValue = *m_pStrFieldValue;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgRDBField::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if((m_strName=="")||(m_strTime=="")||(m_strState=="")||(m_strValue==""))
	{
		AfxMessageBox("×Ö¶Î²»ÄÜÎª¿Õ£¬ÇëÊäÈë×Ö¶Î£¡");
		return;
	}
	*m_pStrFieldVarName = m_strName;
	*m_pStrFieldDateTime = m_strTime;
	*m_pStrFieldState = m_strState;
	*m_pStrFieldValue = m_strValue;
	CDialog::OnOK();
}
