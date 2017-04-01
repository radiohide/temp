// PropertyDlgTabRuleVar.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgTabRuleVar.h"
#include "DlgSysVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabRuleVar dialog


CPropertyDlgTabRuleVar::CPropertyDlgTabRuleVar(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgTabRuleVar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgTabRuleVar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgTabRuleVar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgTabRuleVar)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgTabRuleVar, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgTabRuleVar)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_VARSEL, OnButtonVarSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTabRuleVar message handlers

BOOL CPropertyDlgTabRuleVar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initPortListBoxHead();
	initPortListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgTabRuleVar::SetRDBInf(CCalcModel *pModel,CString *pStrDBName,int *piUnit,int *piSet)
{
	m_pModel=pModel;
	m_pStrDBName=pStrDBName;
	m_piUnit=piUnit;
	m_piSet=piSet;
}
void CPropertyDlgTabRuleVar::SetVarData(CArray<int,int> *m_piVarCGArr,
		CStringArray *m_pStrVarNameArr,
		CStringArray *m_pStrVarDespArr,
		CStringArray *m_pStrVarUnit
		)
{
	this->m_piVarCGArr=m_piVarCGArr;
	this->m_pStrVarNameArr=m_pStrVarNameArr;
	this->m_pStrVarDespArr=m_pStrVarDespArr;
	this->m_pStrVarUnit=m_pStrVarUnit;
}
void CPropertyDlgTabRuleVar::initPortListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","复合组ID","变量名","描述","单位"};//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,150);
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,100);
	m_List.InsertColumn(4,Field[4],LVCFMT_LEFT,40);
}
void CPropertyDlgTabRuleVar::initPortListBoxBody()
{
	CString str;
	int nItem=0;
	for(int i=0;i<(*m_pStrVarNameArr).GetSize();i++)
	{
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		str.Format("%d",(*m_piVarCGArr)[i]);
		this->m_List.SetItemText(nItem,1,str);
		this->m_List.SetItemText(nItem,2,(*m_pStrVarNameArr)[i]);
		this->m_List.SetItemText(nItem,3,(*m_pStrVarDespArr)[i]);
		this->m_List.SetItemText(nItem,4,(*m_pStrVarUnit)[i]);
		nItem++;
	}
}

void CPropertyDlgTabRuleVar::OnButtonDel() 
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
		(*m_piVarCGArr).RemoveAt(delIndexArr[i]);
		(*m_pStrVarNameArr).RemoveAt(delIndexArr[i]);
		(*m_pStrVarDespArr).RemoveAt(delIndexArr[i]);
		(*m_pStrVarUnit).RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgTabRuleVar::OnButtonVarSel() 
{
	// TODO: Add your control notification handler code here
	CDlgSysVar dlg;
	dlg.SetRDBInf(m_pModel,*m_pStrDBName,*m_piUnit,*m_piSet);
	if(*m_pStrDBName=="")
	{
		AfxMessageBox("请选择有效数据源！");
		return;
	}
	if((*m_piUnit==-1)||(*m_piSet==-1))
	{
		AfxMessageBox("请选择机组！");
		return;
	}
	if(IDOK==dlg.DoModal())
	{
		for(int i=0;i<dlg.m_strVarNameArr.GetSize();i++)
		{
			if(beExistInSel(dlg.m_strVarNameArr[i]))
				continue;
			int nItem=m_List.GetItemCount();
			CString str;
			str.Format("%d",nItem+1);
			m_List.InsertItem(nItem,str);
			str.Format("%d",dlg.m_iCGArr[i]);
			m_List.SetItemText(nItem,1,str);
			m_List.SetItemText(nItem,2,dlg.m_strVarNameArr[i]);
			m_List.SetItemText(nItem,3,dlg.m_strVarDespArr[i]);
			m_List.SetItemText(nItem,4,dlg.m_strVarUnitArr[i]);
			
			(*m_piVarCGArr).Add(dlg.m_iCGArr[i]);
			(*m_pStrVarNameArr).Add(dlg.m_strVarNameArr[i]);
			(*m_pStrVarDespArr).Add(dlg.m_strVarDespArr[i]);
			(*m_pStrVarUnit).Add(dlg.m_strVarUnitArr[i]);
		}
	}
}

BOOL CPropertyDlgTabRuleVar::beExistInSel(CString strVarName)
{
	BOOL ret=FALSE;
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemText(i,2)==strVarName)
		{
			ret=TRUE;
			break;
		}
	}
	return ret;
}