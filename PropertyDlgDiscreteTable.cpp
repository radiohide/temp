// PropertyDlgDiscreteTable.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgDiscreteTable.h"
#include "DlgDiscreteValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDiscreteTable dialog


CPropertyDlgDiscreteTable::CPropertyDlgDiscreteTable(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgDiscreteTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgDiscreteTable)
	m_strTableName = _T("");
	m_strDBType = _T("");
	//}}AFX_DATA_INIT
	iComIndex=0;
	m_strDBDesp="";

	m_pModelPro=NULL;
	m_dFromInput=0;
}


void CPropertyDlgDiscreteTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgDiscreteTable)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_ComDBSel);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_DBTYPE, m_strDBType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgDiscreteTable, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgDiscreteTable)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgDiscreteTable message handlers
void CPropertyDlgDiscreteTable::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
BOOL CPropertyDlgDiscreteTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_ComDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}

	this->m_ComDBSel.SetCurSel(iComIndex);
	//m_strDBDesp=m_ArrSourceRDBName[iComIndex];//2012-2-2注释掉
	//m_strDBType=typeArr[iComIndex];	//2012-2-2注释掉
	
	if((iComIndex<despArr.GetSize())&&(iComIndex>-1))//2012-2-2修改
	{
		m_strDBDesp=despArr[iComIndex];
		m_strDBType=typeArr[iComIndex];	
	}

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgDiscreteTable::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	iComIndex=this->m_ComDBSel.GetCurSel();
	if(iComIndex>=0)
	{
		CString str=typeArr[iComIndex];
		m_strDBDesp=despArr[iComIndex];
		m_strDBType=typeArr[iComIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBDesp=str;
		m_strDBType="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	UpdateData(false);
}

void CPropertyDlgDiscreteTable::OnButtonSearch() 
{
	
	m_division_code.RemoveAll();
	m_code_desp.RemoveAll();
	m_low_value.RemoveAll();
	m_high_value.RemoveAll();
	// TODO: Add your control notification handler code here
	
	UpdateData(true);
	
	if(m_strDBType=="Mysql")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModelPro->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
//		m_pConRDB-> Execute("SET   NAMES   gbk",&vtMissing,adCmdText); 
		
		if(!gIsTableExist(m_pConRDB,m_strTableName))
		{
			AfxMessageBox("数据库表不存在!");
			return;
		}
		_RecordsetPtr  pRecord;
		//检索到所有属性
		try   
		{   
			HRESULT hr;
			hr = pRecord.CreateInstance(__uuidof(Recordset)); 
			CString strSQL;
			strSQL.Format("SELECT * FROM %s",m_strTableName);
			pRecord->Open((char *)_bstr_t(strSQL),_variant_t((IDispatch*)m_pConRDB,true),
				adOpenStatic,adLockOptimistic,adCmdText);		
		}   
		
		catch(_com_error e)
		{
			pRecord->Close() ;
			return ;
		}
		int iRowNO=0;
		while(!pRecord->adoEOF)
		{
			
			CString U_division_code =(char *)_bstr_t(pRecord->GetCollect("division_code"));	
			m_division_code.Add(U_division_code);
			
			CString U_code_despe =(char *)_bstr_t(pRecord->GetCollect("code_desp"));
			m_code_desp.Add(U_code_despe);		
			
			CString U_low_value =(char *)_bstr_t(pRecord->GetCollect("low_value"));	
			m_low_value.Add(U_low_value);
			
			CString U_high_value =(char *)_bstr_t(pRecord->GetCollect("high_value"));
			m_high_value.Add(U_high_value);
			
			iRowNO++;
			
			pRecord->MoveNext();
		}
		pRecord->Close();
	}
	else if(m_strDBType=="MDB")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModelPro->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);

		if(!gIsTableExist(m_pConRDB,m_strTableName))
		{
			AfxMessageBox("数据库表不存在!");
			return;
		}

		_RecordsetPtr  pRecord;
		_variant_t RecordsAffected;

		try
		{
			pRecord.CreateInstance("ADODB.Recordset"); //为Recordset对象创建实例
			CString strSQL;
			strSQL.Format("SELECT * FROM %s",m_strTableName);
			pRecord=m_pConRDB->Execute(bstr_t(strSQL),&RecordsAffected,adCmdText);
			
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());
		}
		
		_variant_t vdivision_code,vcode_desp,vLastName,vlow_value,vhigh_value;
		CString strtemp;
		int iRowNO=0;
		try
		{
			while(!pRecord->adoEOF)
			{
				vdivision_code=pRecord->GetCollect("division_code");
				strtemp.Format("%s",vdivision_code.lVal);
				m_division_code.Add(strtemp);				
				
				vcode_desp=pRecord->GetCollect("code_desp");
				strtemp.Format("%s",vcode_desp.lVal);
				m_code_desp.Add(strtemp);
				
				vlow_value=pRecord->GetCollect("low_value");
				strtemp.Format("%s",vlow_value.lVal);
				m_low_value.Add(strtemp);
				
				vhigh_value=pRecord->GetCollect("high_value");
				strtemp.Format("%s",vhigh_value.lVal);
				m_high_value.Add(strtemp);
				
				iRowNO++;
				pRecord->MoveNext();
			}			
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());
		}		
	}

	//
    CDlgDiscreteValue  dlg;
	dlg.m_division_code=&m_division_code;
	dlg.m_code_desp=&m_code_desp;
	dlg.m_low_value=&m_low_value;
	dlg.m_high_value=&m_high_value;
	dlg.DoModal();

}
BOOL CPropertyDlgDiscreteTable::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
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

void CPropertyDlgDiscreteTable::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(true);
	
	if(m_strDBType=="Mysql")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModelPro->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
//		m_pConRDB-> Execute("SET   NAMES   gbk",&vtMissing,adCmdText); 
		
		if(!gIsTableExist(m_pConRDB,m_strTableName))
		{
			AfxMessageBox("数据库表不存在!");
			return;
		}	
	}
	else if(m_strDBType=="MDB")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModelPro->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);

		if(!gIsTableExist(m_pConRDB,m_strTableName))
		{
			AfxMessageBox("数据库表不存在!");
			return;
		}		
	}

	CDialog::OnOK();
}
