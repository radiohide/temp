// ViewUserFrm.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "ViewUserFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewUserFrm

IMPLEMENT_DYNCREATE(CViewUserFrm, CFormView)

CViewUserFrm::CViewUserFrm(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	//{{AFX_DATA_INIT(CViewUserFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewUserFrm::CViewUserFrm()
	: CFormView(CViewUserFrm::IDD)
{
	
}


CViewUserFrm::~CViewUserFrm()
{
}

void CViewUserFrm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewUserFrm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewUserFrm, CFormView)
	//{{AFX_MSG_MAP(CViewUserFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewUserFrm diagnostics

#ifdef _DEBUG
void CViewUserFrm::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewUserFrm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewUserFrm message handlers
void CViewUserFrm::RefreshWindow()
{
	
}