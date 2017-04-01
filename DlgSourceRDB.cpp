// DlgSourceRDB.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgSourceRDB.h"
#include "DlgOneSourceRDB.h"
#include "EPerfManageDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

class CEPerfManageDoc;
CDlgSourceRDB::CDlgSourceRDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSourceRDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSourceRDB)
	m_bBeEdit=FALSE;
	m_StrModelName="";
	//}}AFX_DATA_INIT
}


void CDlgSourceRDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSourceRDB)
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSourceRDB, CDialog)
	//{{AFX_MSG_MAP(CDlgSourceRDB)
	ON_BN_CLICKED(IDC_BUTTON_APP, OnButtonApp)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSourceRDB message handlers

BOOL CDlgSourceRDB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead(0);
	InitListBoxBody(0);//初始化为显示本地
	m_iSelIndex=0;
	GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);
	CString strTitleName;
	strTitleName.Format("关系数据源管理--%s",this->m_StrModelName);
	this->SetWindowText(strTitleName);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgSourceRDB::InitListBoxHead(int tagLocalOrRemote)//tagLocalOrRemote:0,local;1:Remote;
{
	m_ListCtrl.DeleteAllItems();
	while(m_ListCtrl.DeleteColumn(0));

	DWORD liststyle=m_ListCtrl.GetExtendedStyle();//获取列表控件的扩展风格
	m_ListCtrl.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[8]={"序号","数据源名称","数据源类型","IP","数据库名称","数据库用户名","数据库密码","文件名"};
					//列表视的表头
	m_ListCtrl.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_ListCtrl.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);	
	m_ListCtrl.InsertColumn (2, Field[2], LVCFMT_LEFT, 80);
	m_ListCtrl.InsertColumn (3, Field[3], LVCFMT_LEFT, 80);
	m_ListCtrl.InsertColumn (4, Field[4], LVCFMT_LEFT, 120);	
	m_ListCtrl.InsertColumn (5, Field[5], LVCFMT_LEFT, 120);
	m_ListCtrl.InsertColumn (6, Field[6], LVCFMT_LEFT, 80);
	m_ListCtrl.InsertColumn (7, Field[7], LVCFMT_LEFT, 120);	
}
void CDlgSourceRDB::InitListBoxBody(int tagLocalOrRemote)//tagLocalOrRemote:0,local;1:Remote;
{
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
	pRecord.CreateInstance(__uuidof(Recordset));
	
	CString strSQL;
	CString strTable(_T("SourceRDB"));
	
	strSQL.Format("select * from %s Where ModelName='%s' order by ID asc",strTable,m_StrModelName);
	try
	{
		pRecord->Open((const char*)strSQL,theApp.m_pConLocalModel.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);	
	}
   	catch(_com_error e)
	{
		//pRecord->Close() ;
		return;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	int nItem =0;
	while(!pRecord->adoEOF )
	{
		CString RDBType,IP,RDBName,RDBUser,RDBPass,FileName,SourceName;
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("RDBType");
			if(Tempvalue.vt != VT_NULL) RDBType = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("IP");
			if(Tempvalue.vt != VT_NULL) IP = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBName");
			if(Tempvalue.vt != VT_NULL) RDBName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBUser");
			if(Tempvalue.vt != VT_NULL) RDBUser = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBPass");
			if(Tempvalue.vt != VT_NULL) RDBPass = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("FileName");
			if(Tempvalue.vt != VT_NULL) FileName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SourceName");
			if(Tempvalue.vt != VT_NULL) SourceName = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			CString str;
			str.Format("%d",nItem+1);
			m_ListCtrl.InsertItem(nItem,str);
			m_ListCtrl.SetItemText(nItem,1,SourceName);
			m_ListCtrl.SetItemText(nItem,2,RDBType);
			m_ListCtrl.SetItemText(nItem,3,IP);
			m_ListCtrl.SetItemText(nItem,4,RDBName);
			m_ListCtrl.SetItemText(nItem,5,RDBUser);
			m_ListCtrl.SetItemText(nItem,6,RDBPass);
			m_ListCtrl.SetItemText(nItem,7,FileName);
			nItem++;
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}

void CDlgSourceRDB::OnButtonApp() 
{
	// TODO: Add your control notification handler code here
	SaveSourceToDB();
	ReConRDBSource();
	m_bBeEdit=FALSE;
	GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);
}
void CDlgSourceRDB::ReConRDBSource()
{
	CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
	if(pDoc)
	{
		if(pDoc->m_pModel)
		{
			pDoc->m_pModel->CloseAllSourceRDBCon();
			pDoc->m_pModel->EstablishSourceRDBConnect(theApp.m_pConLocalModel);
		}
	}
}
void CDlgSourceRDB::SaveSourceToDB()
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("SourceRDB");
	strSQL.Format("delete from %s where ModelName='%s'",strTable,m_StrModelName);
	try	
	{
		if(theApp.m_bModelLocalConOK)
		{
			theApp.m_pConLocalModel->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
		else
		{
			AfxMessageBox("本地模型未连接！");
			return;
		}
	}
	catch(_com_error e)
	{
		return ;
	}
	for(int i=0;i<this->m_ListCtrl.GetItemCount();i++)
	{
		CString strSourceName=m_ListCtrl.GetItemText(i,1);
		CString strRDBType=m_ListCtrl.GetItemText(i,2);
		CString strIP=m_ListCtrl.GetItemText(i,3);
		CString strRDBName=m_ListCtrl.GetItemText(i,4);
		CString strUserName=m_ListCtrl.GetItemText(i,5);
		CString strRDBPass=m_ListCtrl.GetItemText(i,6);
		CString strFileName=m_ListCtrl.GetItemText(i,7);

		strSQL.Format("INSERT INTO %s(ModelName,SourceName,RDBType,IP,RDBName,RDBUser,RDBPass,FileName)values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",
			strTable,m_StrModelName,strSourceName,strRDBType,strIP,strRDBName,strUserName,strRDBPass,strFileName);
		try	
		{
			if(theApp.m_bModelLocalConOK)
				theApp.m_pConLocalModel->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
		catch(_com_error e)
		{
			continue ;
		}
	}
}
void CDlgSourceRDB::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bBeEdit)
	{
		SaveSourceToDB();
		ReConRDBSource();
	}
	CDialog::OnOK();
}

