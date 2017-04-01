// DlgMRename.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgMRename.h"
#include "ePerfManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMRename dialog


CDlgMRename::CDlgMRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMRename)
	m_strDesp = _T("");
	m_strName = _T("");
	m_strWinText="������";
	//}}AFX_DATA_INIT
}


void CDlgMRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMRename)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMRename, CDialog)
	//{{AFX_MSG_MAP(CDlgMRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMRename message handlers

BOOL CDlgMRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_strWinText);
	m_strOldName=m_strName;
	m_strOldDesp=m_strDesp;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMRename::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strName=="")
	{
		AfxMessageBox("���Ʋ���Ϊ�գ����������룡");
		return;
	}
	if((m_strOldName==m_strName)&&(m_strOldDesp==m_strDesp))//û�б仯����ʾֱ�ӹر�������
	{
		CDialog::OnCancel();
	}
	else
	{
		if(m_strOldName!=m_strName)
		{
			CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
			if(pDoc->m_pModel->LookUpMoudle(m_strName,m_strDesp)==1)
			{
				AfxMessageBox("ģ�������ظ��������¶���ģ������!");
				return;
			}
		}
		CDialog::OnOK();
	}
}
