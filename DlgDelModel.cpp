// DlgDelModel.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgDelModel.h"
#include "ModelData.h"
#include "ModelCalcControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDelModel dialog


CDlgDelModel::CDlgDelModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDelModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDelModel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDelModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDelModel)
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_MODESOURCE, m_ComBo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDelModel, CDialog)
	//{{AFX_MSG_MAP(CDlgDelModel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODESOURCE, OnSelchangeComboModesource)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDelModel message handlers

BOOL CDlgDelModel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitListBoxHead(0);
	InitListBoxBody(0);//��ʼ��Ϊ��ʾ����
	this->m_ComBo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgDelModel::InitListBoxHead(int tagLocalOrRemote)//tagLocalOrRemote:0,local;1:Remote;
{
	m_ListCtrl.DeleteAllItems();
	while(m_ListCtrl.DeleteColumn(0));

	DWORD liststyle=m_ListCtrl.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_ListCtrl.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[5]={"���","ģ������","ģ������","ģ�����м���վIP","�˿ں�"};
					//�б��ӵı�ͷ
	m_ListCtrl.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_ListCtrl.InsertColumn (1, Field[1], LVCFMT_LEFT, 150);	
	m_ListCtrl.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	if(tagLocalOrRemote==1)
	{
		m_ListCtrl.InsertColumn (3, Field[3], LVCFMT_LEFT, 120);
		m_ListCtrl.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
	}
}
void CDlgDelModel::InitListBoxBody(int tagLocalOrRemote)//tagLocalOrRemote:0,local;1:Remote;
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
			if(tagLocalOrRemote==1)
			{
				m_ListCtrl.SetItemText(nItem,3,strIP);
				m_ListCtrl.SetItemText(nItem,4,strPort);
			}
			nItem++;
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}

void CDlgDelModel::OnSelchangeComboModesource() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selIndex=m_ComBo.GetCurSel ();
	if(selIndex==0)
	{
		InitListBoxHead(0);
		if(theApp.m_bModelLocalConOK)
			InitListBoxBody(0);//��ʾ����
		else
			AfxMessageBox("����ģ��δ���ӣ�");
	}
	else if(selIndex==1)
	{
		InitListBoxHead(1);
		if(theApp.m_bModelRemoteConOK)
			InitListBoxBody(1);//��ʾԶ��
		else
			AfxMessageBox("Զ��ģ��δ���ӣ�");
	}
}

void CDlgDelModel::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDlgDelModel::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_ListCtrl.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("��ѡ��ģ�͡�");
		 return;
	}
	CString strSelModelName = m_ListCtrl.GetItemText(nSelectedItem,2);
	int selIndex=m_ComBo.GetCurSel ();
	CString str;
	str.Format("�Ƿ�Ҫɾ������Ϊ:'%s'��ģ�ͣ�",strSelModelName);
	int int_selOK=AfxMessageBox(str,MB_OKCANCEL);
	if(int_selOK==1)
	{
		if(selIndex==0)//ɾ������
		{
			ModelData modelDataLocal;//ɾ������ģ��
			modelDataLocal.ReadDate(theApp.m_pConLocalModel,strSelModelName);
			modelDataLocal.DelTheModelTables(theApp.m_pConLocalModel,strSelModelName);
			modelDataLocal.DelModelInModelListTable(theApp.m_pConLocalModel,strSelModelName);
			//ɾ����������
			ModelCalcControl mCalControl(strSelModelName);
			mCalControl.SetConPtr(theApp.m_pConLocalModel);
			mCalControl.DelCtrlDataInDB();
			InitListBoxHead(0);
			InitListBoxBody(0);//��ʾ����
		}
		else if(selIndex==1)//ɾ��Զ��
		{
			ModelData modelDataRemote;//ɾ��Զ��
			modelDataRemote.ReadDate(theApp.m_pConRemoteModel,strSelModelName);
			modelDataRemote.DelTheModelTables(theApp.m_pConRemoteModel,strSelModelName);
			modelDataRemote.DelModelInModelListTable(theApp.m_pConRemoteModel,strSelModelName);
			//ɾ����������
			ModelCalcControl mCalControl(strSelModelName);
			mCalControl.SetConPtr(theApp.m_pConRemoteModel);
			mCalControl.DelCtrlDataInDB();
			InitListBoxHead(1);
			InitListBoxBody(1);//��ʾԶ��
		}
	}
}
