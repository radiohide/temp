// DlgStatVarSel.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgStatVarSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatVarSel dialog


CDlgStatVarSel::CDlgStatVarSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatVarSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatVarSel)
	m_iVarType=0;//1:accum;2enum;3aver;4impt
	m_pModel=NULL;
	m_iEditSet = 0;
	m_iEditUnit = 0;
	//}}AFX_DATA_INIT
}


void CDlgStatVarSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatVarSel)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT_SET, m_iEditSet);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_iEditUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatVarSel, CDialog)
	//{{AFX_MSG_MAP(CDlgStatVarSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStatVarSel message handlers
void CDlgStatVarSel::initListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","测点名称","描述","单位"};
					//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 150);	
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 60);
}
void CDlgStatVarSel::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<this->m_strVarNameArr.GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,m_strVarNameArr[i]);
		m_List.SetItemText(nItem,2,this->m_strVarDespArr[i]);
		m_List.SetItemText(nItem,3,this->m_strVarUnitArr[i]);
		nItem++;
	}
}
BOOL CDlgStatVarSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	readVarList();
	initListBoxHead();
	initListBoxBody();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgStatVarSel::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s",strTableName);
	try
	{
		pRecord->Open((const char*)strSQL,pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	} 
	return TRUE;
}
void CDlgStatVarSel::readVarList()
{
	m_strVarNameArr.RemoveAll();
	m_strVarUnitArr.RemoveAll();
	m_strVarDespArr.RemoveAll();

	if(m_pModel==NULL)
		return;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	CString m_strTableName="st_system_point_config";
	if(!gIsTableExist(m_pConRDB, m_strTableName))
	{
		AfxMessageBox("当前数据源中不存相关点表，请重新选择数据源！");
		return;
	}
	_RecordsetPtr m_pRec;
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strDBType.MakeUpper();
		if(strDBType=="MYSQL")
		{
			strSql.Format("select  * from %s  where point_type=%d;",
				m_strTableName,m_iVarType);
		}
		else if(strDBType=="MDB")
		{
			strSql.Format("select * from %s  where point_type=%d;",
				m_strTableName,m_iVarType);
		}
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str=e->ErrorMessage();	//AfxMessageBox(e->ErrorMessage());   
	}  
	while(!m_pRec->adoEOF)
	{
		CString strVarName =(char *)_bstr_t(m_pRec->GetCollect("point_name"));
		CString strVarDesc =(char *)_bstr_t(m_pRec->GetCollect("point_desc"));
		CString strVarUnit =(char *)_bstr_t(m_pRec->GetCollect("point_unit"));
		m_strVarNameArr.Add(strVarName);
		m_strVarUnitArr.Add(strVarUnit);
		m_strVarDespArr.Add(strVarDesc);
		m_pRec->MoveNext();
	}
}
void CDlgStatVarSel::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_strSelVarNameArr.RemoveAll();
	m_strSelVarUnitArr.RemoveAll();
	m_strSelVarDespArr.RemoveAll();

	POSITION pos=m_List.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index=m_List.GetNextSelectedItem(pos);
		CString strVarName=m_List.GetItemText(index,1);
		CString strVarDesp=m_List.GetItemText(index,2);
		CString strVarUnit=m_List.GetItemText(index,3);
		
		m_strSelVarNameArr.Add(strVarName);
		m_strSelVarDespArr.Add(strVarDesp);
		m_strSelVarUnitArr.Add(strVarUnit);
	}
	CDialog::OnOK();
}
