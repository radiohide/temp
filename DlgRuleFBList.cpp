// DlgRuleFBList.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgRuleFBList.h"
#include "DlgPropRuleRun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleFBList dialog

CDlgRuleFBList::CDlgRuleFBList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRuleFBList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRuleFBList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRuleFBList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRuleFBList)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRuleFBList, CDialog)
	//{{AFX_MSG_MAP(CDlgRuleFBList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRuleFBList message handlers

BOOL CDlgRuleFBList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	ReadFBData();
	InitListBoxBody();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgRuleFBList::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","前后件名称","前后件描述"};						//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,160);
}
void CDlgRuleFBList::InitListBoxBody()
{
	for(int i=0;i<tableNameArr.GetSize();i++)
	{		
		CString strIndex;
		strIndex.Format("%d",i+1);
		m_List.InsertItem(i,strIndex);

		m_List.SetItemText(i,1,tableNameArr[i]);
		m_List.SetItemText(i,2,tableDespArr[i]);
	}
}
void CDlgRuleFBList::ReadFBData()
{
	tableNameArr.RemoveAll();
	tableDespArr.RemoveAll();

	CDlgPropRuleRun *parDlg=(CDlgPropRuleRun*)GetParent();
	_RecordsetPtr m_pRec;
	CString tableName="table_info";
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s`",tableName);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)(parDlg->m_pConRule),true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	while(!m_pRec->adoEOF)
	{
		tableNameArr.Add((char *)_bstr_t(m_pRec->GetCollect("TableName")));
		tableDespArr.Add((char *)_bstr_t(m_pRec->GetCollect("TableDesp")));
		m_pRec->MoveNext();
	}
}

void CDlgRuleFBList::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		int index =this->m_List.GetNextSelectedItem(pos);
		m_strRuleDBName=m_List.GetItemText(index,1);
		m_strRuleDBDesp=m_List.GetItemText(index,2);
	}
	else
	{
		AfxMessageBox("请选择前后件!");
	}
	CDialog::OnOK();
}
