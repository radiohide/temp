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
	m_strTypeName="���¼ƻ�����";
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
	SetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE,dwStyle);//�������οؼ��ķ��
    m_hRoot = m_ctlTree.InsertItem("�·�");//���ڵ�
	
	CArray<int,int> arrMonthArr;
	((MCtrlMonth*)m_pMCtrlType)->GetMonthNums(arrMonthArr);
	//һ���ӽڵ�
	for(int i=0;i<arrMonthArr.GetSize();i++)//���и�����
	{
		CString str;
		str.Format("%.2d��",arrMonthArr[i]+1);
		m_ctlTree.InsertItem(str,m_hRoot);
	}
	m_ctlTree.Expand(m_hRoot,TVE_EXPAND);//ֻչ��һ���ӽڵ�
	m_ctlTree.SetBkColor(RGB(255,255,255));//�������οؼ��ı���ɫ
	m_ctlTree.SetTextColor(RGB(0,0,0));//�����ı���ɫ
}
void CDlgMCtrlMonth::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[7]={"���","�·�","����","����ʱ��","ʱ������","ʱ����յ�","�ڲ����"};
					//�б��ӵı�ͷ
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
	if(item.hItem == m_hRoot)//����Ǹ�Ŀ¼����ʾ����������
	{
		InitListBoxHead();
		InitListBoxBody(12);
		return;
	}
	CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//��ȡ�����ı�
	CString strMNum=strNodeText.Left(2);
	int mNum=::atoi(strMNum)-1;
	InitListBoxHead();
	InitListBoxBody(mNum);
	*pResult = 0;
}
void CDlgMCtrlMonth::InitListBoxBody(int mInNum)//mInNum ��1�µ�12�£�0~11��ȫ��Ϊ12
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

		str.Format("%d��",mNum+1);
		m_List.SetItemText(nItem,1,str);

		str.Format("%2d��",((MCtrlMonth*)m_pMCtrlType)->m_ArrDay[i]+1);//do
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
		AfxMessageBox("��ѡ���·ݺ��ٽ�����ӣ�");
		return;
	}
	CArray<int,int> arrMonthArr;
	((MCtrlMonth*)m_pMCtrlType)->GetMonthNums(arrMonthArr);
	if(arrMonthArr.GetSize()==0)
	{
		AfxMessageBox("��ǰûѡ���·��޷�������ӣ�");
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
		if(strNodeText == "�·�")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
		{
			for(int i=0;i<arrMonthArr.GetSize();i++)//���и�����
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
			//CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//��ȡ�����ı�
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
		AfxMessageBox("��ѡ��Ҫ�༭�ļ�¼!");
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
		if(strNodeText == "�·�")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
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
		AfxMessageBox("��ѡ��Ҫɾ���ļ�¼!");
		return;
	}
	if(IDOK==AfxMessageBox("ȷ��Ҫɾ��ѡ����¼��",MB_OKCANCEL))
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
		if(strNodeText == "�·�")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
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
