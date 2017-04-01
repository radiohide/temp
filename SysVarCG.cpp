// SysVarCG.cpp: implementation of the SysVarCG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SysVarCG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysVarCG::SysVarCG()
{

}

SysVarCG::~SysVarCG()
{

}
CString SysVarCG::GetNodeText()
{
	CString str;
	str.Format("CG%2s%s",this->ID,this->Name);
	return str;
}
void SysVarCG::InitSysVarCG()
{
	SysVarSelfGArr.RemoveAll();
	CStringArray gIDArr;
	readCGVar(gIDArr);
	for(int i=0;i<gIDArr.GetSize();i++)
	{
		CString strID=gIDArr[i];
		SysVarG *pSysVarG=getSysVarGByStrGID(strID);
		if(pSysVarG!=NULL)
			SysVarSelfGArr.Add(pSysVarG);
	}
}
void SysVarCG::SetGArr(CArray<SysVarG*,SysVarG*&> *pSysVarGArr)
{
	this->pSysVarGArr=pSysVarGArr;
}
void SysVarCG::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void SysVarCG::readCGVar(CStringArray &gIDArr)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_varscomgroup",m_iUnit,m_iSet);

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` where comgroup_id=%s",tableName,ID);
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
		CString strGroupID=(char *)_bstr_t(m_pRec->GetCollect("group_id"));
		gIDArr.Add(strGroupID);
		m_pRec->MoveNext();
	}
}
SysVarG* SysVarCG::getSysVarGByStrGID(CString strGID)
{
	SysVarG *pSysVarG=NULL;
	for(int i=0;i<(*pSysVarGArr).GetSize();i++)
	{
		pSysVarG=(*pSysVarGArr)[i];
		if(pSysVarG->ID==strGID)
		{
			break;
		}
	}
	return pSysVarG;
}
