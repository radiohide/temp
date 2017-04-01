// DlgMCtrlMonth.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlMonth.h"
#include "DlgSelMonth.h"
#include "DlgSetTimeAndSETime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlMonth dialog


CDlgMCtrlMonth::CDlgMCtrlMonth(CWnd* pParent /*=NULL*/)
	: CDlgMCtrlType(CDlgMCtrlMonth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMCtrlMonth)
	m_strTypeName="按月计划任务";
	//}}AFX_DATA_INIT
}


void CDlgMCtrlMonth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMCtrlMonth)
	DDX_Control(pDX, IDC_TREE_MONTH, m_ctlTree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMCtrlMonth, CDialog)
	//{{AFX_MSG_MAP(CDlgMCtrlMonth)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MONTH, OnSelchangedTreeMonth)
	ON_BN_CLICKED(IDC_BTNSELMONTH, OnBtnselmonth)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlMonth message handlers

BOOL CDlgMCtrlMonth::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTree();
	InitListBoxHead();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMCtrlMonth::InitTree()
{
	m_ctlTree.DeleteAllItems();

	DWORD dwStyle = GetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE,dwStyle);//设置树形控件的风格
    m_hRoot = m_ctlTree.InsertItem("月份");//根节点
	
	CArray<int,int> arrMonthArr;
	((MCtrlMonth*)m_pMCtrlType)->GetMonthNums(arrMonthArr);
	//一级子节点
	for(int i=0;i<arrMonthArr.GetSize();i++)//所有复合组
	{
		CString str;
		str.Format("%.2d月",arrMonthArr[i]+1);
		m_ctlTree.InsertItem(str,m_hRoot);
	}
	m_ctlTree.Expand(m_hRoot,TVE_EXPAND);//只展开一级子节点
	m_ctlTree.SetBkColor(RGB(255,255,255));//设置树形控件的背景色
	m_ctlTree.SetTextColor(RGB(0,0,0));//设置文本颜色
}
void CDlgMCtrlMonth::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[7]={"序号","月份","几号","计算时刻","时间段起点","时间段终点","内部编号"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,50);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,50);	
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,140);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,140);
	m_List.InsertColumn(5,Field[5],LVCFMT_LEFT,140);
	m_List.InsertColumn(6,Field[6],LVCFMT_LEFT,0);
}
void CDlgMCtrlMonth::ShowCtrlData()
{
	InitTree();
	InitListBoxHead();
}
void CDlgMCtrlMonth::SetCtrlData()
{
	
}

void CDlgMCtrlMonth::OnSelchangedTreeMonth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)//如果是根目录，显示周所有配置
	{
		InitListBoxHead();
		InitListBoxBody(12);
		return;
	}
	CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//获取活动结点文本
	CString strMNum=strNodeText.Left(2);
	int mNum=::atoi(strMNum)-1;
	InitListBoxHead();
	InitListBoxBody(mNum);
	*pResult = 0;
}
void CDlgMCtrlMonth::InitListBoxBody(int mInNum)//mInNum ：1月到12月：0~11；全部为12
{
	int nItem=0;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<((MCtrlMonth*)m_pMCtrlType)->m_ArrInIndex.GetSize();i++)
	{
		int mNum=((MCtrlMonth*)m_pMCtrlType)->m_ArrMonth[i];
		if(mInNum!=12)
		{
			if(mNum!=mInNum)
			{
				continue;
			}
		}
		if(((MCtrlMonth*)m_pMCtrlType)->m_ArrRunTime[i]=="")
			continue;
		pMCtrlOffsetSE=((MCtrlMonth*)m_pMCtrlType)->m_ArrOffsetSE[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);

		str.Format("%d月",mNum+1);
		m_List.SetItemText(nItem,1,str);

		str.Format("%2d号",((MCtrlMonth*)m_pMCtrlType)->m_ArrDay[i]+1);//do
		m_List.SetItemText(nItem,2,str);

		m_List.SetItemText(nItem,3,((MCtrlMonth*)m_pMCtrlType)->m_ArrRunTime[i]);
		m_List.SetItemText(nItem,4,pMCtrlOffsetSE->GetSTimeStrDesp());
		m_List.SetItemText(nItem,5,pMCtrlOffsetSE->GetETimeStrDesp());
		str.Format("%d",((MCtrlMonth*)m_pMCtrlType)->m_ArrInIndex[i]);
		m_List.SetItemText(nItem,6,str);
		nItem++;
	}
	CString str;
	str.Format("%d",nItem);
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(str);
	UpdateData(false);
}

void CDlgMCtrlMonth::OnBtnselmonth() 
{
	// TODO: Add your control notification handler code here
	CDlgSelMonth dlg;
	CArray<int,int> arrMonthArr;
	((MCtrlMonth*)m_pMCtrlType)->GetMonthNums(arrMonthArr);
	dlg.SetMonthNum(arrMonthArr);
	if(IDOK==dlg.DoModal())
	{
		((MCtrlMonth*)m_pMCtrlType)->SetNewMonthNums(dlg.m_mNumArr);
		InitTree();
		InitListBoxHead();
	}
}

