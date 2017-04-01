// RuleRecommendList.cpp: implementation of the RuleRecommendList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRecommendList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleRecommendList::RuleRecommendList()
{
	histCurrentCon="";
}

RuleRecommendList::~RuleRecommendList()
{
	CArray<RuleRecomFunTag*,RuleRecomFunTag*&> RuleRecomFunArr;//规则推荐方法列表。
	DelCompRRDataArr();
	RuleRecomResult *pRuleRecom;
	for(int i=0;i<RuleRecomReslutArr.GetSize();i++)
	{
		pRuleRecom=RuleRecomReslutArr[i];
		if(pRuleRecom!=NULL)
		{
			delete pRuleRecom;
			pRuleRecom=NULL;
		}
	}
	RuleRecomReslutArr.RemoveAll();

	delConArr();

	RuleRecomFunTag *pRuleRecomFunTag;
	for(i=0;i<RuleRecomFunArr.GetSize();i++)
	{
		pRuleRecomFunTag=RuleRecomFunArr[i];
		if(pRuleRecomFunTag!=NULL)
		{
			delete pRuleRecomFunTag;
			pRuleRecomFunTag=NULL;
		}
	}
	RuleRecomFunArr.RemoveAll();
}
void RuleRecommendList::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
CString RuleRecommendList::GetSubGMarkBySGID(int subGID)
{
	CString subGMar="";
	if(ConArr.GetSize()>0)
	{
		subGMar=ConArr[0]->GetSubGMarkBySGID(subGID);
	}
	return subGMar;
}
void RuleRecommendList::SetRuleFBAllArr(CArray<RuleFB*,RuleFB*&> *pRuleFBAllArr)
{
	this->pRuleFBAllArr=pRuleFBAllArr;
}
void RuleRecommendList::SetConDataArr(CArray<ConDataTag*,ConDataTag*&> *pConDataTagHistArr)
{
	this->pConDataTagHistArr=pConDataTagHistArr;
}
void RuleRecommendList::InitRuleRecomList()
{
	readRuleListVar();
	buildSelfRuleList();

	//readRuleRecomFun();
	//buildConArr();
}

