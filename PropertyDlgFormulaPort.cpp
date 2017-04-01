// PropertyDlgFormulaPort.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgFormulaPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormulaPort dialog


CPropertyDlgFormulaPort::CPropertyDlgFormulaPort(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgFormulaPort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgFormulaPort)
	currentSelPortIndex=-1;
	pPortNameArr=NULL;
	pPortTypeArr=NULL;
	pPortCanBeEdit=NULL;
	//}}AFX_DATA_INIT
}


void CPropertyDlgFormulaPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgFormulaPort)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgFormulaPort, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgFormulaPort)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormulaPort message handlers

BOOL CPropertyDlgFormulaPort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitPortListBoxHead();
	InitPortListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgFormulaPort::SetPortPropData(CStringArray *pPortNameArr,CArray<int,int> *pPortTypeArr)
{
	this->pPortNameArr=pPortNameArr;
	this->pPortTypeArr=pPortTypeArr;
}
void CPropertyDlgFormulaPort::SetDlgIsInOrOut(int tag)//1��IN��2��out��
{
	this->tagInOrOut=tag;
}
void CPropertyDlgFormulaPort::SetCanBeEdit(CArray<int,int> *pPortCanBeEdit)
{
	this->pPortCanBeEdit=pPortCanBeEdit;
}
void CPropertyDlgFormulaPort::InitPortListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[6]={"���","����","����"};
					//�б��ӵı�ͷ
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);
}
void CPropertyDlgFormulaPort::InitPortListBoxBody()
{
	CString str;
	int nItem=0;
	for(int i=0;i<(*pPortNameArr).GetSize();i++)
	{
		if((*pPortNameArr)[i]=="")
			break;
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		
		//m_List.SetEdit(nItem, 1);
		//m_List.SetEdit(nItem, 2);
		
		this->m_List.SetItemText(nItem,1,(*pPortNameArr)[i]);
		if((*pPortTypeArr)[i]==0)
			str="��ֵ";
		else
			str="����";
		this->m_List.SetItemText(nItem,2,str);
		nItem++;
	}
	for(i=0;i<this->m_List.GetItemCount();i++)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(i, 0, LVIS_FOCUSED|LVIS_SELECTED);
	}
	if(currentSelPortIndex!=-1)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(currentSelPortIndex, LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

bool CPropertyDlgFormulaPort::AddPort()
{
	int currentCount=getAvailPortCount();
	if(currentCount==(*pPortNameArr).GetSize())
	{
		AfxMessageBox("�Ѿ��ﵽ���˿ڸ���!");
		return false;
	}
	CDlgPortData dlgPortData;
	dlgPortData.SetType(0);//Ĭ������Ϊ��ֵ����
	if(dlgPortData.DoModal()==IDOK)
	{
		(*pPortNameArr)[currentCount]=dlgPortData.m_StrName;
		(*pPortTypeArr)[currentCount]=dlgPortData.GetType();
		InitPortListBoxHead();
		InitPortListBoxBody();
		return true;
	}
	else
		return false;
}
bool CPropertyDlgFormulaPort::EditPort()
{
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		index =this->m_List.GetNextSelectedItem(pos);
		if(pPortCanBeEdit!=NULL)
		{
			if((*pPortCanBeEdit)[index]==0)
			{
				AfxMessageBox("��ǰ�˿ڲ������༭!");
				return false;
			}
		}
		CDlgPortData dlgPortData;
		dlgPortData.m_StrName=(*pPortNameArr)[index];
		dlgPortData.SetType((*pPortTypeArr)[index]);
		if(dlgPortData.DoModal()==IDOK)
		{
			(*pPortNameArr)[index]=dlgPortData.m_StrName;
			(*pPortTypeArr)[index]=dlgPortData.GetType();
			InitPortListBoxHead();
			InitPortListBoxBody();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		AfxMessageBox("��ѡ��Ҫ�༭�Ķ˿�!");
		return false;
	}
}
bool CPropertyDlgFormulaPort::DelPort()
{
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		index =this->m_List.GetNextSelectedItem(pos);
		if(pPortCanBeEdit!=NULL)
		{
			if((*pPortCanBeEdit)[index]==0)
			{
				AfxMessageBox("��ǰ�˿ڲ�����ɾ��!");
				return false;
			}
		}
		CString strMsg;
		strMsg.Format("ȷ��Ҫɾ��ѡ�еĶ˿�?");
		int int_selOK=AfxMessageBox(strMsg,MB_OKCANCEL);
		if(int_selOK==1)
		{
			(*pPortNameArr).RemoveAt(index);
			(*pPortTypeArr).RemoveAt(index);
			(*pPortCanBeEdit).RemoveAt(index);
			
			(*pPortNameArr).Add("");
			(*pPortTypeArr).Add(0);
			(*pPortCanBeEdit).Add(1);
			InitPortListBoxHead();
			InitPortListBoxBody();
			return true;
		}
		else
			return false;
	}
	else
	{
		AfxMessageBox("��ѡ��Ҫɾ���Ķ˿�!");
		return false;
	}
}
int CPropertyDlgFormulaPort::getAvailPortCount()
{
	int result=(*pPortNameArr).GetSize();
	for(int i=0;i<(*pPortNameArr).GetSize();i++)
	{
		if((*pPortNameArr)[i]=="")
		{
			result=i;
			break;
		}
	}
	return result;
}
bool CPropertyDlgFormulaPort::MoveUpPort()//�����ƶ��˿�
{
	if(currentSelPortIndex!=-1)
	{
		if(currentSelPortIndex!=0)
		{
			CString strTemp=(*pPortNameArr)[currentSelPortIndex];
			(*pPortNameArr)[currentSelPortIndex]=(*pPortNameArr)[currentSelPortIndex-1];
			(*pPortNameArr)[currentSelPortIndex-1]=strTemp;
			
			int iTemp=(*pPortTypeArr)[currentSelPortIndex];
			(*pPortTypeArr)[currentSelPortIndex]=(*pPortTypeArr)[currentSelPortIndex-1];
			(*pPortTypeArr)[currentSelPortIndex-1]=iTemp;

			iTemp=(*pPortCanBeEdit)[currentSelPortIndex];
			(*pPortCanBeEdit)[currentSelPortIndex]=(*pPortCanBeEdit)[currentSelPortIndex-1];
			(*pPortCanBeEdit)[currentSelPortIndex-1]=iTemp;
			
			currentSelPortIndex--;
			InitPortListBoxHead();
			InitPortListBoxBody();
			return true;
		}
		else
		{
			AfxMessageBox("�ö˿��Ѵ������!");
			return false;
		}
	}
	else
	{
		AfxMessageBox("��ѡ��Ҫ�ƶ��Ķ˿�!");
		return false;
	}
}
bool CPropertyDlgFormulaPort::MoveDownPort()//�����ƶ��˿�
{
	if(currentSelPortIndex!=-1)
	{
		if(currentSelPortIndex!=m_List.GetItemCount()-1)
		{
			CString strTemp=(*pPortNameArr)[currentSelPortIndex];
			(*pPortNameArr)[currentSelPortIndex]=(*pPortNameArr)[currentSelPortIndex+1];
			(*pPortNameArr)[currentSelPortIndex+1]=strTemp;
			
			int iTemp=(*pPortTypeArr)[currentSelPortIndex];
			(*pPortTypeArr)[currentSelPortIndex]=(*pPortTypeArr)[currentSelPortIndex+1];
			(*pPortTypeArr)[currentSelPortIndex+1]=iTemp;

			iTemp=(*pPortCanBeEdit)[currentSelPortIndex];
			(*pPortCanBeEdit)[currentSelPortIndex]=(*pPortCanBeEdit)[currentSelPortIndex+1];
			(*pPortCanBeEdit)[currentSelPortIndex+1]=iTemp;
			
			currentSelPortIndex++;
			InitPortListBoxHead();
			InitPortListBoxBody();
			return true;
		}
		else
		{
			AfxMessageBox("�ù�ʽ�Ѵ�������!");
			return false;
		}
	}
	else
	{
		AfxMessageBox("��ѡ��Ҫ�ƶ��Ĺ�ʽ!");
		return false;
	}
}

void CPropertyDlgFormulaPort::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		index =this->m_List.GetNextSelectedItem(pos);
		currentSelPortIndex=index;
	}
	else
		currentSelPortIndex=-1;
	*pResult = 0;
}

void CPropertyDlgFormulaPort::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	EditPort();
	*pResult = 0;
}