void CDlgMCtrlMonth::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	HTREEITEM hItem = m_ctlTree.GetSelectedItem();
	CString strNodeText=m_ctlTree.GetItemText(hItem);
	if(hItem==NULL)
	{
		AfxMessageBox("请选择月份后再进行添加！");
		return;
	}
	CArray<int,int> arrMonthArr;
	((MCtrlMonth*)m_pMCtrlType)->GetMonthNums(arrMonthArr);
	if(arrMonthArr.GetSize()==0)
	{
		AfxMessageBox("当前没选择月份无法进行添加！");
		return;
	}
	CString strTime="";
	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	int *pDay=NULL;
	CString *pStrTime=NULL;
	MCtrlOffsetSE *pMCtrlOffsetSE2=NULL;
	DlgSetTimeAndSETime dlg;
	dlg.SetBeMonthStyle();
	dlg.SetMonthTheDay(-1);
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);
	if(IDOK==dlg.DoModal())
	{
		//TVITEM item = pNMTreeView->itemNew;
		if(strNodeText == "月份")//如果是根目录,在所有已有的每天添加。
		{
			for(int i=0;i<arrMonthArr.GetSize();i++)//所有复合组
			{
				((MCtrlMonth*)m_pMCtrlType)->GetTheMonthData(arrMonthArr[i],pDay,pStrTime,pMCtrlOffsetSE2);
				*pDay=dlg.m_iDay;
				*pMCtrlOffsetSE2=dlg.m_MCtrlOffsetSE;
				*pStrTime=dlg.m_strTime;
			}
			InitListBoxHead();
			InitListBoxBody(12);
		}
		else
		{
			//CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//获取活动结点文本
			CString strMNum=strNodeText.Left(2);
			int mNum=::atoi(strMNum)-1;
			((MCtrlMonth*)m_pMCtrlType)->GetTheMonthData(mNum,pDay,pStrTime,pMCtrlOffsetSE2);
			*pMCtrlOffsetSE2=dlg.m_MCtrlOffsetSE;
			*pStrTime=dlg.m_strTime;
			*pDay=dlg.m_iDay;
			InitListBoxHead();
			InitListBoxBody(mNum);
		}
	}
	delete pMCtrlOffsetSE;
	pMCtrlOffsetSE=NULL;
}

void CDlgMCtrlMonth::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("请选择要编辑的记录!");
		return;
	}
	MCtrlOffsetSE *pMCtrlOffsetSE=NULL;
	CString *pStrTime=NULL;
	int *pDay=NULL;
	int index =this->m_List.GetNextSelectedItem(pos);
	int inIndex=atoi(this->m_List.GetItemText(index,6));
	((MCtrlMonth*)m_pMCtrlType)->GetTheRecordDataByInIndex(inIndex,pDay,pStrTime,pMCtrlOffsetSE);
	
	DlgSetTimeAndSETime dlg;
	dlg.SetBeMonthStyle();
	dlg.SetMonthTheDay(*pDay);
	dlg.SetTimeAndOffSetSETime(*pStrTime,pMCtrlOffsetSE);
	
	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		*pStrTime=dlg.m_strTime;
		*pDay=dlg.m_iDay;
		HTREEITEM hItem = m_ctlTree.GetSelectedItem();
		CString strNodeText=m_ctlTree.GetItemText(hItem);
		if(strNodeText == "月份")//如果是根目录,在所有已有的每天添加。
		{
			InitListBoxHead();
			InitListBoxBody(12);
		}
		else
		{
			CString strMNum=strNodeText.Left(2);
			int mNum=::atoi(strMNum)-1;
			InitListBoxHead();
			InitListBoxBody(mNum);
		}
	}
}

void CDlgMCtrlMonth::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString strTime="";
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("请选择要删除的记录!");
		return;
	}
	if(IDOK==AfxMessageBox("确定要删除选定记录？",MB_OKCANCEL))
	{
		CArray<int,int> selInIndexArr;
		while(pos!=NULL)
		{
			int index =this->m_List.GetNextSelectedItem(pos);
			int inIndex=atoi(this->m_List.GetItemText(index,6));
			selInIndexArr.Add(inIndex);
		}
		for(int i=selInIndexArr.GetSize()-1;i>=0;i--)
		{
			((MCtrlMonth*)m_pMCtrlType)->DelRecordByInIndex(selInIndexArr[i]);	
		}
		HTREEITEM hItem = m_ctlTree.GetSelectedItem();
		CString strNodeText=m_ctlTree.GetItemText(hItem);
		if(strNodeText == "月份")//如果是根目录,在所有已有的每天添加。
		{
			InitListBoxHead();
			InitListBoxBody(12);
		}
		else
		{
			CString strMNum=strNodeText.Left(2);
			int mNum=::atoi(strMNum)-1;
			InitListBoxHead();
			InitListBoxBody(mNum);
		}
	}
}
