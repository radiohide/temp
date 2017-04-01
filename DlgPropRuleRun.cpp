// DlgPropRuleRun.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgPropRuleRun.h"

#include "DlgRuleFBList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRuleRun dialog
extern CString gGetExeFileDir();

CDlgPropRuleRun::CDlgPropRuleRun(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropRuleRun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropRuleRun)
	m_strDBConnect = _T("");
	m_strRuleDBName = _T("");
	m_bCon=false;
	m_strRuleDBDesp = _T("");

	pDlgFVar=NULL;
	pDlgBVar=NULL;
	//}}AFX_DATA_INIT
}


void CDlgPropRuleRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropRuleRun)
	DDX_Control(pDX, IDC_TAB1, m_TabList);
	DDX_Text(pDX, IDC_EDIT_CONSTRING, m_strDBConnect);
	DDX_Text(pDX, IDC_EDIT_RULEDBNAME, m_strRuleDBName);
	DDX_Text(pDX, IDC_EDIT_RULEDBDESP, m_strRuleDBDesp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropRuleRun, CDialog)
	//{{AFX_MSG_MAP(CDlgPropRuleRun)
	ON_BN_CLICKED(IDC_BUTTON_CONSTRING, OnButtonConstring)
	ON_BN_CLICKED(IDC_BUTTON_SEL_RULEID, OnButtonSelRuleID)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRuleRun message handlers
