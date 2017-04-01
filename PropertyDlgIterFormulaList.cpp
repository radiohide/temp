// PropertyDlgIterFormulaList.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgIterFormulaList.h"
#include "DlgFormulaData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIterFormulaList dialog


CPropertyDlgIterFormulaList::CPropertyDlgIterFormulaList(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgIterFormulaList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgIterFormulaList)
	m_iConFOrB = -1;
	m_pConFOrB=NULL;
	//}}AFX_DATA_INIT
}


void CPropertyDlgIterFormulaList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgIterFormulaList)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Radio(pDX, IDC_RADIO1, m_iConFOrB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgIterFormulaList, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgIterFormulaList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIterFormulaList message handlers
void CPropertyDlgIterFormulaList::SetFormulaData(CStringArray *pVNameArr,CArray<int,int> *pTypeArr,CStringArray *pFormulaArr)
{
	this->pVNameArr=pVNameArr;
	this->pTypeArr=pTypeArr;
	this->pFormulaArr=pFormulaArr;
}
void CPropertyDlgIterFormulaList::SetCanBeEditCanBeShow(CArray<int,int> *pCanBeEditArr,CArray<int,int> *pCanBeShowArr)
{
	this->pCanBeEditArr=pCanBeEditArr;
	this->pCanBeShowArr=pCanBeShowArr;
}
BOOL CPropertyDlgIterFormulaList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitFormulaListBoxHead();
	InitFormulaListBoxBody();
	if(m_pConFOrB!=NULL)
	{
		if(*(this->m_pConFOrB)==0)
		{
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgIterFormulaList::InitFormulaListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","变量","类型","公式"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);	
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,140);
}
void CPropertyDlgIterFormulaList::InitFormulaListBoxBody()
{
	int nItem=0;
	for(int i=0;i<(*pVNameArr).GetSize();i++)
	{
		if((*pVNameArr)[i]=="")
			break;
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		
		this->m_List.SetItemText(nItem,1,(*pVNameArr)[i]);
		if((*pTypeArr)[i]==0)
			str="数值";
		else
			str="序列";
		this->m_List.SetItemText(nItem,2,str);

		if((*pCanBeShowArr)[i]==1)
		{
			this->m_List.SetItemText(nItem,3,(*pFormulaArr)[i]);
		}
		else
			this->m_List.SetItemText(nItem,3,"HIDE");
		nItem++;
	}
	for(i=0;i<this->m_List.GetItemCount();i++)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(i, 0, LVIS_FOCUSED|LVIS_SELECTED);
	}
	if((*m_pCurrSelFormulaIndex)!=-1)
	{
		m_List.SetFocus(); 
		m_List.SetItemState((*m_pCurrSelFormulaIndex), LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CPropertyDlgIterFormulaList::SetCurrentSelIndex(int *m_pCurrSelFormulaIndex)
{
	this->m_pCurrSelFormulaIndex=m_pCurrSelFormulaIndex;
}
int CPropertyDlgIterFormulaList::getAvailFormulaCount()
{
	int result=(*pVNameArr).GetSize();
	for(int i=0;i<(*pVNameArr).GetSize();i++)
	{
		if((*pVNameArr)[i]=="")
		{
			result=i;
			break;
		}
	}
	return result;
}
bool CPropertyDlgIterFormulaList::AddVar()
{
	bool result=false;
	int currentCount=getAvailFormulaCount();
	if(currentCount==(*pVNameArr).GetSize())
	{
		AfxMessageBox("已经达到最多公式个数!");
	}
	CDlgFormulaData dlgFormulaData;
	dlgFormulaData.SetType(0);//默认增加数值类型
	if(dlgFormulaData.DoModal()==IDOK)
	{
		(*pVNameArr)[currentCount]=dlgFormulaData.m_EditVarName;
		(*pFormulaArr)[currentCount]=dlgFormulaData.m_EditFormula;
		(*pTypeArr)[currentCount]=dlgFormulaData.GetType();
		InitFormulaListBoxHead();
		InitFormulaListBoxBody();
		result= true;
	}
	return result;
}

bool CPropertyDlgIterFormulaList::EditVar()
{
	bool result=false;
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		index =this->m_List.GetNextSelectedItem(pos);
		if((*pCanBeEditArr)[index]==0)
		{
			AfxMessageBox("当前变量不允许被编辑!");
			return false;
		}
		CDlgFormulaData dlgFormulaData;
		dlgFormulaData.m_EditVarName=(*pVNameArr)[index];
		dlgFormulaData.m_EditFormula=(*pFormulaArr)[index];
		dlgFormulaData.SetType((*pTypeArr)[index]);

		if(dlgFormulaData.DoModal()==IDOK)
		{
			(*pVNameArr)[index]=dlgFormulaData.m_EditVarName;
			(*pFormulaArr)[index]=dlgFormulaData.m_EditFormula;
			(*pTypeArr)[index]=dlgFormulaData.GetType();
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
			result= true;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的公式记录!");
	}
	return result;
}

bool CPropertyDlgIterFormulaList::DelVar()
{
	bool result=false;
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		index =this->m_List.GetNextSelectedItem(pos);
		if((*pCanBeEditArr)[index]==0)
		{
			AfxMessageBox("当前变量不允许被编辑!");
			return false;
		}
		CString strMsg;
		strMsg.Format("确定要删除选中的公式?");
		int int_selOK=AfxMessageBox(strMsg,MB_OKCANCEL);
		if(int_selOK==1)
		{
			(*pVNameArr).RemoveAt(index);
			(*pFormulaArr).RemoveAt(index);
			(*pTypeArr).RemoveAt(index);
			(*pCanBeEditArr).RemoveAt(index);
			(*pCanBeShowArr).RemoveAt(index);

			(*pVNameArr).Add("");
			(*pFormulaArr).Add(0);
			(*pTypeArr).Add(0);
			(*pCanBeEditArr).Add(1);
			(*pCanBeEditArr).Add(1);
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
			result= true;
		}
	}
	else
	{
		AfxMessageBox("请选择要删除的端口!");
	}
	return result;
}

bool CPropertyDlgIterFormulaList::MoveUpVar()
{
	bool result=false;
	if(*m_pCurrSelFormulaIndex!=-1)
	{
		if(*m_pCurrSelFormulaIndex!=0)
		{
			CString strTemp=(*pVNameArr)[*m_pCurrSelFormulaIndex];
			(*pVNameArr)[*m_pCurrSelFormulaIndex]=(*pVNameArr)[*m_pCurrSelFormulaIndex-1];
			(*pVNameArr)[*m_pCurrSelFormulaIndex-1]=strTemp;
			
			strTemp=(*pFormulaArr)[*m_pCurrSelFormulaIndex];
			(*pFormulaArr)[*m_pCurrSelFormulaIndex]=(*pFormulaArr)[*m_pCurrSelFormulaIndex-1];
			(*pFormulaArr)[*m_pCurrSelFormulaIndex-1]=strTemp;

			int iTemp=(*pTypeArr)[*m_pCurrSelFormulaIndex];
			(*pTypeArr)[*m_pCurrSelFormulaIndex]=(*pTypeArr)[*m_pCurrSelFormulaIndex-1];
			(*pTypeArr)[*m_pCurrSelFormulaIndex-1]=iTemp;

			iTemp=(*pCanBeEditArr)[*m_pCurrSelFormulaIndex];
			(*pCanBeEditArr)[*m_pCurrSelFormulaIndex]=(*pCanBeEditArr)[*m_pCurrSelFormulaIndex-1];
			(*pCanBeEditArr)[*m_pCurrSelFormulaIndex-1]=iTemp;

			iTemp=(*pCanBeShowArr)[*m_pCurrSelFormulaIndex];
			(*pCanBeShowArr)[*m_pCurrSelFormulaIndex]=(*pCanBeShowArr)[*m_pCurrSelFormulaIndex-1];
			(*pCanBeShowArr)[*m_pCurrSelFormulaIndex-1]=iTemp;
			
			*m_pCurrSelFormulaIndex--;
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
			result=true;
		}
		else
			AfxMessageBox("该公式已处于最表顶!");
	}
	else
	{
		AfxMessageBox("请选择要移动的公式!");
	}
	return result;
}

bool CPropertyDlgIterFormulaList::MoveDownVar()
{
	bool result=false;
	if(*m_pCurrSelFormulaIndex!=-1)
	{
		if(*m_pCurrSelFormulaIndex!=m_List.GetItemCount()-1)
		{
			CString strTemp=(*pVNameArr)[*m_pCurrSelFormulaIndex];
			(*pVNameArr)[*m_pCurrSelFormulaIndex]=(*pVNameArr)[*m_pCurrSelFormulaIndex+1];
			(*pVNameArr)[*m_pCurrSelFormulaIndex+1]=strTemp;
			
			strTemp=(*pFormulaArr)[*m_pCurrSelFormulaIndex];
			(*pFormulaArr)[*m_pCurrSelFormulaIndex]=(*pFormulaArr)[*m_pCurrSelFormulaIndex+1];
			(*pFormulaArr)[*m_pCurrSelFormulaIndex+1]=strTemp;

			int iTemp=(*pTypeArr)[*m_pCurrSelFormulaIndex];
			(*pTypeArr)[*m_pCurrSelFormulaIndex]=(*pTypeArr)[*m_pCurrSelFormulaIndex+1];
			(*pTypeArr)[*m_pCurrSelFormulaIndex+1]=iTemp;
			
			iTemp=(*pCanBeEditArr)[*m_pCurrSelFormulaIndex];
			(*pCanBeEditArr)[*m_pCurrSelFormulaIndex]=(*pCanBeEditArr)[*m_pCurrSelFormulaIndex+1];
			(*pCanBeEditArr)[*m_pCurrSelFormulaIndex+1]=iTemp;
			
			iTemp=(*pCanBeShowArr)[*m_pCurrSelFormulaIndex];
			(*pCanBeShowArr)[*m_pCurrSelFormulaIndex]=(*pCanBeShowArr)[*m_pCurrSelFormulaIndex+1];
			(*pCanBeShowArr)[*m_pCurrSelFormulaIndex+1]=iTemp;
			
			*m_pCurrSelFormulaIndex++;
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
			result=true;
		}
		else
			AfxMessageBox("该公式已处于最表低!");
	}
	else
	{
		AfxMessageBox("请选择要移动的公式!");
	}
	return result;
}

void CPropertyDlgIterFormulaList::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		index =this->m_List.GetNextSelectedItem(pos);
		*m_pCurrSelFormulaIndex=index;
		this->GetParent()->GetParent()->PostMessage(WM_CLICK_ITER_FORMULALIST);
	}
	else
		*m_pCurrSelFormulaIndex=-1;
	*pResult = 0;
}

void CPropertyDlgIterFormulaList::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	EditVar();
	*pResult = 0;
}

void CPropertyDlgIterFormulaList::HideConFOrBCtrl()//隐藏条件次序相关控件
{
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
}

void CPropertyDlgIterFormulaList::ShowConFOrBCtrl(CRect rc)
{
	GetDlgItem(IDC_STATIC1)->MoveWindow(rc.left,rc.top-15,rc.Width(),rc.Height()-110);
	m_List.MoveWindow(rc.left,rc.top+30,rc.Width(),rc.Height()-45);
}

void CPropertyDlgIterFormulaList::SetPtrTagConFOrB(int *ptrTagConFOrB)
{
	m_pConFOrB=ptrTagConFOrB;
}


void CPropertyDlgIterFormulaList::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()==0)
	{
		*m_pConFOrB=1;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()==1)
	{
		*m_pConFOrB=0;
	}
}

void CPropertyDlgIterFormulaList::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck()==0)
	{
		*m_pConFOrB=0;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck()==1)
	{
		*m_pConFOrB=1;
	}
}
