// DlgSysVar.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgSysVar.h"
#include "SysVarAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVar dialog

CDlgSysVar::CDlgSysVar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysVar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysVar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSysVar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysVar)
	DDX_Control(pDX, IDC_LIST_SELVAR, m_List2);
	DDX_Control(pDX, IDC_LIST_VAR, m_List);
	DDX_Control(pDX, IDC_TREE_GROUP, m_ctlTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysVar, CDialog)
	//{{AFX_MSG_MAP(CDlgSysVar)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_GROUP, OnSelchangedTreeGroup)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VAR, OnDblclkListVar)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELVAR, OnDblclkListSelvar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVar message handlers
void CDlgSysVar::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
BOOL CDlgSysVar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SysVarAll=new SysVarAll;
	m_SysVarAll->SetRDBInf(m_pModel,m_strDBName,m_iUnit,m_iSet);
	m_SysVarAll->InitSysVarAll();
	InitListBoxHead();
	InitListBox2Head();
	InitTree();
	//InitListBoxHead();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgSysVar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDlgSysVar::InitTree()
{
//	m_ctlTree.DeleteAllItems();
	DWORD dwStyle = GetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_ctlTree.m_hWnd,GWL_STYLE,dwStyle);//设置树形控件的风格
    m_hRoot = m_ctlTree.InsertItem("变量分组");//根节点

	//一级子节点+二级子节点
	m_ctlTree.InsertItem("所有子组",m_hRoot);
	HTREEITEM hChild = m_ctlTree.GetChildItem(m_hRoot);
	for(int i=0;i<m_SysVarAll->SysVarGArr.GetSize();i++)//所有具体子组
	{
		SysVarG *pSysVarG=m_SysVarAll->SysVarGArr[i];
		CString str;
		str.Format("G%2s%s",pSysVarG->ID,pSysVarG->Name);
		m_ctlTree.InsertItem(str,hChild);
	}

	for(i=0;i<m_SysVarAll->SysVarCGArr.GetSize();i++)//所有复合组
	{
		SysVarCG *pSysVarCG=m_SysVarAll->SysVarCGArr[i];
		CString str;
		str.Format("CG%2s%s",pSysVarCG->ID,pSysVarCG->Name);
		m_ctlTree.InsertItem(str,m_hRoot);
		hChild = m_ctlTree.GetNextItem(hChild,TVGN_NEXT);
		for(int j=0;j<pSysVarCG->SysVarSelfGArr.GetSize();j++)
		{
			SysVarG *pSysVarG=pSysVarCG->SysVarSelfGArr[j];
			str.Format("G%2s%s",pSysVarG->ID,pSysVarG->Name);
			m_ctlTree.InsertItem(str,hChild);
		}
	}
	m_ctlTree.Expand(m_hRoot,TVE_EXPAND);//只展开一级子节点
	m_ctlTree.SetBkColor(RGB(255,255,255));//设置树形控件的背景色
	m_ctlTree.SetTextColor(RGB(0,0,0));//设置文本颜色
}