BOOL CDlgPropRuleRun::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_strDBConnect!="")
	{
		if(!m_bCon)
		{
			if(EstablishMDBConnectPtr(getABSDirRuleDB()))
			{
				m_bCon=true;
			}
		}
	}
	initTabCtrl();
	displayFBVar();
	if(m_iTagToFOrB==0)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	if(m_iTagWeightOrTime==0)
	{
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
CString CDlgPropRuleRun::getABSDirRuleDB()
{
	UpdateData();
	CString strRet;
	if(m_strDBConnect.Find(':')!=-1)//绝对路径
	{
		strRet = m_strDBConnect;
	}
	else
	{
		strRet= gGetExeFileDir() + "model\\"+m_strDBConnect;
	}
	return strRet;
}
void CDlgPropRuleRun::OnButtonConstring() 
{
	// TODO: 弹出设置的对话框进行设置
	CFileDialog dlg(TRUE,".mdb", NULL, OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR, "Rule Files (*.mdb)|*.mdb||",NULL);   
	int   iReturn  =dlg.DoModal();   
	if(iReturn == IDOK)
	{     
		POSITION   pos   =   dlg.GetStartPosition(); 
		CString strFileName;
		if(pos   !=   NULL)   
		{   
			strFileName = dlg.GetNextPathName(pos);
			if(EstablishMDBConnectPtr(strFileName))
			{
				m_bCon=true;
				m_strDBConnect=strFileName;
			}
			else
			{
				m_bCon=false;
			}
		}
	}
	UpdateData(false);
}
BOOL CDlgPropRuleRun::EstablishMDBConnectPtr(CString strFileName)
{
	BOOL bResult = FALSE;
	try
	{
		HRESULT hr;
		hr =m_pConRule.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			CString strSqlConn;
			strSqlConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+strFileName;
			hr =m_pConRule->Open(_bstr_t(strSqlConn), "", "", adModeUnknown);
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format("连接MDB数据库错误:%s",e.ErrorMessage());
	    AfxMessageBox(temp);
	}
	return bResult;
}

void CDlgPropRuleRun::OnButtonSelRuleID() 
{
	// TODO: Add your control notification handler code here
	if(m_strDBConnect=="")
	{
		AfxMessageBox("请先设置数据源连接，再进行此操作！");
		return;
	}
	if(!m_bCon)
	{
		if(EstablishMDBConnectPtr(getABSDirRuleDB()))
		{
			m_bCon=true;
		}
		else
		{
			m_bCon=false;
			return;
		}
	}
	else
	{
		m_pConRule->Close();
		m_bCon=false;
		if(EstablishMDBConnectPtr(getABSDirRuleDB()))
		{
			m_bCon=true;
		}
		else
		{
			return;
		}
	}
	CDlgRuleFBList dlg;
	if(IDOK==dlg.DoModal())
	{
		m_strRuleDBName=dlg.m_strRuleDBName;
		m_strRuleDBDesp=dlg.m_strRuleDBDesp;
		readFBVar();
		displayFBVar();
		UpdateData(false);
	}
}
void CDlgPropRuleRun::readFBVar()
{
	nameFArr.RemoveAll();
	despFArr.RemoveAll();
	tagBeInFArr.RemoveAll();
	
	nameBArr.RemoveAll();
	despBArr.RemoveAll();
	tagBeInBArr.RemoveAll();
	
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("%s_para",m_strRuleDBName);
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` order by var_sequence asc ",tableName);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)(this->m_pConRule),true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	while(!m_pRec->adoEOF)
	{
		CString strFuncType=(char *)_bstr_t(m_pRec->GetCollect("func_Type"));
		if(strFuncType=="FRONT")//前件
		{
			nameFArr.Add((char *)_bstr_t(m_pRec->GetCollect("var_name")));
			despFArr.Add((char *)_bstr_t(m_pRec->GetCollect("var_desp")));	
			tagBeInFArr.Add(1);
		}
		else//后件
		{
			nameBArr.Add((char *)_bstr_t(m_pRec->GetCollect("var_name")));
			despBArr.Add((char *)_bstr_t(m_pRec->GetCollect("var_desp")));
			tagBeInBArr.Add(1);
		}
		m_pRec->MoveNext();
	}
}
void CDlgPropRuleRun::displayFBVar()
{
	pDlgFVar->pNameArr=&nameFArr;
	pDlgFVar->pDespArr=&despFArr;
	pDlgFVar->pTagBeInArr=&tagBeInFArr;
	pDlgFVar->InitListBoxHead();
	pDlgFVar->FillListBoxBody();

	pDlgBVar->pNameArr=&nameBArr;
	pDlgBVar->pDespArr=&despBArr;
	pDlgBVar->pTagBeInArr=&tagBeInBArr;
	pDlgBVar->InitListBoxHead();
	pDlgBVar->FillListBoxBody();
}
void CDlgPropRuleRun::initTabCtrl()
{
	CRect rc;
	m_TabList.GetClientRect(rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 1;

	if(pDlgFVar!=NULL)
	{
		delete pDlgFVar;
		pDlgFVar=NULL;
	}
	if(pDlgBVar!=NULL)
	{
		delete pDlgBVar;
		pDlgBVar=NULL;
	}
	this->m_TabList.InsertItem(0,_T("前件变量"));
	this->m_TabList.InsertItem(1,_T("后件变量"));
	pDlgFVar=new CDlgFBVar;
	pDlgBVar=new CDlgFBVar;
	
	pDlgFVar->Create(IDD_DIALOG_RULE_FBVAR,&m_TabList);//必须执行create
	pDlgFVar->MoveWindow(rc.left,rc.top,rc.Width()-10,rc.Height()-10);
	pDlgFVar->m_List.MoveWindow(rc.left,rc.top,rc.Width()-11,rc.Height()-11);
	pDlgFVar->ShowWindow(SW_SHOW);
	
	pDlgBVar->Create(IDD_DIALOG_RULE_FBVAR,&m_TabList);//必须执行create
	pDlgBVar->MoveWindow(rc.left,rc.top,rc.Width()-10,rc.Height()-10);
	pDlgBVar->m_List.MoveWindow(rc.left,rc.top,rc.Width()-11,rc.Height()-11);
	pDlgBVar->ShowWindow(SW_HIDE);
}

void CDlgPropRuleRun::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(pDlgFVar!=NULL)
	{
		delete pDlgFVar;
		pDlgFVar=NULL;
	}
	if(pDlgBVar!=NULL)
	{
		delete pDlgBVar;
		pDlgBVar=NULL;
	}
	CDialog::OnClose();
}

void CDlgPropRuleRun::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(0==m_TabList.GetCurSel())
	{
		pDlgFVar->ShowWindow(SW_SHOW);
		pDlgBVar->ShowWindow(SW_HIDE);
	}
	else
	{
		pDlgFVar->ShowWindow(SW_HIDE);
		pDlgBVar->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CDlgPropRuleRun::OnOK() 
{
	// TODO: Add extra validation here
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()==1)//前向推导选定
		m_iTagToFOrB=0;
	else
		m_iTagToFOrB=1;
	if(((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck()==1)//前向推导选定
		m_iTagWeightOrTime=0;
	else
		m_iTagWeightOrTime=1;
	CDialog::OnOK();
}
