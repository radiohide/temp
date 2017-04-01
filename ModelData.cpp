// ModelData.cpp: implementation of the ModelData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ModelData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "EPerfManageDoc.h"
ModelData::ModelData()
{

}

ModelData::~ModelData()
{
	ModuleData *pModuleData;
	for(int i=0;i<ModuleArr.GetSize();i++)
	{
		pModuleData=ModuleArr[i];
		if(pModuleData!=NULL)
		{
			delete pModuleData;
			pModuleData=NULL;
		}
	}
	ModuleArr.RemoveAll();
}
void ModelData::SetIPAndPort(CString strIP,CString strPort)//设置IP和端口
{
	model_IP=strIP;
	model_port=strPort;
}
void ModelData::GetIPAndPort(CString &strIP,CString &strPort)//获得IP和端口信息
{
	strIP=model_IP;
	strPort=model_port;
}
void ModelData::ReadDate(_ConnectionPtr &pCon,CString strModelName) 
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("ModelList");
	strSQL.Format("SELECT * FROM %s WHERE model_name='%s'",strTable,strModelName);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	if(!pRecord->adoEOF)
	{
		CString strID,strName,strDesp,strIP,strPort; 
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("id");
			if(Tempvalue.vt != VT_NULL) strID = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("model_name");
			if(Tempvalue.vt != VT_NULL) strName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("model_desp");
			if(Tempvalue.vt != VT_NULL) strDesp = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("model_IP");
			if(Tempvalue.vt != VT_NULL) strIP = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("model_port");
			if(Tempvalue.vt != VT_NULL) strPort = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			this->id=atoi(strID);
			this->model_desp=strDesp;
			this->model_IP=strIP;
			this->model_port=strPort;
		}
	}
	pRecord->Close();
	readModuleDate(pCon,strModelName);
	readRDBSourceData(pCon,strModelName);
}
void ModelData::readRDBSourceData(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("SourceRDB");
	strSQL.Format("SELECT * FROM %s WHERE ModelName='%s'",strTable,strModelName);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	strSourceNameArr.RemoveAll();
	strRDBTypeArr.RemoveAll();
	strIPArr.RemoveAll();
	strRDBNameArr.RemoveAll();
	strRDBUserArr.RemoveAll();
	strRDBPassArr.RemoveAll();
	strFileNameArr.RemoveAll();
	while(!pRecord->adoEOF)
	{
		CString strSourceName,strRDBType,strIP,strRDBName,strRDBUser,strRDBPass,strFileName;
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("SourceName");
			if(Tempvalue.vt != VT_NULL)	strSourceName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBType");
			if(Tempvalue.vt != VT_NULL) strRDBType = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("IP");
			if(Tempvalue.vt != VT_NULL) strIP = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBName");
			if(Tempvalue.vt != VT_NULL) strRDBName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBUser");
			if(Tempvalue.vt != VT_NULL) strRDBUser = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBPass");
			if(Tempvalue.vt != VT_NULL) strRDBPass = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("FileName");
			if(Tempvalue.vt != VT_NULL) strFileName = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			strSourceNameArr.Add(strSourceName);
			strRDBTypeArr.Add(strRDBType);
			strIPArr.Add(strIP);
			strRDBNameArr.Add(strRDBName);
			strRDBUserArr.Add(strRDBUser);
			strRDBPassArr.Add(strRDBPass);
			strFileNameArr.Add(strFileName);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
void ModelData::readModuleDate(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable;
	strTable.Format("%s_ModuleList",strModelName);
	strSQL.Format("SELECT * FROM %s",strTable);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	ModuleData *pModuleData;
	while(!pRecord->adoEOF)
	{
		CString module_name,module_desp;
		int calcindex=0,GraphCx=0,GraphCy=0; 
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("module_name");
			if(Tempvalue.vt != VT_NULL) module_name = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("module_desp");
			if(Tempvalue.vt != VT_NULL) module_desp = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("calcindex");
			if(Tempvalue.vt != VT_NULL) calcindex = atoi((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("GraphCx");
			if(Tempvalue.vt != VT_NULL) GraphCx = atoi((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("GraphCy");
			if(Tempvalue.vt != VT_NULL) GraphCy = atoi((LPCSTR)_bstr_t(Tempvalue));
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			pModuleData=new ModuleData;
			pModuleData->module_name=module_name;
			pModuleData->module_desp=module_desp;
			pModuleData->calcindex=calcindex;
			pModuleData->GraphCx=GraphCx;
			pModuleData->GraphCy=GraphCy;
			pModuleData->ReadDate(pCon,strModelName);
			ModuleArr.Add(pModuleData);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
BOOL ModelData::DelModelInModelListTable(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("ModelList");
	strSQL.Format("delete from ModelList where model_name='%s'",strModelName);
    try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		//AfxMessageBox("删除模型失败！");
		return FALSE;
	}
	return TRUE;
}
void ModelData::DelTheModelTables(_ConnectionPtr &pCon,CString strModelName)
{
	CString strTableName;
	strTableName.Format("%s_ModuleList",strModelName);
	CDBOperator::DelTable(pCon,strTableName);
	ModuleData *pModuleData;
	for(int i=0;i<ModuleArr.GetSize();i++)
	{
		pModuleData=ModuleArr[i];
		strTableName.Format("%s_%s_BlockList",strModelName,pModuleData->module_name);
		CDBOperator::DelTable(pCon,strTableName);
		strTableName.Format("%s_%s_BlockProp",strModelName,pModuleData->module_name);
		CDBOperator::DelTable(pCon,strTableName);
	}
}
void ModelData::CreatTheModelTablesInMySql(_ConnectionPtr &pCon,CString strModelName)
{
	CDBOperator::CreateModuleListTableInMySql(pCon,strModelName);
	ModuleData *pModuleData;
	for(int i=0;i<ModuleArr.GetSize();i++)
	{
		pModuleData=ModuleArr[i];
		CDBOperator::CreateBlockListTableInMySql(pCon,strModelName,pModuleData->module_name);
		CDBOperator::CreateBlockPropTableInMySql(pCon,strModelName,pModuleData->module_name);
	}
}
void ModelData::CreatTheModelTablesInMDB(_ConnectionPtr &pCon,CString strModelName)
{
	CString strTableName;
	strTableName.Format("%s_ModuleList",strModelName);
	CEPerfManageDoc::CreateModuleListTable(pCon,strModelName);
	ModuleData *pModuleData;
	for(int i=0;i<ModuleArr.GetSize();i++)
	{
		pModuleData=ModuleArr[i];
		strTableName.Format("%s_%s_BlockList",strModelName,pModuleData->module_name);
		CEPerfManageDoc::CreateBlockListTable(pCon,strModelName,pModuleData->module_name);
		strTableName.Format("%s_%s_BlockProp",strModelName,pModuleData->module_name);
		CEPerfManageDoc::CreateBlockPropTable(pCon,strModelName,pModuleData->module_name);
	}
}
void ModelData::writeRDBSourceData(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("SourceRDB");
	strSQL.Format("delete from %s where ModelName='%s'",strTable,strModelName);
	try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		return ;
	}
	for(int i=0;i<this->strSourceNameArr.GetSize();i++)
	{
		CString strSourceName=strSourceNameArr[i];
		CString strRDBType=strRDBTypeArr[i];
		CString strIP=strIPArr[i];
		CString strRDBName=strRDBNameArr[i];
		CString strUserName=strRDBUserArr[i];
		CString strRDBPass=strRDBPassArr[i];
		CString strFileName=strFileNameArr[i];

		strSQL.Format("INSERT INTO %s(ModelName,SourceName,RDBType,IP,RDBName,RDBUser,RDBPass,FileName)values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",
			strTable,strModelName,strSourceName,strRDBType,strIP,strRDBName,strUserName,strRDBPass,strFileName);
		try	
		{
			pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
		catch(_com_error e)
		{
			continue ;
		}
	}
}
void ModelData::WriteData(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("ModelList");
	BOOL bHased = CDBOperator::CheckRecordExisted(pCon,strTable,"model_name",strModelName);
	if(bHased)//更新
	{
		strSQL.Format("UPDATE `%s`SET model_desp='%s',model_IP='%s' ,model_port='%s' WHERE model_name='%s'",
			strTable,this->model_desp,this->model_IP,this->model_port,strModelName);
	}
	else//插入
	{
		strSQL.Format("INSERT INTO %s(model_name,model_desp,model_IP,model_port)values(\'%s\',\'%s\',\'%s\',\'%s\')",
			strTable,strModelName,this->model_desp,this->model_IP,this->model_port);
	}
    try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	writeModuleDate(pCon,strModelName);
	writeRDBSourceData(pCon,strModelName);
}
void ModelData::writeModuleDate(_ConnectionPtr &pCon,CString strModelName)
{
	ModuleData *pModuleData;
	for(int i=0;i<ModuleArr.GetSize();i++)
	{
		pModuleData=ModuleArr[i];
		pModuleData->WriteDate(pCon,strModelName);
	}
}