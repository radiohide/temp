// PropertyDlgStatEnum.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStatEnum.h"
#include "DlgVarSEValue.h"
#include "DlgStatAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatEnum dialog


CPropertyDlgStatEnum::CPropertyDlgStatEnum(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStatEnum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStatEnum)
	m_strVarName = _T("");
	m_strVarUnit = _T("");
	m_strVarDesp = _T("");
	m_lSnapshotT = 0;
	m_iReqType = -1;
	m_iStatType = -1;
	m_strOutName = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgStatEnum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStatEnum)
	DDX_Control(pDX, IDC_COMBO_REQ_TYPE, m_ComBoReqType);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT_VARNAME, m_strVarName);
	DDX_Text(pDX, IDC_EDIT_STR_UNIT, m_strVarUnit);
	DDX_Text(pDX, IDC_EDIT_DESP, m_strVarDesp);
	DDX_Text(pDX, IDC_EDIT_SNAP_TIME, m_lSnapshotT);
	DDX_CBIndex(pDX, IDC_COMBO_REQ_TYPE, m_iReqType);
	DDX_CBIndex(pDX, IDC_COMBO_STAT_TYPE, m_iStatType);
	DDX_Text(pDX, IDC_EDIT_OUTNAME2, m_strOutName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyDlgStatEnum, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStatEnum)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_CBN_SELCHANGE(IDC_COMBO_REQ_TYPE, OnSelchangeComboReqType)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_EN_CHANGE(IDC_EDIT_VARNAME, OnChangeEditVarname)
	ON_BN_CLICKED(IDC_BUTTON_ADV, OnButtonAdv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatEnum message handlers

BOOL CPropertyDlgStatEnum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
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
void CPropertyDlgStatEnum::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgStatEnum::initListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","区间开始(","]区间结束","状态值","高\\低限制"};
					//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 80);	
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 80);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 60);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
}
void CPropertyDlgStatEnum::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->m_dSValueArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		str.Format("%f",m_dSValueArr[i]);
		m_List.SetItemText(nItem,1,str);
		str.Format("%f",m_dEValueArr[i]);
		m_List.SetItemText(nItem,2,str);
		str.Format("%d",m_iSETag[i]);
		m_List.SetItemText(nItem,3,str);
		str.Format("%d",m_iUpOrDowmLimit[i]);
		m_List.SetItemText(nItem,4,str);	
		nItem++;
	}
}

void CPropertyDlgStatEnum::OnSelchangeComboDbsel() 
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

void CPropertyDlgStatEnum::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strVarName=="")
	{
		AfxMessageBox("变量名不能为空，请输入！");
		return;
	}
	if(m_strOutName=="")
	{
		AfxMessageBox("输出名称不能为空，请输入！");
		return;
	}
	if((m_iReqType==0)&&(m_lSnapshotT<=0))
	{
		AfxMessageBox("取快照周期为正整数，请重新输入");
		return;
	}
	if(m_dSValueArr.GetSize()==0)
	{
		AfxMessageBox("请输入区间数据！");
		return;
	}
	bool beSameState=false;
	for(int i=0;i<m_iSETag.GetSize();i++)
	{
		for(int j=i+1;j<m_iSETag.GetSize();j++)
		{
			if(m_iSETag[i]==m_iSETag[j])
			{
				beSameState=true;
				break;
			}
		}
	}
	if(beSameState)
	{
		AfxMessageBox("存在相同的区间状态，请重新输入！");
		return;
	}

	CDialog::OnOK();
}

void CPropertyDlgStatEnum::OnSelchangeComboReqType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selIndex=m_ComBoReqType.GetCurSel ();
	if(selIndex==0)
	{
		m_iReqType=0;
		GetDlgItem(IDC_EDIT_SNAP_TIME)->EnableWindow(true);
	}
	else if(selIndex==1)
	{
		m_iReqType=1;
		GetDlgItem(IDC_EDIT_SNAP_TIME)->EnableWindow(false);
	}
}

void CPropertyDlgStatEnum::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgVarSEValue dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_dSValueArr.Add(dlg.m_dSValue);
		m_dEValueArr.Add(dlg.m_dEValue);
		m_iSETag.Add(dlg.m_iState);
		m_iUpOrDowmLimit.Add(dlg.m_iUpOrDown);
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStatEnum::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_List.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("请选择区间。");
		 return;
	}
	CDlgVarSEValue dlg;
	dlg.m_dSValue=m_dSValueArr[nSelectedItem];
	dlg.m_dEValue=m_dEValueArr[nSelectedItem];
	dlg.m_iState=m_iSETag[nSelectedItem];
	dlg.m_iUpOrDown=m_iUpOrDowmLimit[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		m_dSValueArr[nSelectedItem]=dlg.m_dSValue;
		m_dEValueArr[nSelectedItem]=dlg.m_dEValue;
		m_iSETag[nSelectedItem]=dlg.m_iState;
		m_iUpOrDowmLimit[nSelectedItem]=dlg.m_iUpOrDown;
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStatEnum::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CArray<int,int> indexSelArr;
	while(pos != NULL)
	{
		int iSel= m_List.GetNextSelectedItem(pos);
		indexSelArr.Add(iSel);
	}
	if(indexSelArr.GetSize()==0)
	{ 
		 AfxMessageBox("请选择变量。");
		 return;
	}
	for(int i=0;i<indexSelArr.GetSize();i++)//索引从大到小排序
	{
		for(int j=i+1;j<indexSelArr.GetSize();j++)
		{
			if(indexSelArr[i]<indexSelArr[j])
			{
				int iTemp=indexSelArr[i];
				indexSelArr[i]=indexSelArr[j];
				indexSelArr[j]=iTemp;
			}
		}
	}
	for(i=0;i<indexSelArr.GetSize();i++)
	{
		m_dSValueArr.RemoveAt(indexSelArr[i]);
		m_dEValueArr.RemoveAt(indexSelArr[i]);
		m_iSETag.RemoveAt(indexSelArr[i]);
		m_iUpOrDowmLimit.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgStatEnum::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}

void CPropertyDlgStatEnum::OnChangeEditVarname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(true);
	m_strOutName=m_strVarName;
	UpdateData(false);
	// TODO: Add your control notification handler code here
	
}

void CPropertyDlgStatEnum::OnButtonAdv() 
{
	// TODO: Add your control notification handler code here
	CDlgStatAdv dlg;
	if(this->m_iTagTimeIntM==1)
	{
		dlg.m_BCheckTimeMInt=TRUE;
	}
	else
	{
		dlg.m_BCheckTimeMInt=FALSE;
	}
	if(IDOK==dlg.DoModal())
	{
		if(dlg.m_BCheckTimeMInt)
		{
			this->m_iTagTimeIntM=1;
		}
		else
		{
			this->m_iTagTimeIntM=0;
		}
	}
}
