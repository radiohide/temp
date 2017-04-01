// FrmViewDesignData.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "FrmViewDesignData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrmViewDesignData

IMPLEMENT_DYNCREATE(CFrmViewDesignData, CFormView)

CFrmViewDesignData::CFrmViewDesignData()
	: CFormView(CFrmViewDesignData::IDD)
{
	//{{AFX_DATA_INIT(CFrmViewDesignData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFrmViewDesignData::~CFrmViewDesignData()
{
}

void CFrmViewDesignData::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrmViewDesignData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrmViewDesignData, CFormView)
	//{{AFX_MSG_MAP(CFrmViewDesignData)
//	ON_COMMAND(ID_CFG_ADD_BLOCK, OnCfgAddBlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrmViewDesignData diagnostics

#ifdef _DEBUG
void CFrmViewDesignData::AssertValid() const
{
	CFormView::AssertValid();
}

void CFrmViewDesignData::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrmViewDesignData message handlers
/*
void CFrmViewDesignData::OnCfgAddBlock() 
{
	AfxMessageBox("Ìí¼Ó¼ÆËã¿é");
}
*/
