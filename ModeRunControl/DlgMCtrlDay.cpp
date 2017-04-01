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
	m_strTypeName="每日计划任务";
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

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[4]={"序号","计算时刻","时间段起点","时间段终点"};
					//列表视的表头
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
		AfxMessageBox("请选择要编辑的记录!");
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
		AfxMessageBox("请选择要删除的记录!");
		return;
	}
	
	if(IDOK==AfxMessageBox("确定要删除选定记录？",MB_OKCANCEL))
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
