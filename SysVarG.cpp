// SysVarG.cpp: implementation of the SysVarG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SysVarG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysVarG::SysVarG()
{

}

SysVarG::~SysVarG()
{

}
CString SysVarG::GetNodeText()
{
	CString str;
	str.Format("G%2s%s",this->ID,this->Name);
	return str;
}
void SysVarG::InitSysVarG()
{
	SysSelfVarArr.RemoveAll();
	CStringArray strNameArr;
	readGVar(strNameArr);
	for(int i=0;i<strNameArr.GetSize();i++)
	{
		CString strID=strNameArr[i];
		SysVar *pSysVar=getSysVarByStrName(strID);
		if(pSysVar!=NULL)
			SysSelfVarArr.Add(pSysVar);
	}
}
SysVar* SysVarG::getSysVarByStrName(CString strName)
{
	SysVar *pSysVar=NULL;
	for(int i=0;i<(*pSysVarArr).GetSize();i++)
	{
		pSysVar=(*pSysVarArr)[i];
		if(pSysVar->Name==strName)
			break;
	}
	return pSysVar;
}
void SysVarG::SetVarArr(CArray<SysVar*,SysVar*&> *pSysVarArr)
{
	this->pSysVarArr=pSysVarArr;
}
void SysVarG::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void SysVarG::readGVar(CStringArray &strNameArr)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_varsgroup",m_iUnit,m_iSet);

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` where group_id=%s",tableName,ID);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	while(!m_pRec->adoEOF)
	{
		CString varName=(char *)_bstr_t(m_pRec->GetCollect("var_name"));
		strNameArr.Add(varName);
		m_pRec->MoveNext();
	}
}