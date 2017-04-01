// DlgMCtrlWeek.cpp : implementation file
//

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlWeek.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlWeek dialog

CDlgMCtrlWeek::CDlgMCtrlWeek(CWnd* pParent /*=NULL*/)
	: CDlgMCtrlType(CDlgMCtrlWeek::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMCtrlWeek)
	m_strTypeName="ÿ�ܼƻ�����";
	pNMTreeView=NULL;
	//}}AFX_DATA_INIT
}


void CDlgMCtrlWeek::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMCtrlWeek)
	DDX_Control(pDX, IDC_TREE_WEEK, m_ctlTree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMCtrlWeek, CDialog)
	//{{AFX_MSG_MAP(CDlgMCtrlWeek)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_WEEK, OnSelchangedTreeWeek)
	ON_BN_CLICKED(IDC_BTNSELWEEK, OnBtnselweek)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlWeek message handlers

BOOL CDlgMCtrlWeek::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTree();
	InitListBoxHead();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMCtrlWeek::InitTree()
{
	m_ctlTree.DeleteAllItems();

	DWORD dwStyle = GetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE,dwStyle);//�������οؼ��ķ��
    m_hRoot = m_ctlTree.InsertItem("����");//���ڵ�
	
	CArray<int,int> arrWeekArr;
	((MCtrlWeek*)m_pMCtrlType)->GetWeekNums(arrWeekArr);
	//һ���ӽڵ�
	for(int i=0;i<arrWeekArr.GetSize();i++)//���и�����
	{
		CString str;
		str.Format("����%d",arrWeekArr[i]+1);
		m_ctlTree.InsertItem(str,m_hRoot);
	}
	m_ctlTree.Expand(m_hRoot,TVE_EXPAND);//ֻչ��һ���ӽڵ�
	m_ctlTree.SetBkColor(RGB(255,255,255));//�������οؼ��ı���ɫ
	m_ctlTree.SetTextColor(RGB(0,0,0));//�����ı���ɫ
}
void CDlgMCtrlWeek::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[6]={"���","����","����ʱ��","ʱ������","ʱ����յ�","�ڲ����"};
					//�б��ӵı�ͷ
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,140);	
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,140);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,140);
	m_List.InsertColumn(5,Field[5],LVCFMT_LEFT,0);
}
void CDlgMCtrlWeek::InitListBoxBody(int wInNum)//wNum ����һ��������0~6��ȫ��Ϊ7
{
	int nItem=0;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<((MCtrlWeek*)m_pMCtrlType)->m_ArrInIndex.GetSize();i++)
	{
		int wNum=((MCtrlWeek*)m_pMCtrlType)->m_ArrWeek[i];
		if(wInNum!=7)
		{
			if(wNum!=wInNum)
			{
				continue;
			}
		}
		if(((MCtrlWeek*)m_pMCtrlType)->m_ArrRunTime[i]=="")
			continue;
		pMCtrlOffsetSE=((MCtrlWeek*)m_pMCtrlType)->m_ArrOffsetSE[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);

		str.Format("����%d",wNum+1);
		m_List.SetItemText(nItem,1,str);
		m_List.SetItemText(nItem,2,((MCtrlWeek*)m_pMCtrlType)->m_ArrRunTime[i]);
		m_List.SetItemText(nItem,3,pMCtrlOffsetSE->GetSTimeStrDesp());
		m_List.SetItemText(nItem,4,pMCtrlOffsetSE->GetETimeStrDesp());
		str.Format("%d",((MCtrlWeek*)m_pMCtrlType)->m_ArrInIndex[i]);
		m_List.SetItemText(nItem,5,str);
		nItem++;
	}
	CString str;
	str.Format("%d",nItem);
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(str);
	UpdateData(false);
}

void CDlgMCtrlWeek::ShowCtrlData()
{
	InitTree();
	InitListBoxHead();
}
void CDlgMCtrlWeek::SetCtrlData()
{
	
}

void CDlgMCtrlWeek::OnSelchangedTreeWeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	this->pNMTreeView=pNMTreeView;

	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)//����Ǹ�Ŀ¼����ʾ����������
	{
		InitListBoxHead();
		InitListBoxBody(7);
		return;
	}
	CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//��ȡ�����ı�
	CString strWNum=strNodeText.Right(1);
	int wNum=::atoi(strWNum)-1;
	InitListBoxHead();
	InitListBoxBody(wNum);
	*pResult = 0;
}

