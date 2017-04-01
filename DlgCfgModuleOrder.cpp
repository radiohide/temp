// DlgCfgModuleOrder.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "DlgCfgModuleOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgModuleOrder dialog


CDlgCfgModuleOrder::CDlgCfgModuleOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgModuleOrder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgModuleOrder)
	currentSelIndex=-1;
	//}}AFX_DATA_INIT
}


void CDlgCfgModuleOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgModuleOrder)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgModuleOrder, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgModuleOrder)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnButtonMoveDown)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgModuleOrder message handlers

BOOL CDlgCfgModuleOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitListBoxHead();
	InitListBoxBody();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgCfgModuleOrder::SetModuleList(CModuleList &moduleList)
{	
	m_ArrModule.RemoveAll();
	POSITION pos = moduleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)moduleList.GetNext(pos);
		m_ArrModule.Add(pModule);
	}
}
void CDlgCfgModuleOrder::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[3]={"序号","模块名称","模块描述"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);	
}
void CDlgCfgModuleOrder::InitListBoxBody()
{
	int nItem=0;
	for(int i=0;i<this->m_ArrModule.GetSize();i++)
	{
		CString strIndex;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		this->m_List.SetItemText(nItem,1,m_ArrModule[i]->m_Name);
		this->m_List.SetItemText(nItem,2,m_ArrModule[i]->m_Desp);
		nItem++;
	}
	for(i=0;i<this->m_List.GetItemCount();i++)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(i, 0, LVIS_FOCUSED|LVIS_SELECTED);
	}
	for(i=0;i<this->m_List.GetItemCount();i++)
	{
		if(i==currentSelIndex)
		{
			m_List.SetFocus(); 
			m_List.SetItemState(currentSelIndex, LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
		}
	}
}

void CDlgCfgModuleOrder::OnOK() 
{
	// TODO: Add extra validation here
	for(int i=0;i<this->m_ArrModule.GetSize();i++)
	{
		CCalcModule* pModule=m_ArrModule[i];
		pModule->SetCalcOrderID(i);
	}
	CDialog::OnOK();
}

void CDlgCfgModuleOrder::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgCfgModuleOrder::OnButtonMoveUp() 
{
	// TODO: Add your control notification handler code here
	if(currentSelIndex!=-1)
	{
		if(currentSelIndex!=0)
		{
			CCalcModule* pModuleTemp=m_ArrModule[currentSelIndex];
			m_ArrModule[currentSelIndex]=m_ArrModule[currentSelIndex-1];
			m_ArrModule[currentSelIndex-1]=pModuleTemp;
			
			currentSelIndex--;
			InitListBoxHead();
			InitListBoxBody();
		}
		else
			AfxMessageBox("该模块已处于顶端!");
	}
	else
	{
		AfxMessageBox("请选择要移动的模块!");
	}
}

void CDlgCfgModuleOrder::OnButtonMoveDown() 
{
	// TODO: Add your control notification handler code here
	if(currentSelIndex!=-1)
	{
		if(currentSelIndex!=m_List.GetItemCount()-1)
		{
			CCalcModule* pModuleTemp=m_ArrModule[currentSelIndex];
			m_ArrModule[currentSelIndex]=m_ArrModule[currentSelIndex+1];
			m_ArrModule[currentSelIndex+1]=pModuleTemp;

			currentSelIndex++;
			InitListBoxHead();
			InitListBoxBody();
		}
		else
			AfxMessageBox("该模块已处于末端!");
	}
	else
	{
		AfxMessageBox("请选择要移动的模块!");
	}
}

void CDlgCfgModuleOrder::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		index =this->m_List.GetNextSelectedItem(pos);
		currentSelIndex=index;
	}
	else
		currentSelIndex=-1;
	*pResult = 0;
}
