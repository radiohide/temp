// RuleFB.cpp: implementation of the RuleFB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleFB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleFB::RuleFB()
{

}

RuleFB::~RuleFB()
{
	SubGroupInfTag *pSGInfTag;
	for(int i=0;i<this->FSGInfTagArr.GetSize();i++)
	{
		pSGInfTag=FSGInfTagArr[i];
		if(pSGInfTag!=NULL)
		{
			delete pSGInfTag;
			pSGInfTag=NULL;
		}
	}
	FSGInfTagArr.RemoveAll();
	for(i=0;i<this->BSGInfTagArr.GetSize();i++)
	{
		pSGInfTag=BSGInfTagArr[i];
		if(pSGInfTag!=NULL)
		{
			delete pSGInfTag;
			pSGInfTag=NULL;
		}
	}
	BSGInfTagArr.RemoveAll();
}
void RuleFB::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void RuleFB::InitCGAndSGData()
{
	readCGInf();
	readSGInfByCGID(this->FID,1);
	readSGInfByCGID(this->BID,2);
}
CString RuleFB::GetFSubGsDesp()
{
	CString str;
	SubGroupInfTag *pSGInfTag;
	for(int i=0;i<this->FSGInfTagArr.GetSize();i++)
	{
		pSGInfTag=FSGInfTagArr[i];
		if(i+1<FSGInfTagArr.GetSize())
		{
			str+=pSGInfTag->Desp;
			str+=",";
		}
		else
		{
			str+=pSGInfTag->Desp;
		}
	}
	return str;
}
CString RuleFB::GetBSubGsDesp()
{
	CString str;
	SubGroupInfTag *pSGInfTag;
	for(int i=0;i<this->BSGInfTagArr.GetSize();i++)
	{
		pSGInfTag=BSGInfTagArr[i];
		if(i+1<BSGInfTagArr.GetSize())
		{
			str+=pSGInfTag->Desp;
			str+=",";
		}
		else
		{
			str+=pSGInfTag->Desp;
		}
	}
	return str;
}
void RuleFB::readCGInf()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_comgroup",m_iUnit,m_iSet);	
	try   
	{   
		HRESULT hr;
		CString strSql;
		
		_RecordsetPtr m_pRec;
		hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		strSql.Format("SELECT * FROM `%s` where ComGroup_id=%d or ComGroup_id=%d",tableName,this->FID,this->BID);
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);

		while(!m_pRec->adoEOF)
		{
			int cgID=::atoi((char *)_bstr_t(m_pRec->GetCollect("ComGroup_id")));
			if(cgID==this->FID)
			{
				this->FCGName=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_name"));
				this->FCGDesp=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_desp"));
			}
			else if(cgID==this->BID)
			{
				this->BCGName=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_name"));
				this->BCGDesp=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_desp"));
			}
			m_pRec->MoveNext();
		}
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
}

void RuleFB::readSGInfByCGID(int CGID,int FOrBTag)//FOrBTag,1ÎªF£¬2ÎªB
{
	CString tableNameVarComG,tableNameGroup;	
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableNameVarComG.Format("t_u%ds%d_varscomgroup",m_iUnit,m_iSet);
	tableNameGroup.Format("t_u%ds%d_group",m_iUnit,m_iSet);
	try   
	{   
		HRESULT hr;
		CString strSql;
		SubGroupInfTag *pSunInfTag;
		_RecordsetPtr m_pRec;
		hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		strSql.Format("SELECT * FROM %s WHERE id=ANY(SELECT group_id FROM %s WHERE comgroup_id = %d);",
			tableNameGroup,tableNameVarComG,CGID);
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		while(!m_pRec->adoEOF)
		{
			pSunInfTag=new SubGroupInfTag();
			pSunInfTag->ID=(char *)_bstr_t(m_pRec->GetCollect("id"));
			pSunInfTag->Name=(char *)_bstr_t(m_pRec->GetCollect("name"));
			pSunInfTag->Desp=(char *)_bstr_t(m_pRec->GetCollect("desp"));
			pSunInfTag->Type=(char *)_bstr_t(m_pRec->GetCollect("type"));
			if(FOrBTag==1)
			{
				FSGInfTagArr.Add(pSunInfTag);
			}
			else if(FOrBTag==2)
			{
				BSGInfTagArr.Add(pSunInfTag);
			}
			m_pRec->MoveNext();
		}
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
}
