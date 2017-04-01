// RuleRecomResult.cpp: implementation of the RuleRecomResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRecomResult.h"
#include "SubGroupInfTag.h"
#include "RuleFB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleRecomResult::RuleRecomResult()
{
	pBTargetMarkArr=NULL;
	RuleSelIndex=0;
}

RuleRecomResult::~RuleRecomResult()
{
	delEValueArr();
}
void RuleRecomResult::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void RuleRecomResult::SetConDataTag(ConDataTag *pConDataTag)
{
	this->pConDataTag=pConDataTag;
}
void RuleRecomResult::SetRuleFB(RuleFB *pRuleFB)
{
	this->pRuleFB=pRuleFB;
}
void RuleRecomResult::SetListID(int listID)
{
	this->ListID=listID;
}
void RuleRecomResult::SetBTargetMarkArr(CStringArray *pBTargetMarkArr)
{
	this->pBTargetMarkArr=pBTargetMarkArr;
}
void RuleRecomResult::ReadRecomRule()//读取历史规则
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableNameFB,tableNameRecom;
	int unitIndex=m_iUnit;
	int sets=m_iSet;
	HRESULT hr;
	try
	{
		hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		tableNameFB.Format("t_u%ds%d_f2b_rule",unitIndex,sets);
		tableNameRecom.Format("t_u%ds%d_rule_recommended",unitIndex,sets);

		strSql.Format("SELECT * FROM %s where ID=ANY(SELECT Rule_ID FROM %s WHERE Rule_List_ID = %d)And Rule_Num =%d And condition_code='%s'",
			tableNameFB,tableNameRecom,this->ListID,pRuleFB->RuleID,this->pConDataTag->condition_code);
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
	}
	while(!m_pRec->adoEOF)
	{
		FMarkArr.Add((char *)_bstr_t(m_pRec->GetCollect("FrontMode_Code")));
		BMarkArr.Add((char *)_bstr_t(m_pRec->GetCollect("BehindMode_Code")));
		WeightArr.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("Weight"))));
		m_pRec->MoveNext();
	}
}
void RuleRecomResult::ReadRuleByBTargetMarkArr()
{
	FMarkArr.RemoveAll();
	BMarkArr.RemoveAll();
	WeightArr.RemoveAll();

	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableNameFB;
	int unitIndex=m_iUnit;
	int sets=m_iSet;
	HRESULT hr;
	CString strSql;
	try
	{
		hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		tableNameFB.Format("t_u%ds%d_f2b_rule",unitIndex,sets);

		strSql.Format("SELECT * FROM %s where Rule_Num =%d And condition_code='%s'",
			tableNameFB,pRuleFB->RuleID,this->pConDataTag->condition_code);
		if((*pBTargetMarkArr).GetSize()>0)
		{
			strSql+=" AND (";
			for(int i=0;i<(*pBTargetMarkArr).GetSize();i++)
			{
				CString str;
				if(i+1<(*pBTargetMarkArr).GetSize())
				{
					str.Format(" BehindMode_Code='%s' OR ",(*pBTargetMarkArr)[i]);
				}
				else
					str.Format(" BehindMode_Code='%s'",(*pBTargetMarkArr)[i]);
				strSql+=str;
			}	
			strSql+=" )";
			m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		}
		//AfxMessageBox(strSql);
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
	}
	if((*pBTargetMarkArr).GetSize()>0)
	{
		while(!m_pRec->adoEOF)
		{
			FMarkArr.Add((char *)_bstr_t(m_pRec->GetCollect("FrontMode_Code")));
			BMarkArr.Add((char *)_bstr_t(m_pRec->GetCollect("BehindMode_Code")));
			WeightArr.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("Weight"))));
			IDArr.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("ID"))));
			m_pRec->MoveNext();
		}
	}
}
void RuleRecomResult::CopyRuleByRIndex(RuleRecomResult *pRRResult,int ruleIndex)
{
	RuleSelIndex=ruleIndex;
	this->pConDataTag=pRRResult->pConDataTag;
	this->pRuleFB=pRRResult->pRuleFB;
	FMark=pRRResult->FMarkArr[ruleIndex-1];
	BMark=pRRResult->BMarkArr[ruleIndex-1];
	Weight=pRRResult->WeightArr[ruleIndex-1];
}

