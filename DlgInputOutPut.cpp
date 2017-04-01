// DlgInputOutPut.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgInputOutPut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutPut dialog


CDlgInputOutPut::CDlgInputOutPut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputOutPut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputOutPut)
	m_strDesp = _T("");
	m_strInput = _T("");
	m_strOutput = _T("");
	m_strUnit = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInputOutPut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputOutPut)
	DDX_Text(pDX, IDC_EDIT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputOutPut, CDialog)
	//{{AFX_MSG_MAP(CDlgInputOutPut)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutPut message handlers
