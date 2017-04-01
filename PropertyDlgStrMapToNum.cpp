// PropertyDlgStrMapToNum.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStrMapToNum.h"
#include "PropertyDlgStrToNumData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMapToNum dialog


CPropertyDlgStrMapToNum::CPropertyDlgStrMapToNum(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStrMapToNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStrMapToNum)
	m_pModel=NULL;
	m_strTableName = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgStrMapToNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStrMapToNum)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Text(pDX, IDC_EDIT_DBTABLE_NAME, m_strTableName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStrMapToNum, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStrMapToNum)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStrMapToNum message handlers
void CPropertyDlgStrMapToNum::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgStrMapToNum::SetModelPtr(CCalcModel *pModel)
{
	m_pModel=pModel;
}
BOOL CPropertyDlgStrMapToNum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBDesp);
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBDesp)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
		m_strDBType=typeArr[iBeFind];
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgStrMapToNum::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBDesp=despArr[selIndex];
		m_strDBType=typeArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBDesp="";
		m_strDBType="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);
	UpdateData(false);
}

void CPropertyDlgStrMapToNum::OnButtonQuery() 
{
	strValueArr.RemoveAll();
	dValueArr.RemoveAll();
	strDespArr.RemoveAll();
	// TODO: Add your control notification handler code here
	
	UpdateData(true);
	if(m_strDBType=="Mysql")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
		
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
			return ;
		}
		while(!pRecord->adoEOF)
		{
			CString strValue =(char *)_bstr_t(pRecord->GetCollect("StrValue"));
			double dValue=atof((char *)_bstr_t(pRecord->GetCollect("NumValue")));
			CString strDesp =(char *)_bstr_t(pRecord->GetCollect("Desp"));
			strValueArr.Add(strValue);
			dValueArr.Add(dValue);
			strDespArr.Add(strDesp);
			pRecord->MoveNext();
		}
		pRecord->Close();
	}
	else if(m_strDBType=="MDB")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
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
			return ;
		}		
		try
		{
			while(!pRecord->adoEOF)
			{
				CString strValue =(char *)_bstr_t(pRecord->GetCollect("StrValue"));
				double dValue=atof((char *)_bstr_t(pRecord->GetCollect("NumValue")));
				CString strDesp =(char *)_bstr_t(pRecord->GetCollect("Desp"));
				strValueArr.Add(strValue);
				dValueArr.Add(dValue);
				strDespArr.Add(strDesp);
				pRecord->MoveNext();
			}			
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());
		}		
	}
    CPropertyDlgStrToNumData  dlg;
	dlg.pStrValueArr=&strValueArr;
	dlg.pDValueArr=&dValueArr;
	dlg.pStrDespArr=&strDespArr;
	dlg.DoModal();
}

void CPropertyDlgStrMapToNum::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(m_strDBType=="Mysql")
	{
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);		
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
		_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);

		if(!gIsTableExist(m_pConRDB,m_strTableName))
		{
			AfxMessageBox("数据库表不存在!");
			return;
		}		
	}

	CDialog::OnOK();
}
