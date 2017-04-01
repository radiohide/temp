// PropertyDlgCoalK.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgCoalK.h"
#include "DlgCoalFlowTagData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalK dialog


CPropertyDlgCoalK::CPropertyDlgCoalK(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgCoalK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgCoalK)
	m_TimeE = 0;
	m_TimeS = 0;
	m_iBDay = 0;
	m_iFDay = 0;
	m_StrPowerUnit = _T("");
	m_iStepLength = 0;
	m_strPowerTagName = _T("");
	m_lOffset = 0;
	m_TagTimeOffsetOrInput = -1;
	m_strTableName = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgCoalK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgCoalK)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Control(pDX, IDC_COMBO_STEPUNIT, m_ComboBoxStepUnit);
	DDX_Control(pDX, IDC_LIST_FLOW, m_List);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ETIME, m_TimeE);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STIME, m_TimeS);
	DDX_Text(pDX, IDC_EDIT_BDAY, m_iBDay);
	DDX_Text(pDX, IDC_EDIT_FDAY, m_iFDay);
	DDX_Text(pDX, IDC_EDIT_POWER_UNIT, m_StrPowerUnit);
	DDX_Text(pDX, IDC_EDIT_STEP, m_iStepLength);
	DDX_Text(pDX, IDC_EDIT_TAGNAME_POWER, m_strPowerTagName);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_lOffset);
	DDX_Radio(pDX, IDC_RADIO5, m_TagTimeOffsetOrInput);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgCoalK, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgCoalK)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FLOW, OnButtonAddFlow)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FLOW, OnButtonEditFlow)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FLOW, OnButtonDelFlow)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FLOW, OnDblclkListFlow)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgCoalK message handlers

BOOL CPropertyDlgCoalK::OnInitDialog() 
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

	if(m_iStepUnit>0)
		this->m_ComboBoxStepUnit.SetCurSel(m_iStepUnit-1);
	else
		this->m_ComboBoxStepUnit.SetCurSel(2);//默认选分钟

	initListBoxHead();
	initListBoxBody();

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgCoalK::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgCoalK::SetSETime(int iSHour,int iSMinute,int iSSecond,int iEHour,int iEMinute,int iESecond)
{
	m_TimeS=CTime(2011,1,1,iSHour,iSMinute,iSSecond);
	m_TimeE=CTime(2011,1,1,iEHour,iEMinute,iESecond);
}
void CPropertyDlgCoalK::SetStepUnit(int iStepUnit)//iStepUnit=1:秒；2:分钟；3:小时
{
	this->m_iStepUnit=iStepUnit;
}
void CPropertyDlgCoalK::SetFlowTagData(CStringArray &strFlowTagNameArr,
						CStringArray &strFlowUnitArr,
						CStringArray &strFlowDespArr)
{
	m_strFlowTagNameArr.Copy(strFlowTagNameArr);
	m_strFlowUnitArr.Copy(strFlowUnitArr);
	m_strFlowDespArr.Copy(strFlowDespArr);
}

void CPropertyDlgCoalK::initListBoxHead()
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
void CPropertyDlgCoalK::initListBoxBody()
{
	int nItem=0;
	for(int i=0;i<this->m_strFlowTagNameArr.GetSize();i++)
	{
		CString str;
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,m_strFlowTagNameArr[i]);
		m_List.SetItemText(nItem,2,this->m_strFlowDespArr[i]);
		m_List.SetItemText(nItem,3,this->m_strFlowUnitArr[i]);
		nItem++;
	}
}

void CPropertyDlgCoalK::OnButtonAddFlow() 
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

		m_strFlowTagNameArr.Add(dlg.m_strTagName);
		m_strFlowDespArr.Add(dlg.m_strDesp);
		m_strFlowUnitArr.Add(dlg.m_strUnit);
		nItem++;
	}
}

void CPropertyDlgCoalK::OnButtonEditFlow() 
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
			
			m_strFlowTagNameArr[index]=dlg.m_strTagName;
			m_strFlowDespArr[index]=dlg.m_strDesp;
			m_strFlowUnitArr[index]=dlg.m_strUnit;
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的测点标签!");
	}
}

void CPropertyDlgCoalK::OnButtonDelFlow() 
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
		m_strFlowTagNameArr.RemoveAt(delIndexArr[i]);
		m_strFlowDespArr.RemoveAt(delIndexArr[i]);
		m_strFlowUnitArr.RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgCoalK::OnDblclkListFlow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEditFlow();
	*pResult = 0;
}

void CPropertyDlgCoalK::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(this->m_strTableName=="")
	{
		AfxMessageBox("数据源表名称！");
		return;
	}
	if(this->m_strPowerTagName=="")
	{
		AfxMessageBox("请输入功率测点标签名！");
		return;
	}
	if(this->m_strFlowTagNameArr.GetSize()==0)
	{
		AfxMessageBox("请输入流量测点标签名！");
		return;
	}

	this->m_iStepUnit=this->m_ComboBoxStepUnit.GetCurSel()+1;
	CDialog::OnOK();
}

void CPropertyDlgCoalK::OnSelchangeComboDbsel() 
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

void CPropertyDlgCoalK::OnRadio5() 
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

void CPropertyDlgCoalK::OnRadio6() 
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
