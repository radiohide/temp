// PropertyDlgCoalData.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgCoalData.h"
#include "DlgCoalFlowTagData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalData dialog


CPropertyDlgCoalData::CPropertyDlgCoalData(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgCoalData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgCoalData)
	m_strTableName = _T("");
	m_TagTimeOffsetOrInput = -1;
	m_lOffset = 0;
	m_iDays =0;
	m_iNearTimeOrValue = -1;
	m_dThreshold = 0.0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgCoalData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgCoalData)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	DDX_Radio(pDX, IDC_RADIO5, m_TagTimeOffsetOrInput);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Text(pDX, IDC_EDIT_DAY, m_iDays);
	DDX_Radio(pDX, IDC_RADIO1, m_iNearTimeOrValue);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_dThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgCoalData, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgCoalData)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalData message handlers
void CPropertyDlgCoalData::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgCoalData::SetCoalTagData(CStringArray &m_strCoalTagName,
		CStringArray &m_strCoalTagDesp,
		CStringArray &m_strCoalTagUnit
		)
{
	this->m_strCoalTagName.Copy(m_strCoalTagName);
	this->m_strCoalTagDesp.Copy(m_strCoalTagDesp);
	this->m_strCoalTagUnit.Copy(m_strCoalTagUnit);
}


BOOL CPropertyDlgCoalData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			CString str1=m_ArrSourceRDBType[i];
			CString str2=m_ArrSourceRDBType[i];

			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBName);
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBName)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}
	initListBoxHead();
	initListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgCoalData::initListBoxHead()
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
void CPropertyDlgCoalData::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<m_strCoalTagName.GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,(m_strCoalTagName)[i]);
		m_List.SetItemText(nItem,2,(m_strCoalTagDesp)[i]);
		m_List.SetItemText(nItem,3,(m_strCoalTagUnit)[i]);
		nItem++;
	}
}
void CPropertyDlgCoalData::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(this->m_strTableName=="")
	{
		AfxMessageBox("请输入数据源表名称！");
		return;
	}
	if(this->m_iDays==0)
	{
		AfxMessageBox("请输入预匹配时间段长度!");
		return;
	}
	CDialog::OnOK();
}

void CPropertyDlgCoalData::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBName=despArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBName="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);
	UpdateData(false);
}

void CPropertyDlgCoalData::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
}

void CPropertyDlgCoalData::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	// TODO: Add your control notification handler code here
	if(this->m_TagTimeOffsetOrInput==0)
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET)->EnableWindow(true);
	}
}

void CPropertyDlgCoalData::OnButtonAdd() 
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

		(m_strCoalTagName).Add(dlg.m_strTagName);
		(m_strCoalTagDesp).Add(dlg.m_strDesp);
		(m_strCoalTagUnit).Add(dlg.m_strUnit);
		nItem++;
	}
}

void CPropertyDlgCoalData::OnButtonDel() 
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
		(m_strCoalTagName).RemoveAt(delIndexArr[i]);
		(m_strCoalTagDesp).RemoveAt(delIndexArr[i]);
		(m_strCoalTagUnit).RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgCoalData::OnButtonEdit() 
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
			
			(m_strCoalTagName)[index]=dlg.m_strTagName;
			(m_strCoalTagDesp)[index]=dlg.m_strDesp;
			(m_strCoalTagUnit)[index]=dlg.m_strUnit;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的测点标签!");
	}
}
