// DlgMCtrlDuty.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlDuty.h"
#include "MCtrlDuty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDuty dialog


CDlgMCtrlDuty::CDlgMCtrlDuty(CWnd* pParent /*=NULL*/)
	: CDlgMCtrlType(CDlgMCtrlDuty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMCtrlDuty)
	m_strTypeName="按值计划任务";
	//}}AFX_DATA_INIT
}


void CDlgMCtrlDuty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMCtrlDuty)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMCtrlDuty, CDialog)
	//{{AFX_MSG_MAP(CDlgMCtrlDuty)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlDuty message handlers
BOOL CDlgMCtrlDuty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	InitListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMCtrlDuty::ShowCtrlData()
{
	InitListBoxHead();
	InitListBoxBody();
}
void CDlgMCtrlDuty::SetCtrlData()
{
	
}
void CDlgMCtrlDuty::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","值","计算时刻","时间段起点","时间段终点"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,140);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,140);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,140);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,140);
}
void CDlgMCtrlDuty::InitListBoxBody()
{
	int nItem=0;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum.GetSize();i++)
	{
		pMCtrlOffsetSE=((MCtrlDuty*)m_pMCtrlType)->m_ArrOffsetSE[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);

		m_List.SetItemText(nItem,1,((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum[i]);
		if(((MCtrlDuty*)m_pMCtrlType)->m_ArrDutySOrE[i]==0)//
		{
			m_List.SetItemText(nItem,2,"该值开始时间");
		}
		else
			m_List.SetItemText(nItem,2,"该值结束时间");

		m_List.SetItemText(nItem,3,pMCtrlOffsetSE->GetSTimeStrDesp());
		m_List.SetItemText(nItem,4,pMCtrlOffsetSE->GetETimeStrDesp());
		nItem++;
	}
	CString str;
	str.Format("%d",nItem);
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(str);
}

void CDlgMCtrlDuty::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	DlgSetTimeAndSETime dlg;
	CString strTime="";
	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	((MCtrlDuty*)m_pMCtrlType)->ReadAllDuty();
	dlg.SetBeDutyStyle();
	dlg.SetAllDuty(((MCtrlDuty*)m_pMCtrlType)->m_ArrAllDuty);
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);
	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		CString dutyName=dlg.m_strDutyName;
		int dutySOrE=dlg.m_iDutySOrE;
		((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum.Add(dutyName);
		((MCtrlDuty*)m_pMCtrlType)->m_ArrDutySOrE.Add(dutySOrE);
		((MCtrlDuty*)m_pMCtrlType)->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
		InitListBoxHead();
		InitListBoxBody();
	}
	else
	{
		delete pMCtrlOffsetSE;
		pMCtrlOffsetSE=NULL;
	}
}

void CDlgMCtrlDuty::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("请选择要编辑的记录!");
		return;
	}
	MCtrlOffsetSE *pMCtrlOffsetSE;
	CString strTime="";
	
	int index =this->m_List.GetNextSelectedItem(pos);
	pMCtrlOffsetSE=((MCtrlDuty*)m_pMCtrlType)->m_ArrOffsetSE[index];
	CString dutyName=((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum[index];
	int dutySOrE=((MCtrlDuty*)m_pMCtrlType)->m_ArrDutySOrE[index];

	DlgSetTimeAndSETime dlg;
	((MCtrlDuty*)m_pMCtrlType)->ReadAllDuty();
	dlg.SetBeDutyStyle();
	dlg.SetAllDuty(((MCtrlDuty*)m_pMCtrlType)->m_ArrAllDuty);
	dlg.SetDutyTimeData(dutyName,dutySOrE);
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);

	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum[index]=dlg.m_strDutyName;
		((MCtrlDuty*)m_pMCtrlType)->m_ArrDutySOrE[index]=dlg.m_iDutySOrE;
		InitListBoxHead();
		InitListBoxBody();
	}
	else
	{
		delete pMCtrlOffsetSE;
		pMCtrlOffsetSE=NULL;
	}
}

void CDlgMCtrlDuty::OnButtonDel() 
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
			pMCtrlOffsetSE=((MCtrlDuty*)m_pMCtrlType)->m_ArrOffsetSE[selIndexArr[i]];
			if(pMCtrlOffsetSE!=NULL)
			{
				delete pMCtrlOffsetSE;
				pMCtrlOffsetSE=NULL;
			}
			((MCtrlDuty*)m_pMCtrlType)->m_ArrOffsetSE.RemoveAt(selIndexArr[i]);
			((MCtrlDuty*)m_pMCtrlType)->m_ArrDutyNum.RemoveAt(selIndexArr[i]);
			((MCtrlDuty*)m_pMCtrlType)->m_ArrDutySOrE.RemoveAt(selIndexArr[i]);
		}
		InitListBoxHead();
		InitListBoxBody();
	}
}
