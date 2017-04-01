// DBOperator.cpp: implementation of the CDBOperator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "DBOperator.h"
////////////////////////////////////////////////
#include "FlowchartEditor/DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBOperator::CDBOperator()
{

}

CDBOperator::~CDBOperator()
{

}

BOOL  CDBOperator::CreateTable(_ConnectionPtr &pCon,CString strTableName,CStringArray& Fields,CStringArray& FieldTypes)
{
	BOOL bRet = TRUE;
	CString strField,strFieldType;
    CString strSQL;
	//构造字段
	int size = Fields.GetSize();
	strField.Empty();
	for (int i = 0;i<size;i++)
	{
		//分析类型
		CString strType;
		CTokenizer tok(FieldTypes.GetAt(i),_T(":"));
		int num = tok.GetSize();
		if(num ==1)
		{
			tok.GetAt(0,strType);
			if(strType ==_T("short"))
			{
				strFieldType = _T("SmallInt");
			}
			else if(strType ==_T("double"))
			{
				strFieldType = _T("Float");
			}
		}
		else if(num==2)
		{
              tok.GetAt(0,strType);
			 if(strType ==_T("string"))
			 {
				 CString strlen;
			      tok.GetAt(1,strlen);
				  if(atoi(strlen)>255)
				  {
					  //strFieldType.Format("NTEXT");
					  strFieldType.Format("memo");
				  }
				  else
				  {
					  //strFieldType.Format("TEXT(%s)",strlen);
					  strFieldType.Format("VarChar(%s)",strlen);
				  }
			 }
		}
		else 
		{
			TRACE("error\n\r");
		}
		strField = strField +" [" + Fields.GetAt(i) + "] " + strFieldType;
		if (i != size-1)
			strField = strField + ",";
	}

	strSQL.Format("CREATE TABLE %s ( %s )",strTableName,strField);

	 try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	   //rename table modellist_copy to ddddd//mysql 重命名语句
	   //strSQL.Format("select * into bb from aa");
	   //pCon->Execute((const char*)strSQL,0,adCmdText); 
	   
	}
   	catch(_com_error e)
	{
		CString str;
		str.Format("%s",e.ErrorMessage());
		AfxMessageBox(str);
	    bRet =  FALSE;
	}
	return bRet;
}
BOOL  CDBOperator::RenameTable(_ConnectionPtr &pCon,CString strOldName,CString strNewName)
{
	BOOL bRet=TRUE;
	ADOX::_CatalogPtr m_pCatalog = NULL;
	ADOX::_TablePtr m_pTable = NULL;
	try 
	{

		HRESULT hr = S_OK;
		//hr = pCon.CreateInstance(__uuidof(ADO::Connection));
		hr = m_pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
		m_pCatalog->PutActiveConnection(_variant_t((IDispatch *)pCon));
		for(int i=0;i<m_pCatalog->Tables->GetCount();i++)
		{
			_variant_t vIndex = (short) i;
			m_pTable = m_pCatalog->Tables->GetItem(vIndex);
			CString strTableName=(LPCSTR)(m_pTable->Name);
			if(strTableName==strOldName)//将名称为“m_strOldName”的表名重命名为“m_strNewName”
			{
				m_pTable->Name=(LPCSTR)strNewName;
				break;
			}
		}
	}
	catch(_com_error &e) 
	{
		// Notify the user of errors if any.
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		CString str;
		str.Format("\nError\n\tSource :  %s \n\tdescription : %s \n", (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
		//printf("\nError\n\tSource :  %s \n\tdescription : %s \n", (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
	}
	catch(...) 
	{
		bRet=FALSE;
	}
	return bRet;
}
BOOL  CDBOperator::CheckRecordExisted(_ConnectionPtr &pCon,CString strTable,CString strKeyField,CString strValue)
{
	BOOL ret=FALSE;
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from %s where %s='%s'",strTable,strKeyField,strValue);
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	}	
    long recCount = pRecord->GetRecordCount();
	if(recCount>0)
	{
		ret = TRUE;
	}
	pRecord->Close();
   
   return ret;
}
BOOL  CDBOperator::DelTable(_ConnectionPtr &pCon,CString strTable)
{
	BOOL ret=TRUE;
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    //strSQL.Format("DROP TABLE IF EXISTS `%s`;",strTable);
	strSQL.Format("DROP TABLE `%s`;",strTable);
	_variant_t RecordsAffected;
    try
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	}	
   return ret;
}
BOOL  CDBOperator::CreateBlockListTableInMySql(_ConnectionPtr &pCon,CString strModelName,CString strModuleName)
{
	BOOL bRet = TRUE;
	CString strTableName;
	strTableName.Format("%s_%s_blocklist",strModelName,strModuleName);
    CString strSQL;
	strSQL.Format("CREATE TABLE `%s` (`ID` bigint(10) NOT NULL AUTO_INCREMENT,`name` varchar(255) NOT NULL DEFAULT '',`desp` varchar(255) NOT NULL,`type` varchar(255) NOT NULL,`orderid` int(10) NOT NULL,`left` double(100,0) NOT NULL,`top` double(100,0) NOT NULL,`right` double(100,0) NOT NULL,`bottom` double(100,0) NOT NULL,`InNum` int(10) NOT NULL,`OutNum` int(255) NOT NULL,`InNames` TEXT DEFAULT NULL,`InValueTypes` TEXT DEFAULT NULL,`OutNames` TEXT DEFAULT NULL, `OutValueTypes` varchar(2048) DEFAULT NULL, `InputLinkStrs` TEXT DEFAULT NULL,PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk",strTableName);
	try
	{
		pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	    bRet =  FALSE;
	}
	return bRet;
}
BOOL CDBOperator::CreateBlockPropTableInMySql(_ConnectionPtr &pCon,CString strModelName,CString strModuleName)
{
	BOOL bRet = TRUE;
	CString strTableName;
	strTableName.Format("%s_%s_blockprop",strModelName,strModuleName);
    CString strSQL;
	strSQL.Format("CREATE TABLE `%s` (`ID` bigint(10) NOT NULL AUTO_INCREMENT,`block_name` varchar(255) NOT NULL,  `prop_name` varchar(255) NOT NULL,  `value_item1` varchar(255) DEFAULT NULL,  `value_item2` varchar(255) DEFAULT NULL,  `value_item3` varchar(255) DEFAULT NULL,  `value_item4` varchar(255) DEFAULT NULL,  `value_item5` varchar(255) DEFAULT NULL,PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk;",strTableName);
	try
	{
		pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	    bRet =  FALSE;
	}
	return bRet;
}
BOOL CDBOperator::CreateModuleListTableInMySql(_ConnectionPtr &pCon,CString strModelName)
{
	BOOL bRet = TRUE;
	CString strTableName;
	strTableName.Format("%s_ModuleList",strModelName);
    CString strSQL;
	strSQL.Format("CREATE TABLE `%s` (`ID` bigint(10) NOT NULL AUTO_INCREMENT, `module_name` varchar(255) NOT NULL,  `module_desp` varchar(255) NOT NULL,  `calcindex` int(10) DEFAULT NULL,  `GraphCx` int(10) DEFAULT NULL,  `GraphCy` int(10) DEFAULT NULL,PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk;",strTableName);
	try
	{
		pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	    bRet =  FALSE;
	}
	return bRet;

}