void RuleRecomResult::ReadHistEValueArr()
{
	delEValueArr();
	buildEValueArr();
}
void RuleRecomResult::buildEValueArr()
{
	/*
	SubGroupInfTag *pSubGInfTag;
	RuleBGParaEValue *pGBEValue;
	for(int i=0;i<this->pRuleFB->FSGInfTagArr.GetSize();i++)
	{
		pSubGInfTag=this->pRuleFB->FSGInfTagArr[i];
		pGBEValue=new RuleBGParaEValue;
		pGBEValue->pSGInfTag=pSubGInfTag;
		pGBEValue->SubGMark=getSubGMarkByCGMark(pSubGInfTag,FMark,pRuleFB->FID);
		pGBEValue->ReadHistEValue();
		FEValueArr.Add(pGBEValue);
	}
	for(i=0;i<this->pRuleFB->BSGInfTagArr.GetSize();i++)
	{
		pSubGInfTag=this->pRuleFB->BSGInfTagArr[i];
		pGBEValue=new RuleBGParaEValue;
		pGBEValue->pSGInfTag=pSubGInfTag;
		pGBEValue->SubGMark=getSubGMarkByCGMark(pSubGInfTag,BMark,pRuleFB->BID);
		pGBEValue->ReadHistEValue();
		BEValueArr.Add(pGBEValue);
	}
	*/
}
void RuleRecomResult::delEValueArr()
{
	/*
	RuleBGParaEValue *pGBEValue;
	for(int i=0;i<FEValueArr.GetSize();i++)
	{
		pGBEValue=FEValueArr[i];
		if(pGBEValue!=NULL)
		{
			delete pGBEValue;
			pGBEValue=NULL;
		}
	}
	FEValueArr.RemoveAll();
	for(i=0;i<BEValueArr.GetSize();i++)
	{
		pGBEValue=BEValueArr[i];
		if(pGBEValue!=NULL)
		{
			delete pGBEValue;
			pGBEValue=NULL;
		}
	}
	BEValueArr.RemoveAll();
	*/
}
CString RuleRecomResult::GetSubGMarkBySGID(int comGID,int subGID)
{	
	if((pRuleFB->BID!=comGID)&&(pRuleFB->FID!=comGID))
		return "";//不在该结果内
	//.............找权重最大的复合组分类。用该种方式查找会有问题，
	//当都选择局部权重最大时候，有可能不是一条链，也就是背后不一定是一个稳态了
	CString cGmark="";
	int maxWeightIndex=-1;
	int iTemp=0;//0是最小的权重了
	for(int i=0;i<WeightArr.GetSize();i++)
	{
		if(WeightArr[i]>iTemp)
		{
			maxWeightIndex=i;
			iTemp=WeightArr[i];
		}
	}
	if(maxWeightIndex==-1)//查找最大权重出错返回
		return "";
	if(pRuleFB->BID==comGID)
	{
		cGmark=BMarkArr[maxWeightIndex];
	}
	else if(pRuleFB->FID==comGID)
	{
		cGmark=FMarkArr[maxWeightIndex];
	}
	//.............找权重最大的复合组分类。得到cGmark
	return getSubGMarkByCGMark(subGID,cGmark,comGID);
}
CString RuleRecomResult::getSubGMarkByCGMark(int subGID,CString cGmark,int cGID)
{
	CString subGMark="";
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	int unitIndex=m_iUnit;
	int sets=m_iSet;
	tableName.Format("t_u%ds%d_combgroup%d_class_hist",unitIndex,sets,cGID);

	CString strSql;
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		//CString strSql("select * from ");
		//strSql+=tableName;
		
		strSql.Format("SELECT * FROM `%s` Where class_mark='%s'",tableName,cGmark);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return "";
	}
//	ConDataTag *pConDataTag;
	if(!m_pRec->adoEOF)
	{
		CString fieldTag;
		fieldTag.Format("SubGroup%d_ClassMark",subGID);
		const CString conFieldTag=fieldTag;
		subGMark=(char *)_bstr_t(m_pRec->GetCollect((_variant_t)conFieldTag));
	}
	return subGMark;
}

void RuleRecomResult::SaveToRunRecom()
{
	delRuleRecomBeAdvice();
	for(int i=0;i<this->IDArr.GetSize();i++)
	{
		if(!beHaveTheRule(IDArr[i],this->ListID))//如果不存在则写入，否则不操作。
		{
			writeToTheRuleRecom(IDArr[i],this->ListID);
		}
	}
}

void RuleRecomResult::delRuleRecomBeAdvice()//删除建议移除的规则
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;

	int unitIndex=m_iUnit;
	int sets=m_iSet;
	HRESULT hr;
	try
	{
		hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		tableName.Format("t_u%ds%d_rule_recommended",unitIndex,sets);

		strSql.Format("delete FROM %s where optimum =2",tableName);
	
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		//AfxMessageBox(strSql);
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
	}
}

bool RuleRecomResult::beHaveTheRule(int ruleID, int ruleListID)//判断该规则是否已经在推荐库中存在
{
	bool resule=false;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_rule_recommended",m_iUnit,this->m_iSet);

	CString strSql;
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		//CString strSql("select * from ");
		//strSql+=tableName;
		
		strSql.Format("SELECT * FROM `%s` Where Rule_ID=%d AND Rule_List_ID=%d",
			tableName,ruleID,ruleListID);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		resule= false;
	}
	if(!m_pRec->adoEOF)
	{
		resule= true;
	}
	return resule;
}

void RuleRecomResult::writeToTheRuleRecom(int ruleID, int ruleListID)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_rule_recommended",m_iUnit,m_iSet);

	CString strSql;
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		int optimum=0;
		strSql.Format("INSERT INTO %s(Rule_ID,Rule_List_ID,optimum)values(\'%d\',\'%d\',\'%d\')",
			tableName,ruleID,ruleListID,optimum);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);			
	}
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
	}
}