void RuleRecommendList::readRuleListVar()
{
	ruleFBIDArr.RemoveAll();
	ruleVarIDArr.RemoveAll();
	ruleFrontRuleNumArr.RemoveAll();
	
	CArray<int,int> fbIDArrTemp;

	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_modle_rulelistvar",m_iUnit,m_iSet);
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` WHERE List_id=%d",tableName,this->ListID);
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
		fbIDArrTemp.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("FB_id"))));
		ruleVarIDArr.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("ID"))));
		ruleFrontRuleNumArr.Add(::atoi((char *)_bstr_t(m_pRec->GetCollect("Front_RuleNum"))));
		m_pRec->MoveNext();
	}
	//...............进行前后件排序，性能排在最前....................//
	int frontNum=0;//从0开始找，即找到在规则链条最前端的前后件
	for(int i=0;i<fbIDArrTemp.GetSize();i++)
	{
		int index=-1;
		if(getIndexInArr(ruleFrontRuleNumArr,frontNum,index))
		{
			ruleFBIDArr.Add(fbIDArrTemp[index]);
			frontNum=ruleVarIDArr[index];
		}
	}
	//................................................................
}
void RuleRecommendList::readRuleRecomFun()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_rule_recommended_funVar",m_iUnit,m_iSet);
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` WHERE Rule_List_ID=%d",tableName,this->ListID);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	RuleRecomFunTag *pRuleRecomFunTag;
	while(!m_pRec->adoEOF)
	{	
		pRuleRecomFunTag=new RuleRecomFunTag;
		pRuleRecomFunTag->CGroup_ID=::atoi((char *)_bstr_t(m_pRec->GetCollect("CGroup_ID")));
		pRuleRecomFunTag->Expression=(char *)_bstr_t(m_pRec->GetCollect("Expression"));
		pRuleRecomFunTag->Desp=(char *)_bstr_t(m_pRec->GetCollect("Desp"));
		pRuleRecomFunTag->AnalyExp();
		RuleRecomFunArr.Add(pRuleRecomFunTag);
		m_pRec->MoveNext();
	}
}
void RuleRecommendList::SetFunExpData(CArray<int,int> *m_piCGArr,CStringArray *m_pStrFunExpArr,CStringArray *m_pStrFunDepArr)
{
	RuleRecomFunTag *pRuleRecomFunTag;
	for(int i=0;i<(*m_pStrFunExpArr).GetSize();i++)
	{
		pRuleRecomFunTag=new RuleRecomFunTag;
		pRuleRecomFunTag->CGroup_ID=(*m_piCGArr)[i];
		pRuleRecomFunTag->Expression=(*m_pStrFunExpArr)[i];
		pRuleRecomFunTag->Desp=(*m_pStrFunDepArr)[i];
		pRuleRecomFunTag->AnalyExp();
		RuleRecomFunArr.Add(pRuleRecomFunTag);
	}
}
bool RuleRecommendList::getIndexInArr(const CArray<int,int> &intArr,int num,int &index)
{
	bool result=false;
	for(int i=0;i<intArr.GetSize();i++)
	{
		if(intArr[i]==num)
		{
			index=i;
			result=true;
			break;
		}
	}
	return result;
}
void RuleRecommendList::buildSelfRuleList()
{
	RuleFB *pRuleFB;
	for(int i=0;i<this->ruleFBIDArr.GetSize();i++)
	{
		int ruleID=ruleFBIDArr[i];
		for(int j=0;j<(*pRuleFBAllArr).GetSize();j++)
		{
			pRuleFB=(*pRuleFBAllArr)[j];
			if(pRuleFB->RuleID==ruleID)
			{
				this->RuleFBArr.Add(pRuleFB);
			}
		}
	}
}
void RuleRecommendList::buildConArr()
{
	ConDataTag *pConDataTag;
	ConForRuleRecom *pConForRuleRecom;
	for(int i=0;i<(*pConDataTagHistArr).GetSize();i++)
	{
		pConDataTag=(*pConDataTagHistArr)[i];
		pConForRuleRecom=new ConForRuleRecom;
		pConForRuleRecom->SetListID(ListID);
		pConForRuleRecom->SetConDataTag(pConDataTag);
		pConForRuleRecom->SetRuleFBArr(&RuleFBArr);
		pConForRuleRecom->SetRuleRecomFunArr(&RuleRecomFunArr);
		pConForRuleRecom->SetDllPtrArr(pListPoteDll);
		pConForRuleRecom->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
		//pConForRule->SetRResultDataAllArr(this->pRResuleDataAllArr);
		//pConForRuleRecom->InitConForRuleRecom();
		ConArr.Add(pConForRuleRecom);
	}
}
void RuleRecommendList::GetRuleCount(int &ruleAllCount)
{
	ruleAllCount=0;
}
void RuleRecommendList::CopyData(RuleRecommendList *pSource)//拷贝数据，在创建当前选择的历史规则时使用
{
	this->ListID=pSource->ListID;
	this->RuleName=pSource->RuleName;
	this->RuleDesp=pSource->RuleDesp;
	this->pRuleFBAllArr=pSource->pRuleFBAllArr;
	this->pConDataTagHistArr=pSource->pConDataTagHistArr;
}
void RuleRecommendList::SetCurrSelHistCon(CString strCon)
{
	this->histCurrentCon=strCon;
}
void RuleRecommendList::SetDllPtrArr(CPtrArray *pListPoteDll)
{
	this->pListPoteDll=pListPoteDll;
}
void RuleRecommendList::BuildSelHistConData()//创建当前选择的历史工况数据
{
	RuleFB *pRuleFB;
	RuleRecomResult *pRRResult;
	for(int i=0;i<RuleRecomReslutArr.GetSize();i++)
	{
		pRRResult=RuleRecomReslutArr[i];
		if(pRRResult!=NULL)
		{
			delete pRRResult;
			pRRResult=NULL;
		}
	}
	RuleRecomReslutArr.RemoveAll();

	readRuleListVar();
	buildSelfRuleList();
	
	ConDataTag *pConDataTag;
	for(i=0;i<(*pConDataTagHistArr).GetSize();i++)
	{
		pConDataTag=(*pConDataTagHistArr)[i];
		if(pConDataTag->condition_code==histCurrentCon)
		{
			break;
		}
	}
	for(i=0;i<RuleFBArr.GetSize();i++)
	{
		pRuleFB=RuleFBArr[i];
		pRRResult=new RuleRecomResult;
		pRRResult->SetConDataTag(pConDataTag);
		pRRResult->SetRuleFB(pRuleFB);
		pRRResult->SetListID(this->ListID);
		pRRResult->ReadRecomRule();
		this->RuleRecomReslutArr.Add(pRRResult);
	}
}