void CDlgSourceRDB::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgSourceRDB::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgOneSourceRDB dlg;
	if(IDOK==dlg.DoModal())
	{
		int nItem=m_ListCtrl.GetItemCount();
		CString str;
		str.Format("%d",nItem+1);
		m_ListCtrl.InsertItem(nItem,str);
		m_ListCtrl.SetItemText(nItem,1,dlg.m_strSourceName);
		m_ListCtrl.SetItemText(nItem,2,dlg.m_strRDBType);
		m_ListCtrl.SetItemText(nItem,3,dlg.m_strIP);
		m_ListCtrl.SetItemText(nItem,4,dlg.m_strRDBName);
		m_ListCtrl.SetItemText(nItem,5,dlg.m_strUserName);
		m_ListCtrl.SetItemText(nItem,6,dlg.m_strRDBPass);
		m_ListCtrl.SetItemText(nItem,7,dlg.m_strFileName);
		m_bBeEdit=true;
		GetDlgItem(IDC_BUTTON_APP)->EnableWindow(true);
	}
}

void CDlgSourceRDB::OnButtonDel()
{
	// TODO: Add your control notification handler code here
	CArray<int,int> delIndexArr;
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index=m_ListCtrl.GetNextSelectedItem(pos);
		delIndexArr.Add(index);
	}
	for(int i=delIndexArr.GetSize()-1;i>=0;i--)
	{
		m_ListCtrl.DeleteItem(delIndexArr[i]);
		m_bBeEdit=true;
		GetDlgItem(IDC_BUTTON_APP)->EnableWindow(true);
	}
}

void CDlgSourceRDB::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	if(pos)
	{
		CDlgOneSourceRDB dlg;
		int index=m_ListCtrl.GetNextSelectedItem(pos);
		dlg.m_strSourceName=m_ListCtrl.GetItemText(index,1);
		dlg.m_strRDBType=m_ListCtrl.GetItemText(index,2);
		dlg.m_strIP=m_ListCtrl.GetItemText(index,3);
		dlg.m_strRDBName=m_ListCtrl.GetItemText(index,4);
		dlg.m_strUserName=m_ListCtrl.GetItemText(index,5);
		dlg.m_strRDBPass=m_ListCtrl.GetItemText(index,6);
		dlg.m_strFileName=m_ListCtrl.GetItemText(index,7);
		if(IDOK==dlg.DoModal())
		{
			m_ListCtrl.SetItemText(index,1,dlg.m_strSourceName);
			m_ListCtrl.SetItemText(index,2,dlg.m_strRDBType);
			m_ListCtrl.SetItemText(index,3,dlg.m_strIP);
			m_ListCtrl.SetItemText(index,4,dlg.m_strRDBName);
			m_ListCtrl.SetItemText(index,5,dlg.m_strUserName);
			m_ListCtrl.SetItemText(index,6,dlg.m_strRDBPass);
			m_ListCtrl.SetItemText(index,7,dlg.m_strFileName);
			m_bBeEdit=true;
			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(true);
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的数据源再进行操作!");
	}
}

void CDlgSourceRDB::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}
