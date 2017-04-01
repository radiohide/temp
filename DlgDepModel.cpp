// DlgDepModel.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgDepModel.h"
#include "DBOperator.h"
#include "DlgIPPortData.h"
#include "SocketManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDepModel dialog

extern bool g_SocketInit(CSocketManage *&pSocket,const char* IP,int port);
extern void g_SocketExit(CSocketManage *&pSocket);

CDlgDepModel::CDlgDepModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDepModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDepModel)
	beEdit=false;
	//}}AFX_DATA_INIT
}


void CDlgDepModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDepModel)
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDepModel, CDialog)
	//{{AFX_MSG_MAP(CDlgDepModel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDepModel message handlers

void CDlgDepModel::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_ListCtrl.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("请选择一个模型。");
		 return;
	}
	CString strName=m_ListCtrl.GetItemText(nSelectedItem,2);
	CString strIP= m_ListCtrl.GetItemText(nSelectedItem,3);
	CString strPort= m_ListCtrl.GetItemText(nSelectedItem,4);
	CDlgIPPortData dlg;
	dlg.m_StrIP=strIP;
	dlg.m_StrPort=strPort;
	dlg.m_strModeName=strName;
	if(IDOK==dlg.DoModal())
	{
		if((strIP!=dlg.m_StrIP)||(strPort!=dlg.m_StrPort))
		{
			strIPChangeArr.Add(strIP);
			strIPChangeArr.Add(dlg.m_StrIP);
			m_ListCtrl.SetItemText(nSelectedItem,3,dlg.m_StrIP);
			m_ListCtrl.SetItemText(nSelectedItem,4,dlg.m_StrPort);
			beEdit=true;
		}
	}
}

BOOL CDlgDepModel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	InitListBoxBody(1);//初始化为显示本地

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgDepModel::InitListBoxHead()
{
	m_ListCtrl.DeleteAllItems();
	while(m_ListCtrl.DeleteColumn(0));

	DWORD liststyle=m_ListCtrl.GetExtendedStyle();//获取列表控件的扩展风格
	m_ListCtrl.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","模型描述","模型名称","模型运行计算站IP","端口号"};
					//列表视的表头
	m_ListCtrl.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_ListCtrl.InsertColumn (1, Field[1], LVCFMT_LEFT, 150);	
	m_ListCtrl.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn (3, Field[3], LVCFMT_LEFT, 120);
	m_ListCtrl.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
}
void CDlgDepModel::InitListBoxBody(int tagLocalOrRemote)//tagLocalOrRemote:0,local;1:Remote;
{
	int nItem=0;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
	VARIANT_BOOL bBOF ;
	pRecord.CreateInstance(__uuidof(Recordset));
	
	CString strSQL;
	CString strTable(_T("ModelList"));
	
	strSQL.Format("select * from %s ORDER BY id",strTable);
	try
	{
		if(tagLocalOrRemote==0)
		{
			pRecord->Open((const char*)strSQL,theApp.m_pConLocalModel.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		}
		else if(tagLocalOrRemote==1)
		{
			pRecord->Open((const char*)strSQL,theApp.m_pConRemoteModel.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		}
	}
   	catch(_com_error e)
	{
		//pRecord->Close() ;
		return;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	while(!pRecord->adoEOF )
	{
		CString strName;
		CString strDesp;
		CString strIP;
		CString strPort;
		BOOL bThisOk = TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("model_name");
			strName = (LPCSTR)_bstr_t(Tempvalue);
			
			Tempvalue = pRecord->GetCollect("model_desp");
			strDesp = (LPCSTR)_bstr_t(Tempvalue);
			
			Tempvalue = pRecord->GetCollect("model_IP");
			strIP = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("model_port");
			strPort = (LPCSTR)_bstr_t(Tempvalue);
			
		}
		catch(_com_error e)
		{
			bThisOk = FALSE;
			break;
		}
		if(bThisOk)
		{
			CString str;
			str.Format("%d",nItem+1);
			m_ListCtrl.InsertItem(nItem,str);
			m_ListCtrl.SetItemText(nItem,1,strDesp);
			m_ListCtrl.SetItemText(nItem,2,strName);
			m_ListCtrl.SetItemText(nItem,3,strIP);
			m_ListCtrl.SetItemText(nItem,4,strPort);
			nItem++;
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
void CDlgDepModel::WriteData(_ConnectionPtr &pCon)
{
	CString strModelName;//do

	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("ModelList");
	for(int i=0;i<this->m_ListCtrl.GetItemCount();i++)
	{
		try	
		{
			CString strName = m_ListCtrl.GetItemText(i,2);
			CString strDesp = m_ListCtrl.GetItemText(i,1);
			CString strIP= m_ListCtrl.GetItemText(i,3);
			CString strPort= m_ListCtrl.GetItemText(i,4);

			strSQL.Format("UPDATE `%s`SET model_desp='%s',model_IP='%s' ,model_port='%s' WHERE model_name='%s'",
				strTable,strDesp,strIP,strPort,strName);
			pCon->Execute((_bstr_t)strSQL,0,adCmdText);
		}
		catch(_com_error e)
		{
			return ;
		}
	}
	//pRecord->Close();
}
void CDlgDepModel::RefurbishMangerToolList()
{
	CStringArray strDiffChangeArr;
	for(int i=0;i<strIPChangeArr.GetSize();i++)
	{
		bool bFind=false;
		for(int j=0;j<strDiffChangeArr.GetSize();j++)
		{
			if(strIPChangeArr[i]==strDiffChangeArr[j])
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
		{
			strDiffChangeArr.Add(strIPChangeArr[i]);	
		}
	}
	for(i=0;i<strDiffChangeArr.GetSize();i++)
	{
		CString strIP=strDiffChangeArr[i];
		CSocketManage *pSocket=NULL;
		if(g_SocketInit(pSocket,strIP,11650))
		{
			char lbuf[1024];	
			memset(lbuf,0,1024);
			
			lbuf[0] = '*';	//安全传输符号
			*(int *)(lbuf+1) = 6;//+sNodeName.GetLength();
			lbuf[5] = 'B';	//协议号
			
			pSocket->Send(lbuf,*(int *)(lbuf+1));
			int ret = pSocket->Receive(lbuf,1024);
			if(ret > 0)
			{
				if(lbuf[0] != '*')
				{
					return;
				}
				if(lbuf[5] != 'B')
				{
					return;
				}
				//CString str(lbuf[6]);
				//CString sMsg = "返回结果是"+str;
				//AfxMessageBox(sMsg);
			}
		}
		g_SocketExit(pSocket);
	}
}
void CDlgDepModel::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(beEdit)
	{
		WriteData(theApp.m_pConRemoteModel);
		RefurbishMangerToolList();
		beEdit=false;
		strIPChangeArr.RemoveAll();
	}
}
void CDlgDepModel::OnOK() 
{
	// TODO: Add extra validation here
	if(beEdit)
	{
		WriteData(theApp.m_pConRemoteModel);
		RefurbishMangerToolList();
		beEdit=false;
		strIPChangeArr.RemoveAll();
	}
	CDialog::OnOK();
}

void CDlgDepModel::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgDepModel::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}
