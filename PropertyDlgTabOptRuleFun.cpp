// PropertyDlgTabOptRuleFun.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTabOptRuleFun.h"
#include "DlgRuleOptFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabOptRuleFun dialog


CPropertyDlgTabOptRuleFun::CPropertyDlgTabOptRuleFun(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTabOptRuleFun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTabOptRuleFun)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgTabOptRuleFun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTabOptRuleFun)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTabOptRuleFun, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTabOptRuleFun)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabOptRuleFun message handlers

BOOL CPropertyDlgTabOptRuleFun::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initPortListBoxHead();
	FillListBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgTabOptRuleFun::initPortListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","目标复合组ID","表达式","描述"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,100);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,150);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
}
void CPropertyDlgTabOptRuleFun::SetRuleData(CArray<int,int> *m_piCGArr,
											CStringArray *m_pStrFunExpArr,
											CStringArray *m_pStrFunDepArr)
{
	this->m_piCGArr=m_piCGArr;
	this->m_pStrFunExpArr=m_pStrFunExpArr;
	this->m_pStrFunDepArr=m_pStrFunDepArr;	
}
void CPropertyDlgTabOptRuleFun::FillListBox()
{
	CString str;
	int nItem=0;
	for(int i=0;i<(*m_piCGArr).GetSize();i++)
	{
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		
		str.Format("%d",(*m_piCGArr)[i]);
		this->m_List.SetItemText(nItem,1,str);
		
		this->m_List.SetItemText(nItem,2,(*m_pStrFunExpArr)[i]);
		this->m_List.SetItemText(nItem,3,(*m_pStrFunDepArr)[i]);
		nItem++;
	}
}


void CPropertyDlgTabOptRuleFun::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgRuleOptFun dlg;
	if(IDOK==dlg.DoModal())
	{
		int nItem=m_List.GetItemCount();
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);

		str.Format("%d",dlg.m_iCGID);
		m_List.SetItemText(nItem,1,str);
		m_List.SetItemText(nItem,2,dlg.m_strFormula);
		m_List.SetItemText(nItem,3,dlg.m_strDesp);

		(*m_piCGArr).Add(dlg.m_iCGID);
		(*m_pStrFunExpArr).Add(dlg.m_strFormula);
		(*m_pStrFunDepArr).Add(dlg.m_strDesp);
	}
}

void CPropertyDlgTabOptRuleFun::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos)
	{
		CDlgRuleOptFun dlg;
		int index=m_List.GetNextSelectedItem(pos);
		dlg.m_iCGID=atoi(m_List.GetItemText(index,1));
		dlg.m_strFormula=m_List.GetItemText(index,2);
		dlg.m_strDesp=m_List.GetItemText(index,3);
		if(IDOK==dlg.DoModal())
		{
			CString str;
			str.Format("%d",dlg.m_iCGID);
			m_List.SetItemText(index,1,str);
			m_List.SetItemText(index,2,dlg.m_strFormula);
			m_List.SetItemText(index,3,dlg.m_strDesp);
			
			(*m_piCGArr)[index]=dlg.m_iCGID;
			(*m_pStrFunExpArr)[index]=dlg.m_strFormula;
			(*m_pStrFunDepArr)[index]=dlg.m_strDesp;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的表达式再进行操作!");
	}
}

void CPropertyDlgTabOptRuleFun::OnButtonDel() 
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
		(*m_piCGArr).RemoveAt(delIndexArr[i]);
		(*m_pStrFunExpArr).RemoveAt(delIndexArr[i]);
		(*m_pStrFunDepArr).RemoveAt(delIndexArr[i]);
	}
}
