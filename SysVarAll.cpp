// SysVarAll.cpp: implementation of the SysVarAll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SysVarAll.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysVarAll::SysVarAll()
{
	pCurrentSelSysVar=NULL;
}

SysVarAll::~SysVarAll()
{
	delSysVarCGArr();
}
void SysVarAll::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
SysVar* SysVarAll::GetSysVarByVarName(CString strVarName)
{
	SysVar *pSysVar=NULL;
	for(int i=0;i<SysVarArr.GetSize();i++)
	{
		pSysVar=SysVarArr[i];
		if(pSysVar->Name==strVarName)
		{
			break;
		}
	}
	return pSysVar;
}
void SysVarAll::GetSelObj(CString strNodeText,int &type,SysVarCG *&pRSysVarCG,SysVarG *&pRSysVarG)
{
	type=-1;//未找到对应的对象
	if(strNodeText=="所有子组")
	{
		type=0;
		return;
	}

	SysVarCG *pSysVarCG;
	for(int i=0;i<SysVarCGArr.GetSize();i++)
	{
		pSysVarCG=SysVarCGArr[i];
		if(pSysVarCG->GetNodeText()==strNodeText)
		{
			pRSysVarCG=pSysVarCG;
			type=1;//复合组
			return;
		}
	}
	
	SysVarG *pSysVarG;
	for(i=0;i<SysVarGArr.GetSize();i++)
	{
		pSysVarG=SysVarGArr[i];
		if(pSysVarG->GetNodeText()==strNodeText)
		{
			pRSysVarG=pSysVarG;
			type=2;//子组
			return;
		}
	}
}
void SysVarAll::InitSysVarAll()
{
	delSysVarCGArr();
	delSysVarGArr();
	delSysVarArr();

	readSysVarCG();
	readSysVarG();
	readSysVar();

	SysVarCG *pSysVarCG;
	for(int i=0;i<SysVarCGArr.GetSize();i++)
	{
		pSysVarCG=SysVarCGArr[i];
		pSysVarCG->SetGArr(&SysVarGArr);
		pSysVarCG->InitSysVarCG();
	}

	SysVarG *pSysVarG;
	for(i=0;i<SysVarGArr.GetSize();i++)
	{
		pSysVarG=SysVarGArr[i];
		pSysVarG->SetVarArr(&SysVarArr);
		pSysVarG->InitSysVarG();
	}
}
void SysVarAll::readSysVarCG()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_comgroup",m_iUnit,m_iSet);//ee
	//tableName.Format("%s%d%s","t_unit",unitIndex+1,"_varscomgroup");

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		//CString strSql("select * from ");
		//strSql+=tableName;
		CString strSql;
		strSql.Format("SELECT * FROM `%s` t",tableName);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	SysVarCG *pSysVarCG;
	while(!m_pRec->adoEOF)
	{
		pSysVarCG=new SysVarCG;
		pSysVarCG->ID=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_id"));
		pSysVarCG->Name=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_name"));
		pSysVarCG->Desp=(char *)_bstr_t(m_pRec->GetCollect("ComGroup_desp"));
		pSysVarCG->SetRDBInf(m_pModel,m_strDBName,m_iUnit,m_iSet);
		SysVarCGArr.Add(pSysVarCG);
		m_pRec->MoveNext();
	}
}
void SysVarAll::readSysVarG()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_group",m_iUnit,m_iSet);

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` t",tableName);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	SysVarG *pSysVarG;
	while(!m_pRec->adoEOF)
	{
		pSysVarG=new SysVarG;
		pSysVarG->ID=(char *)_bstr_t(m_pRec->GetCollect("id"));
		pSysVarG->Name=(char *)_bstr_t(m_pRec->GetCollect("name"));
		pSysVarG->Desp=(char *)_bstr_t(m_pRec->GetCollect("desp"));
		pSysVarG->Type=(char *)_bstr_t(m_pRec->GetCollect("type"));
		pSysVarG->SetRDBInf(m_pModel,m_strDBName,m_iUnit,m_iSet);
		SysVarGArr.Add(pSysVarG);
		m_pRec->MoveNext();
	}
}
void SysVarAll::readSysVar()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_sysvars",m_iUnit,m_iSet);

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` t",tableName);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	SysVar *pSysVar;
	while(!m_pRec->adoEOF)
	{
		pSysVar=new SysVar;
		pSysVar->ID=(char *)_bstr_t(m_pRec->GetCollect("var_id"));
		pSysVar->Name=(char *)_bstr_t(m_pRec->GetCollect("var_name"));
		pSysVar->Desp=(char *)_bstr_t(m_pRec->GetCollect("var_desp"));
		pSysVar->unit=(char *)_bstr_t(m_pRec->GetCollect("unit"));
		pSysVar->var_expression=(char *)_bstr_t(m_pRec->GetCollect("var_expression"));
		pSysVar->type=atoi((char *)_bstr_t(m_pRec->GetCollect("type")));
		pSysVar->category_endpoint=atof((char *)_bstr_t(m_pRec->GetCollect("category_endpoint")));
		pSysVar->fixed_interval=atof((char *)_bstr_t(m_pRec->GetCollect("fixed_interval")));
		pSysVar->relative_interval=atof((char *)_bstr_t(m_pRec->GetCollect("relative_interval")));
		pSysVar->cluster_source=(char *)_bstr_t(m_pRec->GetCollect("cluster_source"));
		SysVarArr.Add(pSysVar);
		m_pRec->MoveNext();
	}
}
void SysVarAll::delSysVarCGArr()
{
	SysVarCG *pSysVarCG;
	for(int i=0;i<SysVarCGArr.GetSize();i++)
	{
		pSysVarCG=SysVarCGArr[i];
		if(pSysVarCG!=NULL)
		{
			delete pSysVarCG;
			pSysVarCG=NULL;
		}
	}
	SysVarCGArr.RemoveAll();
}

void SysVarAll::delSysVarGArr()
{
	SysVarG *pSysVarG;
	for(int i=0;i<SysVarGArr.GetSize();i++)
	{
		pSysVarG=SysVarGArr[i];
		if(pSysVarG!=NULL)
		{
			delete pSysVarG;
			pSysVarG=NULL;
		}
	}
	SysVarGArr.RemoveAll();
}
void SysVarAll::delSysVarArr()
{
	SysVar *pSysVar;
	for(int i=0;i<SysVarArr.GetSize();i++)
	{
		pSysVar=SysVarArr[i];
		if(pSysVar!=NULL)
		{
			delete pSysVar;
			pSysVar=NULL;
		}
	}
	SysVarArr.RemoveAll();
}