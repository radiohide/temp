// DlgMCtrlDay.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlDay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDay dialog


CDlgMCtrlDay::CDlgMCtrlDay(CWnd* pParent /*=NULL*/)
	: CDlgMCtrlType(CDlgMCtrlDay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMCtrlDay)
	m_strTypeName="ÿ�ռƻ�����";
	//}}AFX_DATA_INIT
}


void CDlgMCtrlDay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMCtrlDay)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMCtrlDay, CDialog)
	//{{AFX_MSG_MAP(CDlgMCtrlDay)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDay message handlers

BOOL CDlgMCtrlDay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	InitListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMCtrlDay::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[4]={"���","����ʱ��","ʱ������","ʱ����յ�"};
					//�б��ӵı�ͷ
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,140);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,140);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
}
void CDlgMCtrlDay::InitListBoxBody()
{
	int nItem=0;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime.GetSize();i++)
	{
		pMCtrlOffsetSE=((MCtrlDay*)m_pMCtrlType)->m_ArrOffsetSE[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);

		m_List.SetItemText(nItem,1,((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime[i]);
		m_List.SetItemText(nItem,2,pMCtrlOffsetSE->GetSTimeStrDesp());
		m_List.SetItemText(nItem,3,pMCtrlOffsetSE->GetETimeStrDesp());
		nItem++;
	}
	CString str;
	str.Format("%d",nItem);
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(str);
}
void CDlgMCtrlDay::ShowCtrlData()
{
	InitListBoxHead();
	InitListBoxBody();
}
void CDlgMCtrlDay::SetCtrlData()
{
	
}

void CDlgMCtrlDay::OnButtonAdd()
{
	// TODO: Add your control notification handler code here
	DlgSetTimeAndSETime dlg;
	CString strTime="";
	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);
	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		strTime=dlg.m_strTime;
		((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime.Add(strTime);
		((MCtrlDay*)m_pMCtrlType)->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
		InitListBoxHead();
		InitListBoxBody();
	}
	else
	{
		delete pMCtrlOffsetSE;
		pMCtrlOffsetSE=NULL;
	}
}

void CDlgMCtrlDay::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("��ѡ��Ҫ�༭�ļ�¼!");
		return;
	}
	DlgSetTimeAndSETime dlg;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	CString strTime="";
	
	int index =this->m_List.GetNextSelectedItem(pos);
	pMCtrlOffsetSE=((MCtrlDay*)m_pMCtrlType)->m_ArrOffsetSE[index];
	strTime=((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime[index];
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);
	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime[index]=dlg.m_strTime;
		InitListBoxHead();
		InitListBoxBody();
	}
	else
	{
		delete pMCtrlOffsetSE;
		pMCtrlOffsetSE=NULL;
	}
}

void CDlgMCtrlDay::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	MCtrlOffsetSE *pMCtrlOffsetSE;
	CString strTime="";
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("��ѡ��Ҫɾ���ļ�¼!");
		return;
	}
	
	if(IDOK==AfxMessageBox("ȷ��Ҫɾ��ѡ����¼��",MB_OKCANCEL))
	{
		CArray<int,int> selIndexArr;
		while(pos!=NULL)
		{
			int index =this->m_List.GetNextSelectedItem(pos);
			selIndexArr.Add(index);
		}
		for(int i=selIndexArr.GetSize()-1;i>=0;i--)
		{
			pMCtrlOffsetSE=((MCtrlDay*)m_pMCtrlType)->m_ArrOffsetSE[selIndexArr[i]];
			if(pMCtrlOffsetSE!=NULL)
			{
				delete pMCtrlOffsetSE;
				pMCtrlOffsetSE=NULL;
			}
			((MCtrlDay*)m_pMCtrlType)->m_ArrOffsetSE.RemoveAt(selIndexArr[i]);
			((MCtrlDay*)m_pMCtrlType)->m_ArrRunTime.RemoveAt(selIndexArr[i]);
		}
		InitListBoxHead();
		InitListBoxBody();
	}
}
