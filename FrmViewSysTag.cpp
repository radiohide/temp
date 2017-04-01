// FrmViewSysTag.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "FrmViewSysTag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrmViewSysTag

IMPLEMENT_DYNCREATE(CFrmViewSysTag, CFormView)

CFrmViewSysTag::CFrmViewSysTag()
	: CFormView(CFrmViewSysTag::IDD)
{
	//{{AFX_DATA_INIT(CFrmViewSysTag)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFrmViewSysTag::~CFrmViewSysTag()
{
}

void CFrmViewSysTag::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrmViewSysTag)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrmViewSysTag, CFormView)
	//{{AFX_MSG_MAP(CFrmViewSysTag)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrmViewSysTag diagnostics

#ifdef _DEBUG
void CFrmViewSysTag::AssertValid() const
{
	CFormView::AssertValid();
}

void CFrmViewSysTag::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrmViewSysTag message handlers

void CFrmViewSysTag::OnButtonConfirm() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("confirm this result");
}

void CFrmViewSysTag::Show()
{
	AfxMessageBox("show module info");
}
