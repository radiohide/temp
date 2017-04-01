// PropertyDlgTabCoalOut.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTabCoalOut.h"
#include "DlgCoalFlowTagData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalOut dialog


CPropertyDlgTabCoalOut::CPropertyDlgTabCoalOut(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTabCoalOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTabCoalOut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgTabCoalOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTabCoalOut)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTabCoalOut, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTabCoalOut)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalOut message handlers
void CPropertyDlgTabCoalOut::initListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[4]={"序号","测点名称","描述","单位"};
					//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 150);	
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 60);
}
void CPropertyDlgTabCoalOut::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<(*m_pStrCoalTagName).GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,(*m_pStrCoalTagName)[i]);
		m_List.SetItemText(nItem,2,(*m_pStrCoalTagDesp)[i]);
		m_List.SetItemText(nItem,3,(*m_pStrCoalTagUnit)[i]);
		nItem++;
	}
}
BOOL CPropertyDlgTabCoalOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initListBoxHead();
	initListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgTabCoalOut::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgCoalFlowTagData dlg;
	dlg.m_strWinName="煤质变量";
	if(IDOK==dlg.DoModal())
	{
		int nItem=m_List.GetItemCount();
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);

		m_List.SetItemText(nItem,1,dlg.m_strTagName);
		m_List.SetItemText(nItem,2,dlg.m_strDesp);
		m_List.SetItemText(nItem,3,dlg.m_strUnit);

		(*m_pStrCoalTagName).Add(dlg.m_strTagName);
		(*m_pStrCoalTagDesp).Add(dlg.m_strDesp);
		(*m_pStrCoalTagUnit).Add(dlg.m_strUnit);
		nItem++;
	}
}

void CPropertyDlgTabCoalOut::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CArray<int,int> delIndexArr;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index=m_List.GetNextSelectedItem(pos);
		delIndexArr.Add(index);
	}
	for(int i=delIndexArr.GetSize()-1;i>=0;i--)
	{
		m_List.DeleteItem(delIndexArr[i]);
		(*m_pStrCoalTagName).RemoveAt(delIndexArr[i]);
		(*m_pStrCoalTagDesp).RemoveAt(delIndexArr[i]);
		(*m_pStrCoalTagUnit).RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgTabCoalOut::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos)
	{
		CDlgCoalFlowTagData dlg;
		dlg.m_strWinName="煤质变量";
		int index=m_List.GetNextSelectedItem(pos);
		dlg.m_strTagName=m_List.GetItemText(index,1);
		dlg.m_strDesp=m_List.GetItemText(index,2);
		dlg.m_strUnit=m_List.GetItemText(index,3);
		if(IDOK==dlg.DoModal())
		{
			m_List.SetItemText(index,1,dlg.m_strTagName);
			m_List.SetItemText(index,2,dlg.m_strDesp);
			m_List.SetItemText(index,3,dlg.m_strUnit);
			
			(*m_pStrCoalTagName)[index]=dlg.m_strTagName;
			(*m_pStrCoalTagDesp)[index]=dlg.m_strDesp;
			(*m_pStrCoalTagUnit)[index]=dlg.m_strUnit;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的测点标签!");
	}
}

void CPropertyDlgTabCoalOut::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}