void CDlgMCtrlWeek::OnBtnselweek() 
{
	// TODO: Add your control notification handler code here
	CDlgSelWeek dlg;
	CArray<int,int> arrWeekArr;
	((MCtrlWeek*)m_pMCtrlType)->GetWeekNums(arrWeekArr);
	dlg.SetWeekNum(arrWeekArr);
	if(IDOK==dlg.DoModal())
	{
		((MCtrlWeek*)m_pMCtrlType)->SetNewWeekNums(dlg.m_wNumArr);
		InitTree();
		InitListBoxHead();
	}
}

void CDlgMCtrlWeek::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	HTREEITEM hItem = m_ctlTree.GetSelectedItem();
	CString strNodeText=m_ctlTree.GetItemText(hItem);
	if(hItem==NULL)
	{
		AfxMessageBox("��ѡ�����ں��ٽ�����ӣ�");
		return;
	}
	CArray<int,int> arrWeekArr;
	((MCtrlWeek*)m_pMCtrlType)->GetWeekNums(arrWeekArr);
	if(arrWeekArr.GetSize()==0)
	{
		AfxMessageBox("��ǰûѡ�������޷�������ӣ�");
		return;
	}
	CString strTime="";
	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	CString *pStrTime=NULL;
	MCtrlOffsetSE *pMCtrlOffsetSE2=NULL;
	DlgSetTimeAndSETime dlg;
	dlg.SetTimeAndOffSetSETime(strTime,pMCtrlOffsetSE);
	if(IDOK==dlg.DoModal())
	{
		//TVITEM item = pNMTreeView->itemNew;
		if(strNodeText == "����")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
		{
			for(int i=0;i<arrWeekArr.GetSize();i++)//���и�����
			{
				((MCtrlWeek*)m_pMCtrlType)->GetTheWeekData(arrWeekArr[i],pStrTime,pMCtrlOffsetSE2);
				*pMCtrlOffsetSE2=dlg.m_MCtrlOffsetSE;
				*pStrTime=dlg.m_strTime;
			}
			InitListBoxHead();
			InitListBoxBody(7);
		}
		else
		{
			//CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//��ȡ�����ı�
			CString strWNum=strNodeText.Right(1);
			int wNum=::atoi(strWNum)-1;
			((MCtrlWeek*)m_pMCtrlType)->GetTheWeekData(wNum,pStrTime,pMCtrlOffsetSE2);
			*pMCtrlOffsetSE2=dlg.m_MCtrlOffsetSE;
			*pStrTime=dlg.m_strTime;
			InitListBoxHead();
			InitListBoxBody(wNum);
		}
	}
	delete pMCtrlOffsetSE;
	pMCtrlOffsetSE=NULL;
}

void CDlgMCtrlWeek::OnButtonEdit() 
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
	int index =this->m_List.GetNextSelectedItem(pos);
	int inIndex=atoi(this->m_List.GetItemText(index,5));
	((MCtrlWeek*)m_pMCtrlType)->GetTheRecordDataByInIndex(inIndex,pStrTime,pMCtrlOffsetSE);
	
	DlgSetTimeAndSETime dlg;
	dlg.SetTimeAndOffSetSETime(*pStrTime,pMCtrlOffsetSE);
	
	if(IDOK==dlg.DoModal())
	{
		*pMCtrlOffsetSE=dlg.m_MCtrlOffsetSE;
		*pStrTime=dlg.m_strTime;
		HTREEITEM hItem = m_ctlTree.GetSelectedItem();
		CString strNodeText=m_ctlTree.GetItemText(hItem);
		if(strNodeText == "����")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
		{
			InitListBoxHead();
			InitListBoxBody(7);
		}
		else
		{
			CString strWNum=strNodeText.Right(1);
			int wNum=::atoi(strWNum)-1;
			InitListBoxHead();
			InitListBoxBody(wNum);
		}
	}
}
void CDlgMCtrlWeek::OnButtonDel()
{
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
			int inIndex=atoi(this->m_List.GetItemText(index,5));
			selInIndexArr.Add(inIndex);
		}
		for(int i=selInIndexArr.GetSize()-1;i>=0;i--)
		{
			((MCtrlWeek*)m_pMCtrlType)->DelRecordByInIndex(selInIndexArr[i]);	
		}
		HTREEITEM hItem = m_ctlTree.GetSelectedItem();
		CString strNodeText=m_ctlTree.GetItemText(hItem);
		if(strNodeText == "����")//����Ǹ�Ŀ¼,���������е�ÿ����ӡ�
		{
			InitListBoxHead();
			InitListBoxBody(7);
		}
		else
		{
			CString strWNum=strNodeText.Right(1);
			int wNum=::atoi(strWNum)-1;
			InitListBoxHead();
			InitListBoxBody(wNum);
		}
	}
}