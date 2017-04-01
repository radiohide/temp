// PropertyDlgStat.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStat.h"
#include "DlgStatVarSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStat dialog


CPropertyDlgStat::CPropertyDlgStat(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStat)
	m_iVarType=0;//1:accum;2enum;3aver;4impt
	m_iSETimeType = -1;
	m_pModel=NULL;
	//}}AFX_DATA_INIT
}


void CPropertyDlgStat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStat)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Radio(pDX, IDC_RADIOSE1, m_iSETimeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStat, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStat)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_VAR_SEL, OnButtonVarSel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FLOW, OnButtonDelFlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStat message handlers
void CPropertyDlgStat::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
BOOL CPropertyDlgStat::OnInitDialog() 
{
	CDialog::OnInitDialog();

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
	
	this->SetWindowText(this->m_strTitle);
	GetDlgItem(IDC_STATIC_BLOCK_DESP)->SetWindowText(this->m_strBlockDesp);
	if(this->m_iVarType==4)//引入型变量不能选择采用数据时间
		GetDlgItem(IDC_RADIOSE3)->ShowWindow(false);
	if(this->m_iVarType!=2)//非枚举类型，需隐藏枚举累计策略设定框
	{
		GetDlgItem(IDC_STATIC_ENUM)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_UP)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_DOWN)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_VALUE_UP)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_VALUE_DOWN)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_CODE_DOWN)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_CODE_UP)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_CODE_UP)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_CODE_DOWN)->ShowWindow(false);

		CRect recButtonOK;
		GetDlgItem(IDOK)->GetWindowRect(&recButtonOK);
		ScreenToClient(recButtonOK);
		recButtonOK.top-=78;
		recButtonOK.bottom-=78;
		GetDlgItem(IDOK)->MoveWindow(&recButtonOK);

		CRect recButtonCancle;
		GetDlgItem(IDCANCEL)->GetWindowRect(&recButtonCancle);
		ScreenToClient(recButtonCancle);
		recButtonCancle.top-=78;
		recButtonCancle.bottom-=78;
		GetDlgItem(IDCANCEL)->MoveWindow(&recButtonCancle);

		CRect rec;
		this->GetWindowRect(&rec);
		//ScreenToClient(rec);
		rec.bottom-=78;
		this->MoveWindow(&rec);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgStat::initListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","测点名称","描述","单位","机组"};
					//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 150);	
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 60);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 60);
}
void CPropertyDlgStat::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<this->m_strVarNameArr.GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,m_strVarNameArr[i]);
		m_List.SetItemText(nItem,2,this->m_strVarDespArr[i]);
		m_List.SetItemText(nItem,3,this->m_strVarUnitArr[i]);
		m_List.SetItemText(nItem,4,this->m_strUnitSetArr[i]);
		nItem++;
	}
}
void CPropertyDlgStat::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(this->m_List.GetItemCount()==0)
	{
		AfxMessageBox("请选择要统计的变量！");
		return;
	}
	m_strVarNameArr.RemoveAll();//变量名称
	m_strVarDespArr.RemoveAll();//变量描述
	m_strVarUnitArr.RemoveAll();//机组类型
	m_strUnitSetArr.RemoveAll();//机组名称
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		CString strVarName=m_List.GetItemText(i,1);
		CString strVarDesp=m_List.GetItemText(i,2);
		CString strVarUnit=m_List.GetItemText(i,3);
		CString strUnitSet=m_List.GetItemText(i,4);
		
		m_strVarNameArr.Add(strVarName);
		m_strVarDespArr.Add(strVarDesp);
		m_strVarUnitArr.Add(strVarUnit);
		m_strUnitSetArr.Add(strUnitSet);
	}
	CDialog::OnOK();
}

void CPropertyDlgStat::OnSelchangeComboDbsel() 
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

void CPropertyDlgStat::OnButtonVarSel() 
{
	// TODO: Add your control notification handler code here
	CDlgStatVarSel dlg;
	dlg.m_strDBName=this->m_strDBName;
	dlg.m_iVarType=m_iVarType;
	dlg.m_pModel=m_pModel;
	if(IDOK==dlg.DoModal())
	{
		for(int i=0;i<dlg.m_strSelVarNameArr.GetSize();i++)
		{
			addVarToList(dlg.m_strSelVarNameArr[i],dlg.m_strSelVarDespArr[i],dlg.m_strSelVarUnitArr[i],
				dlg.m_iEditUnit,dlg.m_iEditSet);
		}
	}
}
void CPropertyDlgStat::addVarToList(CString strVarName,CString strDesp,CString strUnit,
									int iUnit,int iSet)
{
	CString str;
	bool bFind=false;
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		CString strName=m_List.GetItemText(i,1);
		if(strName==strVarName)
		{
			m_List.SetItemText(i,1,strVarName);
			str.Format("u%ds%d",iUnit,iSet);
			m_List.SetItemText(i,4,str);
			bFind=true;
		}
	}
	if(!bFind)
	{
		int nCount=m_List.GetItemCount();
		str.Format("%d",nCount);
		m_List.InsertItem(nCount,str);
		m_List.SetItemText(nCount,1,strVarName);
		m_List.SetItemText(nCount,2,strDesp);
		m_List.SetItemText(nCount,3,strUnit);
		str.Format("u%ds%d",iUnit,iSet);
		m_List.SetItemText(nCount,4,str);
	}
}

void CPropertyDlgStat::OnButtonDelFlow() 
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
	}
}