void RuleRecommendList::DelCompRRDataArr()
{
	RuleRecomResult *pRRResult;
	for(int i=0;i<this->CompRuleReslutArr.GetSize();i++)
	{
		pRRResult=CompRuleReslutArr[i];
		if(pRRResult!=NULL)
		{
			delete pRRResult;
			pRRResult=NULL;
		}
	}
	CompRuleReslutArr.RemoveAll();
}
void RuleRecommendList::AddCompRRDataArr(RuleRecomResult *pRRResult,int ruleIndex)
{ 
	RuleRecomResult *pRRCompResult=new RuleRecomResult;
	pRRCompResult->CopyRuleByRIndex(pRRResult,ruleIndex);
	CompRuleReslutArr.Add(pRRCompResult);
}
void RuleRecommendList::ReadCompHistRREValueArr()
{
	RuleRecomResult *pRRResult;
	for(int i=0;i<CompRuleReslutArr.GetSize();i++)
	{
		pRRResult=CompRuleReslutArr[i];
		pRRResult->ReadHistEValueArr();
	}
}

void RuleRecommendList::RunRecom()
{
	ReInitConArr();
	ConForRuleRecom *pConForRuleRecom;
	for(int i=0;i<ConArr.GetSize();i++)
	{
		pConForRuleRecom=ConArr[i];
		pConForRuleRecom->RunRecom();
	}
}
void RuleRecommendList::SaveRunRecom()
{
	ConForRuleRecom *pConForRuleRecom;
	for(int i=0;i<ConArr.GetSize();i++)
	{
		pConForRuleRecom=ConArr[i];
		pConForRuleRecom->SaveRunRecom();
	}
}
void RuleRecommendList::ReInitConArr()//重新初始化ConArr
{
	delConArr();
	//delRunRecomFunArr();
	//readRuleRecomFun();
	buildConArr();
}

void RuleRecommendList::delConArr()
{
	ConForRuleRecom *pConForRuleRecom;
	for(int i=0;i<ConArr.GetSize();i++)
	{
		pConForRuleRecom=ConArr[i];
		if(pConForRuleRecom!=NULL)
		{
			delete pConForRuleRecom;
			pConForRuleRecom=NULL;
		}
	}
	ConArr.RemoveAll();
}

void RuleRecommendList::delRunRecomFunArr()
{
	RuleRecomFunTag *pRuleRecomFunTag;
	for(int i=0;i<RuleRecomFunArr.GetSize();i++)
	{	
		pRuleRecomFunTag=RuleRecomFunArr[i];
		if(pRuleRecomFunTag!=NULL)
		{
			delete pRuleRecomFunTag;
			pRuleRecomFunTag=NULL;
		}
	}
	RuleRecomFunArr.RemoveAll();
}
