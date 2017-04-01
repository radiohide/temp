// ModuleData.cpp: implementation of the ModuleData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ModuleData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModuleData::ModuleData()
{
	module_name="";
	module_desp="";
	calcindex=0;
	GraphCx=0;
	GraphCy=0;
}

ModuleData::~ModuleData()
{
	BlockListData *pBlockListData;
	for(int i=0;i<BlockListDataArr.GetSize();i++)
	{
		pBlockListData=BlockListDataArr[i];
		if(pBlockListData!=NULL)
		{
			delete pBlockListData;
			pBlockListData=NULL;
		}
	}
	BlockListDataArr.RemoveAll();
	BlockPropData *pBlockPropData;
	for(i=0;i<BlockPropDataArr.GetSize();i++)
	{
		pBlockPropData=BlockPropDataArr[i];
		if(pBlockPropData!=NULL)
		{
			delete pBlockPropData;
			pBlockPropData=NULL;
		}
	}
	BlockPropDataArr.RemoveAll();
}
void ModuleData::ReadDate(_ConnectionPtr &pCon,CString strModelName)
{
	readBlockListData(pCon,strModelName);
	readBlockPropData(pCon,strModelName);
}
void ModuleData::readBlockListData(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable;
	strTable.Format("%s_%s_BlockList",strModelName,this->module_name);
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
	BlockListData *pBlockListData;
	while(!pRecord->adoEOF)
	{
		CString name;
		CString desp;
		CString type;
		int orderid;
		double left;
		double top;
		double right;
		double bottom;
		int InNum;
		int OutNum;
		CString InNames;
		CString InValueTypes;
		CString OutNames;
		CString OutValueTypes;
		CString InputLinkStrs;

		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("name");
			if(Tempvalue.vt != VT_NULL) name = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("desp");
			if(Tempvalue.vt != VT_NULL) desp = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("type");
			if(Tempvalue.vt != VT_NULL) type = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("orderid");
			if(Tempvalue.vt != VT_NULL) orderid = atoi((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("left");
			if(Tempvalue.vt != VT_NULL) left = atof((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("top");
			if(Tempvalue.vt != VT_NULL) top = atof((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("right");
			if(Tempvalue.vt != VT_NULL) right = atof((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("bottom");
			if(Tempvalue.vt != VT_NULL) bottom = atof((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("InNum");
			if(Tempvalue.vt != VT_NULL) InNum = atoi((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("OutNum");
			if(Tempvalue.vt != VT_NULL) OutNum = atoi((LPCSTR)_bstr_t(Tempvalue));
			Tempvalue = pRecord->GetCollect("InNames");
			if(Tempvalue.vt != VT_NULL) InNames = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("InValueTypes");
			if(Tempvalue.vt != VT_NULL) InValueTypes = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("OutNames");
			if(Tempvalue.vt != VT_NULL) OutNames = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("OutValueTypes");
			if(Tempvalue.vt != VT_NULL) OutValueTypes = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("InputLinkStrs");
			if(Tempvalue.vt != VT_NULL) InputLinkStrs = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			pBlockListData=new BlockListData;
			pBlockListData->name=name;
			pBlockListData->desp=desp;
			pBlockListData->type=type;
			pBlockListData->orderid=orderid;
			pBlockListData->left=left;
			pBlockListData->top=top;
			pBlockListData->right=right;
			pBlockListData->bottom=bottom;
			pBlockListData->InNum=InNum;
			pBlockListData->OutNum=OutNum;
			pBlockListData->InNames=InNames;
			pBlockListData->InValueTypes=InValueTypes;
			pBlockListData->OutNames=OutNames;
			pBlockListData->OutValueTypes=OutValueTypes;
			pBlockListData->InputLinkStrs=InputLinkStrs;
			BlockListDataArr.Add(pBlockListData);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
void ModuleData::readBlockPropData(_ConnectionPtr &pCon,CString strModelName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable;
	strTable.Format("%s_%s_BlockProp",strModelName,this->module_name);
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
	BlockPropData *pBlockPropData;
	while(!pRecord->adoEOF)
	{
		CString block_name;
		CString prop_name;
		CString value_item1;
		CString value_item2;
		CString value_item3;
		CString value_item4;
		CString value_item5;
		
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("block_name");
			if(Tempvalue.vt != VT_NULL) block_name = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("prop_name");
			if(Tempvalue.vt != VT_NULL) prop_name = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("value_item1");
			if(Tempvalue.vt != VT_NULL) value_item1 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("value_item2");
			if(Tempvalue.vt != VT_NULL) value_item2 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("value_item3");
			if(Tempvalue.vt != VT_NULL) value_item3 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("value_item4");
			if(Tempvalue.vt != VT_NULL) value_item4 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("value_item5");
			if(Tempvalue.vt != VT_NULL) value_item5 = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			pBlockPropData=new BlockPropData;
			pBlockPropData->block_name=block_name;
			pBlockPropData->prop_name=prop_name;
			pBlockPropData->value_item1=value_item1;
			pBlockPropData->value_item2=value_item2;
			pBlockPropData->value_item3=value_item3;
			pBlockPropData->value_item4=value_item4;
			pBlockPropData->value_item5=value_item5;
			BlockPropDataArr.Add(pBlockPropData);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
void ModuleData::WriteDate(_ConnectionPtr &pCon,CString strModelName)
{
	/*
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	CString strTable;
	//////////////////////////////////////////////////////
	//检索到所有类型
    try	
	{
		strTable.Format("%s_ModuleList",strModelName);
		strSQL.Format("INSERT INTO %s(module_name,module_desp,calcindex,GraphCx,GraphCy)values(\'%s\',\'%s\',\'%d\',\'%d\',\'%d\')",
			strTable,module_name,module_desp,calcindex,GraphCx,GraphCy);
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);

		strTable.Format("%s_%s_BlockProp",strModelName,this->module_name);
		BlockPropData *pBlockPropData;
		for(int i=0;i<BlockPropDataArr.GetSize();i++)
		{
			pBlockPropData=BlockPropDataArr[i];
			strSQL.Format("INSERT INTO %s(block_name,prop_name,value_item1,value_item2,value_item3,value_item4,value_item5)values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",
				strTable,pBlockPropData->block_name,pBlockPropData->prop_name,
				pBlockPropData->value_item1,pBlockPropData->value_item2,
				pBlockPropData->value_item3,pBlockPropData->value_item4,
				pBlockPropData->value_item5);
			pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
		strTable.Format("%s_%s_BlockList",strModelName,this->module_name);
		BlockListData *pBlockListData;
		for(i=0;i<BlockListDataArr.GetSize();i++)
		{
			pBlockListData=BlockListDataArr[i];
			strSQL.Format("INSERT INTO %s(name, desp, type, orderid, [left], [top], [right], [bottom], InNum, OutNum, InNames, InValueTypes, OutNames, OutValueTypes, InputLinkStrs)values(\'%s\',\'%s\',\'%s\',\'%d\',\'%f\',\'%f\',\'%f\',\'%f\',\'%d\',\'%d\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",
				strTable,pBlockListData->name,pBlockListData->desp,pBlockListData->type,pBlockListData->orderid,
				pBlockListData->left,pBlockListData->top,pBlockListData->right,
				pBlockListData->bottom,pBlockListData->InNum,pBlockListData->OutNum,
				pBlockListData->InNames,pBlockListData->InValueTypes,
				pBlockListData->OutNames,pBlockListData->OutValueTypes,pBlockListData->InputLinkStrs);
			pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
    }
   	catch(_com_error e)
	{
		AfxMessageBox(strSQL);
		AfxMessageBox(e.Description());
		return ;
	}
	
*/

	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
	CString strTable;
	strTable.Format("%s_ModuleList",strModelName);
    strSQL.Format("select * from %s",strTable); //写信息到模块列表数据表
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		//pRecord->Close() ;
	}	
    //不断写入
	if(!pRecord->adoEOF) pRecord->MoveLast();
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("module_name"),_variant_t(module_name) ); //放Name
	pRecord->PutCollect(_T("module_desp"),_variant_t(module_desp) ); //放DESP
	pRecord->PutCollect(_T("calcindex"),_variant_t((short)calcindex) ); //放
	pRecord->PutCollect(_T("GraphCx"),_variant_t((short)GraphCx)); //放
	pRecord->PutCollect(_T("GraphCy"),_variant_t((short)GraphCy)); //放
	pRecord->Update();
	pRecord->Close();

	strTable.Format("%s_%s_BlockProp",strModelName,this->module_name);
	strSQL.Format("select * from %s",strTable); //写信息到模块列表数据表
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	BlockPropData *pBlockPropData;
	for(int i=0;i<BlockPropDataArr.GetSize();i++)
	{
		pBlockPropData=BlockPropDataArr[i];
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(pBlockPropData->block_name) ); //放Name
		pRecord->PutCollect(_T("prop_name"),_variant_t(pBlockPropData->prop_name) ); //放DESP
		pRecord->PutCollect(_T("value_item1"),_variant_t(pBlockPropData->value_item1) ); //放
		pRecord->PutCollect(_T("value_item2"),_variant_t(pBlockPropData->value_item2) ); //放
		pRecord->PutCollect(_T("value_item3"),_variant_t(pBlockPropData->value_item3) ); //放
		pRecord->PutCollect(_T("value_item4"),_variant_t(pBlockPropData->value_item4) ); //放
		pRecord->PutCollect(_T("value_item5"),_variant_t(pBlockPropData->value_item5) ); //放
		pRecord->Update();
	}
	pRecord->Close();
	
	strTable.Format("%s_%s_BlockList",strModelName,this->module_name);
	strSQL.Format("select * from %s",strTable); //写信息到模块列表数据表
	try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	BlockListData *pBlockListData;
	for(i=0;i<BlockListDataArr.GetSize();i++)
	{
		pBlockListData=BlockListDataArr[i];
		pRecord->AddNew();
		pRecord->PutCollect(_T("name"),_variant_t(pBlockListData->name) ); //放Name
		pRecord->PutCollect(_T("desp"),_variant_t(pBlockListData->desp) ); //放DESP
		pRecord->PutCollect(_T("type"),_variant_t(pBlockListData->type) ); //放Name
		pRecord->PutCollect(_T("orderid"),_variant_t((short)pBlockListData->orderid) ); //放DESP
		pRecord->PutCollect(_T("left"),_variant_t((double)pBlockListData->left) ); //放Name
		pRecord->PutCollect(_T("top"),_variant_t((double)pBlockListData->top) ); //放DESP
		pRecord->PutCollect(_T("right"),_variant_t((double)pBlockListData->right) ); //放Name
		pRecord->PutCollect(_T("bottom"),_variant_t((double)pBlockListData->bottom) ); //放DESP
		pRecord->PutCollect(_T("InNum"),_variant_t((short)pBlockListData->InNum) ); //放Name
		pRecord->PutCollect(_T("OutNum"),_variant_t((short)pBlockListData->OutNum) ); //放DESP
		pRecord->PutCollect(_T("InNames"),_variant_t(pBlockListData->InNames) ); //放Name
		pRecord->PutCollect(_T("InValueTypes"),_variant_t(pBlockListData->InValueTypes) ); //放DESP
		pRecord->PutCollect(_T("OutNames"),_variant_t(pBlockListData->OutNames) ); //放DESP
		pRecord->PutCollect(_T("OutValueTypes"),_variant_t(pBlockListData->OutValueTypes) ); //放Name
		pRecord->PutCollect(_T("InputLinkStrs"),_variant_t(pBlockListData->InputLinkStrs) ); //放DESP
		pRecord->Update();
	}
	pRecord->Close();
}

