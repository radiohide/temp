// PropertyDlgTabCoalKTag.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTabCoalKTag.h"
#include "DlgCoalFlowTagData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalKTag dialog


CPropertyDlgTabCoalKTag::CPropertyDlgTabCoalKTag(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTabCoalKTag::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTabCoalKTag)
	m_pStrPowerTagName=NULL;
	m_pStrPowerUnit=NULL;

	m_pStrFlowTagNameArr=NULL;
	m_pStrFlowUnitArr=NULL;
	m_pStrFlowDespArr=NULL;
	//}}AFX_DATA_INIT
}


void CPropertyDlgTabCoalKTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTabCoalKTag)
	DDX_Control(pDX, IDC_LIST_FLOW, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTabCoalKTag, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTabCoalKTag)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FLOW, OnButtonAddFlow)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FLOW, OnButtonEditFlow)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FLOW, OnButtonDelFlow)
	ON_EN_CHANGE(IDC_EDIT_TAGNAME_POWER, OnChangeEditTagnamePower)
	ON_EN_CHANGE(IDC_EDIT_POWER_UNIT, OnChangeEditPowerUnit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FLOW, OnDblclkListFlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabCoalKTag message handlers

BOOL CPropertyDlgTabCoalKTag::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_TAGNAME_POWER)->SetWindowText(*m_pStrPowerTagName);
	GetDlgItem(IDC_EDIT_POWER_UNIT)->SetWindowText(*m_pStrPowerUnit);
	initListBoxHead();
	initListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgTabCoalKTag::initListBoxHead()
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
void CPropertyDlgTabCoalKTag::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<(*m_pStrFlowTagNameArr).GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,(*m_pStrFlowTagNameArr)[i]);
		m_List.SetItemText(nItem,2,(*m_pStrFlowDespArr)[i]);
		m_List.SetItemText(nItem,3,(*m_pStrFlowUnitArr)[i]);
		nItem++;
	}
}
void CPropertyDlgTabCoalKTag::OnButtonAddFlow() 
{
	// TODO: Add your control notification handler code here
	CDlgCoalFlowTagData dlg;
	if(IDOK==dlg.DoModal())
	{
		int nItem=m_List.GetItemCount();
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);

		m_List.SetItemText(nItem,1,dlg.m_strTagName);
		m_List.SetItemText(nItem,2,dlg.m_strDesp);
		m_List.SetItemText(nItem,3,dlg.m_strUnit);

		(*m_pStrFlowTagNameArr).Add(dlg.m_strTagName);
		(*m_pStrFlowDespArr).Add(dlg.m_strDesp);
		(*m_pStrFlowUnitArr).Add(dlg.m_strUnit);
		nItem++;
	}
}

void CPropertyDlgTabCoalKTag::OnButtonEditFlow() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos)
	{
		CDlgCoalFlowTagData dlg;
		int index=m_List.GetNextSelectedItem(pos);
		dlg.m_strTagName=m_List.GetItemText(index,1);
		dlg.m_strDesp=m_List.GetItemText(index,2);
		dlg.m_strUnit=m_List.GetItemText(index,3);
		if(IDOK==dlg.DoModal())
		{
			m_List.SetItemText(index,1,dlg.m_strTagName);
			m_List.SetItemText(index,2,dlg.m_strDesp);
			m_List.SetItemText(index,3,dlg.m_strUnit);
			
			(*m_pStrFlowTagNameArr)[index]=dlg.m_strTagName;
			(*m_pStrFlowDespArr)[index]=dlg.m_strDesp;
			(*m_pStrFlowUnitArr)[index]=dlg.m_strUnit;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的测点标签!");
	}
}

void CPropertyDlgTabCoalKTag::OnButtonDelFlow() 
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
		(*m_pStrFlowTagNameArr).RemoveAt(delIndexArr[i]);
		(*m_pStrFlowDespArr).RemoveAt(delIndexArr[i]);
		(*m_pStrFlowUnitArr).RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgTabCoalKTag::OnChangeEditTagnamePower() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_TAGNAME_POWER)->GetWindowText(*m_pStrPowerTagName);
	// TODO: Add your control notification handler code here
	
}

void CPropertyDlgTabCoalKTag::OnChangeEditPowerUnit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_POWER_UNIT)->GetWindowText(*m_pStrPowerUnit);
	// TODO: Add your control notification handler code here
	
}

void CPropertyDlgTabCoalKTag::OnDblclkListFlow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEditFlow();
	*pResult = 0;
}
