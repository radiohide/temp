// DlgModelSynNote.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgModelSynNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModelSynNote dialog


CDlgModelSynNote::CDlgModelSynNote(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModelSynNote::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModelSynNote)
	m_iTag=0;
	//}}AFX_DATA_INIT
}


void CDlgModelSynNote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModelSynNote)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModelSynNote, CDialog)
	//{{AFX_MSG_MAP(CDlgModelSynNote)
	ON_BN_CLICKED(IDC_BUTTON_UPRESTAR, OnButtonUpRestar)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, OnButtonCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModelSynNote message handlers

void CDlgModelSynNote::OnButtonUpRestar() 
{
	// TODO: Add your control notification handler code here
	m_iTag=2;
	CDialog::OnOK();
}

void CDlgModelSynNote::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	m_iTag=1;
	CDialog::OnOK();
}

void CDlgModelSynNote::OnButtonCancle() 
{
	// TODO: Add your control notification handler code here
	m_iTag=0;
	CDialog::OnCancel();
}
