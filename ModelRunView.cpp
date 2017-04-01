// ModelRunView.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "ModelRunView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelRunView

IMPLEMENT_DYNCREATE(CModelRunView, CView)

CModelRunView::CModelRunView()
{
}

CModelRunView::~CModelRunView()
{
}


BEGIN_MESSAGE_MAP(CModelRunView, CView)
	//{{AFX_MSG_MAP(CModelRunView)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelRunView drawing

void CModelRunView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CModelRunView diagnostics

#ifdef _DEBUG
void CModelRunView::AssertValid() const
{
	CView::AssertValid();
}

void CModelRunView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModelRunView message handlers

