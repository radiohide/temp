// ModelView.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "ModelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelView

IMPLEMENT_DYNCREATE(CModelView, CListView)

CModelView::CModelView()
{
}

CModelView::~CModelView()
{
}


BEGIN_MESSAGE_MAP(CModelView, CListView)
	//{{AFX_MSG_MAP(CModelView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelView drawing

void CModelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CModelView diagnostics

#ifdef _DEBUG
void CModelView::AssertValid() const
{
	CListView::AssertValid();
}

void CModelView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModelView message handlers
