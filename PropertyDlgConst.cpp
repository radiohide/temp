// PropertyDlgConst.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgConst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgConst dialog


CPropertyDlgConst::CPropertyDlgConst(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgConst::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgConst)
	m_dConstValue = 0.0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgConst::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgConst)
	DDX_Text(pDX, IDC_EDIT_CONST, m_dConstValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgConst, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgConst)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgConst message handlers