void CDlgSysVar::OnSelchangedTreeGroup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)//如果是根目录，返回
		return;
	CString strNodeText=m_ctlTree.GetItemText(pNMTreeView->itemNew.hItem);//获取活动结点文本

	int type=-1;
	SysVarCG *pRSysVarCG=NULL;
	SysVarG *pRSysVarG=NULL;

	m_SysVarAll->GetSelObj(strNodeText,type,pRSysVarCG,pRSysVarG);
	if(type==0)
	{
		InitListBoxHead();
		InitListBoxBodyByAllVar();
	}
	else if(type==1)
	{
		InitListBoxHead();
		InitListBoxBodyByCG(pRSysVarCG);
	}
	else if(type==2)
	{
		InitListBoxHead();
		InitListBoxBodyByG(pRSysVarG);
	}

	*pResult = 0;
}
void CDlgSysVar::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[11]={"序号","组名","变量名","描述","单位","D/A"};
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,36);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,36);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,120);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,36);
	m_List.InsertColumn(5,Field[5],LVCFMT_LEFT,30);
}
void CDlgSysVar::InitListBox2Head()
{
	m_List2.DeleteAllItems();
	while(m_List2.DeleteColumn(0));

	DWORD liststyle=m_List2.GetExtendedStyle();//获取列表控件的扩展风格
	m_List2.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[11]={"序号","组名","变量名","描述","单位","D/A"};
	m_List2.InsertColumn(0,Field[0],LVCFMT_LEFT,36);	
	m_List2.InsertColumn(1,Field[1],LVCFMT_LEFT,36);	
	m_List2.InsertColumn(2,Field[2],LVCFMT_LEFT,120);
	m_List2.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
	m_List2.InsertColumn(4,Field[4],LVCFMT_LEFT,36);
	m_List2.InsertColumn(5,Field[5],LVCFMT_LEFT,30);
}
void CDlgSysVar::InitListBoxBodyByAllVar()
{
	int nItem=0;
	for(int i=0;i<m_SysVarAll->SysVarArr.GetSize();i++)
	{
		SysVar *pSysVar=m_SysVarAll->SysVarArr[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		
		this->m_List.SetItemText(nItem,1,"");
		this->m_List.SetItemText(nItem,2,pSysVar->Name);
		this->m_List.SetItemText(nItem,3,pSysVar->Desp);
		this->m_List.SetItemText(nItem,4,pSysVar->unit);
		str.Format("%d",pSysVar->type);
		this->m_List.SetItemText(nItem,5,str);
		nItem++;
	}
}
void CDlgSysVar::InitListBoxBodyByCG(SysVarCG *pRSysVarCG)
{
	SysVarG *pRSysVarG;
	for(int i=0;i<pRSysVarCG->SysVarSelfGArr.GetSize();i++)
	{
		pRSysVarG=pRSysVarCG->SysVarSelfGArr[i];
		InitListBoxBodyByG(pRSysVarG);
	}
}
void CDlgSysVar::InitListBoxBodyByG(SysVarG *pRSysVarG)
{
	int nItem=m_List.GetItemCount();
	SysVar *pSysVar;
	for(int i=0;i<pRSysVarG->SysSelfVarArr.GetSize();i++)
	{
		pSysVar=pRSysVarG->SysSelfVarArr[i];
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		this->m_List.SetItemText(nItem,1,pRSysVarG->ID);
		this->m_List.SetItemText(nItem,2,pSysVar->Name);
		this->m_List.SetItemText(nItem,3,pSysVar->Desp);
		this->m_List.SetItemText(nItem,4,pSysVar->unit);
		str.Format("%d",pSysVar->type);
		this->m_List.SetItemText(nItem,5,str);
		nItem++;
	}
}

void CDlgSysVar::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index=m_List.GetNextSelectedItem(pos);
		CString strCGID=m_List.GetItemText(index,1);
		CString strVarName=m_List.GetItemText(index,2);
		CString strVarDesp=m_List.GetItemText(index,3);
		CString strVarUnit=m_List.GetItemText(index,4);
		CString strDOrA=m_List.GetItemText(index,5);
		
		if(beExistInSel(strVarName))
		{
			CString str;
			str.Format("测点%s已经在最新选择中!",strVarName);
			AfxMessageBox(str);
			continue;
		}
		int nItem=m_List2.GetItemCount();
		CString str;
		str.Format("%d",nItem+1);
		m_List2.InsertItem(nItem,str);
		m_List2.SetItemText(nItem,1,strCGID);
		m_List2.SetItemText(nItem,2,strVarName);
		m_List2.SetItemText(nItem,3,strVarDesp);
		m_List2.SetItemText(nItem,4,strVarUnit);
		m_List2.SetItemText(nItem,5,strDOrA);	
	}
}
BOOL CDlgSysVar::beExistInSel(CString strVarName)
{
	BOOL ret=FALSE;
	for(int i=0;i<m_List2.GetItemCount();i++)
	{
		if(m_List2.GetItemText(i,2)==strVarName)
		{
			ret=TRUE;
			break;
		}
	}
	return ret;
}
void CDlgSysVar::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CArray<int,int> delIndexArr;
	POSITION pos=m_List2.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index=m_List2.GetNextSelectedItem(pos);
		delIndexArr.Add(index);
	}
	for(int i=delIndexArr.GetSize()-1;i>=0;i--)
	{
		m_List2.DeleteItem(delIndexArr[i]);
	}
}

void CDlgSysVar::OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonAdd();
	*pResult = 0;
}

void CDlgSysVar::OnDblclkListSelvar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonDel();
	*pResult = 0;
}

void CDlgSysVar::OnOK() 
{
	// TODO: Add extra validation here
	m_iCGArr.RemoveAll();
	m_strVarNameArr.RemoveAll();
	m_strVarDespArr.RemoveAll();
	m_strVarUnitArr.RemoveAll();
	for(int i=0;i<m_List2.GetItemCount();i++)
	{
		CString strCGID=m_List2.GetItemText(i,1);
		CString strVarName=m_List2.GetItemText(i,2);
		CString strVarDesp=m_List2.GetItemText(i,3);
		CString strVarUnit=m_List2.GetItemText(i,4);
		
		m_iCGArr.Add(atoi(strCGID));
		m_strVarNameArr.Add(strVarName);
		m_strVarDespArr.Add(strVarDesp);
		m_strVarUnitArr.Add(strVarUnit);
	}
	CDialog::OnOK();
}
