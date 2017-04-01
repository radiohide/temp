// PropertyDlgOptimalRule.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgOptimalRule.h"
#include "PropertyDlgTabOptRuleFun.h"
#include "PropertyDlgTabRuleVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgOptimalRule dialog

CPropertyDlgOptimalRule::CPropertyDlgOptimalRule(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgOptimalRule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgOptimalRule)
	m_iUnit=-1;
	m_iSet=-1;
	m_strDBName="";
	m_iRuleID=-1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgOptimalRule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgOptimalRule)
	DDX_Control(pDX, IDC_COMBO_RULELIST, m_comboBoxRuleList);
	DDX_Control(pDX, IDC_COMBO_UNITSEL, m_comboBoxUnitSel);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Control(pDX, IDC_TABLIST, m_TabList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgOptimalRule, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgOptimalRule)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABLIST, OnSelchangeTablist)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbSel)
	ON_CBN_SELCHANGE(IDC_COMBO_UNITSEL, OnSelchangeComboUnitSel)
	ON_CBN_SELCHANGE(IDC_COMBO_RULELIST, OnSelchangeComboRulelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgOptimalRule message handlers

BOOL CPropertyDlgOptimalRule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initTabCtrl();//初始化Tab页
	
	//初始化关系数据源
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			CString str1=m_ArrSourceRDBType[i];
			CString str2=m_ArrSourceRDBType[i];

			nameArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBName);
	int iBeFind=-1;
	for(i=0;i<nameArr.GetSize();i++)
	{
		if(nameArr[i]==m_strDBName)
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

	//初始化机组信息
	readUnitList();
	comboBoxUnitSelAddStr();
	iBeFind=-1;
	for(i=0;i<unitNameArr.GetSize();i++)
	{
		if((unitArr[i]==m_iUnit)&&(setsArr[i]==m_iSet))
		{
			iBeFind=i;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxUnitSel.SetCurSel(iBeFind);
	}

	//初始化规则信息
	readRuleList();
	comboBoxRuleListAddStr();
	iBeFind=-1;
	for(i=0;i<ruleIDArr.GetSize();i++)
	{
		if(ruleIDArr[i]==m_iRuleID)
		{
			iBeFind=i;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxRuleList.SetCurSel(iBeFind);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CPropertyDlgOptimalRule::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s limit 0,1",strTableName);
	try
	{
		pRecord->Open((const char*)strSQL,pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	} 
	return TRUE;
}
BOOL CPropertyDlgOptimalRule::readUnitList()
{
	unitNameArr.RemoveAll();
	unitArr.RemoveAll();
	setsArr.RemoveAll();
	
	if(m_strDBName=="")//当前块还没有配置数据源
		return FALSE;
	int tagBeCon=0;
	CString strDBType="";

	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
	{
		AfxMessageBox("当前关系数据源未连接，未能正确加载机组信息！");
		return FALSE;
	}
	if(!gIsTableExist(m_pConRDB,"t_unitlist"))
	{
		AfxMessageBox("当前数据源中不存在't_unitlist'表，可能该数据源不是模式库！");
		return FALSE;
	}
	_RecordsetPtr m_pRec;
	try   
	{
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset));
		CString strSql;
		strSql.Format("select  * from t_unitlist");
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		//m_pConRDB-> Execute("set character_set_connection=gbk",&vtMissing,adCmdText);
		//m_pConRDB-> Execute("SET NAMES GBK",&vtMissing,adCmdText);
	}
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}  
	while(!m_pRec->adoEOF)
	{
		CString strUnit =(char *)_bstr_t(m_pRec->GetCollect("unit_unit"));
		CString strSets =(char *)_bstr_t(m_pRec->GetCollect("unit_sets"));
		CString strUnitName =(char *)_bstr_t(m_pRec->GetCollect("unit_name"));
		unitNameArr.Add(strUnitName);
		unitArr.Add(atoi(strUnit));
		setsArr.Add(atoi(strSets));
		m_pRec->MoveNext();
	}
	return TRUE;
}
void CPropertyDlgOptimalRule::SetPtrModel(CCalcModel *p)
{
	m_pModel=p;
}
void CPropertyDlgOptimalRule::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgOptimalRule::initTabCtrl()//初始化Tab控件的时候，创建每页所需要的数据
{
	CRect rc;
	m_TabList.GetClientRect(rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 1;
	
	m_TabList.InsertItem(0,_T("配置最优规则判断方法"));
	pDlgFun=new CPropertyDlgTabOptRuleFun;
	pDlgFun->SetRuleData(&m_iCGArr,&m_strFunExpArr,&m_strFunDepArr);
	pDlgFun->Create(IDD_DIALOG_TAB_OPTRULEFUN,&m_TabList);//必须执行create
	pDlgFun->MoveWindow(rc);
	//pDlgFun->m_List.MoveWindow(rc.left,rc.top-15,rc.Width(),rc.Height()-10);
	pDlgFun->ShowWindow(SW_SHOW);

	m_TabList.InsertItem(1,_T("选择关注的变量"));
	pDlgVar=new CPropertyDlgTabRuleVar;
	pDlgVar->SetRDBInf(m_pModel,&m_strDBName,&m_iUnit,&m_iSet);
	pDlgVar->SetVarData(&m_iVarCGArr,&m_strVarNameArr,&m_strVarDespArr,&m_StrVarUnit);
	pDlgVar->Create(IDD_DIALOG_TAB_RULEVAR,&m_TabList);//必须执行create
	pDlgVar->MoveWindow(rc);
	//pDlgVar->m_List.MoveWindow(rc.left,rc.top-15,rc.Width(),rc.Height()-10);
	pDlgVar->ShowWindow(SW_HIDE);
}

void CPropertyDlgOptimalRule::OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(0==m_TabList.GetCurSel())
	{
		pDlgFun->ShowWindow(SW_SHOW);
		pDlgVar->ShowWindow(SW_HIDE);
	}
	else 
	{
		pDlgFun->ShowWindow(SW_HIDE);
		pDlgVar->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CPropertyDlgOptimalRule::OnSelchangeComboDbSel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBName=nameArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBName="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);

	readUnitList();
	comboBoxUnitSelAddStr();
	m_comboBoxUnitSel.SetCurSel(-1);
	m_iUnit=-1;
	m_iSet=-1;

	readRuleList();
	comboBoxRuleListAddStr();
	m_comboBoxRuleList.SetCurSel(-1);
	m_iRuleID=-1;

	UpdateData(false);
}
void CPropertyDlgOptimalRule::comboBoxUnitSelAddStr() 
{
	m_comboBoxUnitSel.ResetContent();
	for(int i=0;i<unitNameArr.GetSize();i++)
	{
		this->m_comboBoxUnitSel.AddString(unitNameArr[i]);
	}
}

void CPropertyDlgOptimalRule::OnSelchangeComboUnitSel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxUnitSel.GetCurSel();
	if(selIndex>=0)
	{
		m_iUnit=unitArr[selIndex];
		m_iSet=setsArr[selIndex];
	}
	else
	{
		m_iUnit=-1;
		m_iSet=-1;
	}
	m_comboBoxUnitSel.SetCurSel(selIndex);

	readRuleList();
	comboBoxRuleListAddStr();
	m_iRuleID=-1;

	UpdateData(false);
}
BOOL CPropertyDlgOptimalRule::readRuleList()
{
	ruleIDArr.RemoveAll();//规则ID
	ruleNameArr.RemoveAll();//规则名称
	ruleDespArr.RemoveAll();//规则描述
	
	if((m_iUnit==-1)||(m_iSet==-1))//当前块还没有配置数据源
		return FALSE;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
	{
		AfxMessageBox("当前关系数据源未连接，未能正确加载机组信息！");
		return FALSE;
	}
	CString strTableName;
	strTableName.Format("t_u%ds%d_modle_rulelist",m_iUnit,m_iSet);
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		AfxMessageBox("当前数据源中不存在't_unitlist'表，可能该数据源不是模式库！");
		return FALSE;
	}
	_RecordsetPtr m_pRec;
	try   
	{
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset));
		CString strSql;
		strSql.Format("select  * from %s",strTableName);
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		//m_pConRDB-> Execute("set character_set_connection=gbk",&vtMissing,adCmdText);
		//m_pConRDB-> Execute("SET NAMES GBK",&vtMissing,adCmdText);
	}
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}  
	while(!m_pRec->adoEOF)
	{
		CString strRuleID =(char *)_bstr_t(m_pRec->GetCollect("RuleID"));
		CString strRuleName =(char *)_bstr_t(m_pRec->GetCollect("rule_name"));
		CString strRuleDesp =(char *)_bstr_t(m_pRec->GetCollect("rule_desp"));
		ruleIDArr.Add(atoi(strRuleID));
		ruleNameArr.Add(strRuleName);
		ruleDespArr.Add(strRuleDesp);
		m_pRec->MoveNext();
	}
	return TRUE;
}
void CPropertyDlgOptimalRule::comboBoxRuleListAddStr() 
{
	m_comboBoxRuleList.ResetContent();
	for(int i=0;i<ruleNameArr.GetSize();i++)
	{
		this->m_comboBoxRuleList.AddString(ruleNameArr[i]);
	}
}


void CPropertyDlgOptimalRule::OnSelchangeComboRulelist() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxRuleList.GetCurSel();
	if(selIndex>=0)
	{
		m_iRuleID=ruleIDArr[selIndex];
	}
	else
	{
		m_iRuleID=-1;
	}
	m_comboBoxUnitSel.SetCurSel(selIndex);
}
