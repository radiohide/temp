// RuleRecommendListArr.cpp: implementation of the RuleRecommendListArr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRecommendListArr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleRecommendListArr::RuleRecommendListArr()
{
	histCurrentListID=-1;
	histCurrentCon="";
	pCurrentRuleRecom=NULL;
}

RuleRecommendListArr::~RuleRecommendListArr()
{
	delConData();

	if(this->pCurrentRuleRecom!=NULL)
	{
		delete pCurrentRuleRecom;
		pCurrentRuleRecom=NULL;
	}
	RuleFB *pRuleFB;
	for(int i=0;i<RuleFBAllArr.GetSize();i++)
	{
		pRuleFB=RuleFBAllArr[i];
		if(pRuleFB!=NULL)
		{
			delete pRuleFB;
			pRuleFB=NULL;
		}
	}
	RuleFBAllArr.RemoveAll();
	
	RuleRecommendList *pRuleRecommendList;
	for(i=0;i<RuleArr.GetSize();i++)
	{
		pRuleRecommendList=RuleArr[i];
		if(pRuleRecommendList!=NULL)
		{
			delete pRuleRecommendList;
			pRuleRecommendList=NULL;
		}
	}
	RuleArr.RemoveAll();

	CPoteDll* PoteDll;
	for(i=0;i<ListPoteDll.GetSize();i++)
	{
		PoteDll=(CPoteDll *)ListPoteDll[i];
		if(PoteDll!=NULL)
		{
			delete PoteDll;
			PoteDll=NULL;
		}
	}
	ListPoteDll.RemoveAll();
	DelRuleRecomFunArrForView();
}
void RuleRecommendListArr::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void RuleRecommendListArr::InitRuleRecommendListArr()//初始化
{
	initDllMapping();
	//readConData();
	readModeRuleList();

	readModeRuleFB();
	buildRuleFB();

	buildRuleList();
}
void RuleRecommendListArr::delConData()
{
	ConDataTag *pConDataTag;
	for(int i=0;i<ConDataTagHistArr.GetSize();i++)
	{
		pConDataTag=ConDataTagHistArr[i];
		if(pConDataTag!=NULL)
		{
			delete pConDataTag;
			pConDataTag=NULL;
		}
	}
	ConDataTagHistArr.RemoveAll();
}
void RuleRecommendListArr::readConData()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_conditionclass_hist",m_iUnit,m_iSet);
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` where condition_code='%s'",tableName,this->m_strConCode);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	ConDataTag *pConDataTag;
	while(!m_pRec->adoEOF)
	{
		pConDataTag=new ConDataTag;
		pConDataTag->condition_code=(char *)_bstr_t(m_pRec->GetCollect("condition_code"));
		pConDataTag->record_numble=::atoi((char *)_bstr_t(m_pRec->GetCollect("record_numble")));
		pConDataTag->load_desp=(char *)_bstr_t(m_pRec->GetCollect("load_desp"));
		pConDataTag->coal_desp=(char *)_bstr_t(m_pRec->GetCollect("coal_desp"));
		pConDataTag->temp_desp=(char *)_bstr_t(m_pRec->GetCollect("temp_desp"));
		pConDataTag->RID_early=::atoi((char *)_bstr_t(m_pRec->GetCollect("ear_RID")));
		pConDataTag->RID_late=::atoi((char *)_bstr_t(m_pRec->GetCollect("late_RID")));
		ConDataTagHistArr.Add(pConDataTag);
		m_pRec->MoveNext();
	}
}
void RuleRecommendListArr::readModeRuleFB()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_modle_rulefb",m_iUnit,m_iSet);
	
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
	RuleFB *pRuleFB;
	while(!m_pRec->adoEOF)
	{
		pRuleFB=new RuleFB;
		pRuleFB->RuleID=::atoi((char *)_bstr_t(m_pRec->GetCollect("ID")));
		pRuleFB->FID=::atoi((char *)_bstr_t(m_pRec->GetCollect("Front_Comgroup_id")));
		pRuleFB->BID=::atoi((char *)_bstr_t(m_pRec->GetCollect("Behind_Comgroup_id")));
		pRuleFB->FBDesp=(char *)_bstr_t(m_pRec->GetCollect("Desp"));
		pRuleFB->SetRDBInf(m_pModel,m_strDBName,m_iUnit,m_iSet);
		this->RuleFBAllArr.Add(pRuleFB);
		m_pRec->MoveNext();
	}
}
void RuleRecommendListArr::SetFunExpData(CArray<int,int> *m_piCGArr,CStringArray *m_pStrFunExpArr,CStringArray *m_pStrFunDepArr)
{
	this->m_piCGArr=m_piCGArr;
	this->m_pStrFunDepArr=m_pStrFunDepArr;
	this->m_pStrFunExpArr=m_pStrFunExpArr;
}
void RuleRecommendListArr::readModeRuleList()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	//tableName.Format("t_u%ds%d_modle_rulelist",1,2);//debug
	tableName.Format("t_u%ds%d_modle_rulelist",m_iUnit,m_iSet);
	
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT * FROM `%s` where RuleID=%d",tableName,this->m_iRuleID);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	RuleRecommendList *pRuleRecom;
	while(!m_pRec->adoEOF)
	{
		pRuleRecom=new RuleRecommendList;
		pRuleRecom->ListID=::atoi((char *)_bstr_t(m_pRec->GetCollect("RuleID")));
		pRuleRecom->RuleName=(char *)_bstr_t(m_pRec->GetCollect("rule_name"));
		pRuleRecom->RuleDesp=(char *)_bstr_t(m_pRec->GetCollect("rule_desp"));
		pRuleRecom->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
		pRuleRecom->SetFunExpData(m_piCGArr,m_pStrFunExpArr,m_pStrFunDepArr);
		this->RuleArr.Add(pRuleRecom);
		m_pRec->MoveNext();
	}
}
void RuleRecommendListArr::buildRuleFB()
{
	RuleFB *pRuleFB;
	for(int i=0;i<this->RuleFBAllArr.GetSize();i++)
	{
		pRuleFB=this->RuleFBAllArr[i];
		pRuleFB->InitCGAndSGData();
	}
}
void RuleRecommendListArr::buildRuleList()
{
	RuleRecommendList *pRuleRecom;
	for(int i=0;i<this->RuleArr.GetSize();i++)
	{
		pRuleRecom=this->RuleArr[i];
		pRuleRecom->SetRuleFBAllArr(&RuleFBAllArr);
		pRuleRecom->SetConDataArr(&ConDataTagHistArr);
		pRuleRecom->SetDllPtrArr(&ListPoteDll);//设置外部函数指针
		//pRuleRecom->SetRResultDataAllArr(&RResuleDataAllArr);
		pRuleRecom->InitRuleRecomList();
	}
}
void RuleRecommendListArr::SetCurrHistRuleListID(int listID)
{
	this->histCurrentListID=listID;
}
void RuleRecommendListArr::SetCurrHistCon(CString conStr)//设置当前选中的历史工况
{
	this->histCurrentCon=conStr;
}
CString RuleRecommendListArr::GetCurrHistCon()
{
	return histCurrentCon;
}
int RuleRecommendListArr::GetCurrHistRuleListID()
{
	return histCurrentListID;
}

void RuleRecommendListArr::BuildCurrHistSelRule()//根据选择信息创建当前选择的历史规则
{
	if((histCurrentListID!=-1)&&(histCurrentCon!=""))
	{
		if(this->pCurrentRuleRecom!=NULL)
		{
			delete pCurrentRuleRecom;
			pCurrentRuleRecom=NULL;
		}
		this->pCurrentRuleRecom=new RuleRecommendList();

		RuleRecommendList *pRuleRecom;
		for(int i=0;i<this->RuleArr.GetSize();i++)
		{
			pRuleRecom=this->RuleArr[i];
			if(pRuleRecom->ListID==histCurrentListID)
			{
				pCurrentRuleRecom->CopyData(pRuleRecom);
				pCurrentRuleRecom->SetCurrSelHistCon(histCurrentCon);
				pCurrentRuleRecom->BuildSelHistConData();
			}
		}
	}
}

bool RuleRecommendListArr::LookUpHistConByConStr(CString strCon,ConDataTag *&pConDataTag)
{
	bool result=false;
	for(int i=0;i<ConDataTagHistArr.GetSize();i++)
	{
		pConDataTag=ConDataTagHistArr[i];
		//AfxMessageBox(pConDataTag->condition_code);
		if(pConDataTag->condition_code==strCon)
		{
			result=true;
			break;
		}
	}
	return result;
}

void RuleRecommendListArr::RunRecom()
{
	ReInitRuleArr();
	RuleRecommendList *pRuleRecom;
	for(int i=0;i<RuleArr.GetSize();i++)
	{
		pRuleRecom=RuleArr[i];
		pRuleRecom->RunRecom();
	}
}
void RuleRecommendListArr::SaveRunRecom()
{
	RuleRecommendList *pRuleRecom;
	for(int i=0;i<RuleArr.GetSize();i++)
	{
		pRuleRecom=RuleArr[i];
		pRuleRecom->SaveRunRecom();
	}
}
void RuleRecommendListArr::initDllMapping()//初始化约束所用外部函数，保存函数指针
{
	CString path= gGetExeFileDir() + "ePerfManage.CFG";

	char inBuf[80];
	int n = 0;
	int dllNum=0;
	CString strKey;
	char* cNodesName=new char[20];
	try
	{
		CPoteDll* PoteDll;
		
		n=GetPrivateProfileString ("DLL", "dll.fileNum", "0", inBuf, 80, path);
		dllNum=::atoi(inBuf);
		
		for (int i = 0; i<dllNum; i++)
		{
			CString str;	str.Format("%d",i+1);
			strKey = "dll.filename"+str;
			strcpy(cNodesName,strKey);
			n=GetPrivateProfileString ("DLL", cNodesName, NULL, inBuf, 80, path);
			strKey = CString(inBuf);
			strcpy(cNodesName,CString(inBuf));
			
			PoteDll = new CPoteDll(strKey);
			if( PoteDll->IsDllValidate(cNodesName)==TRUE )
			{
				PoteDll->LoadDll(cNodesName);
				ListPoteDll.Add(PoteDll);
			}
		}
		if (cNodesName) delete[] cNodesName;
		PoteDll =NULL;  
	}
	catch(CException *e)
	{
		e->Delete();
	}
}
void RuleRecommendListArr::SetConCode(CString strConCode)
{
	m_strConCode=strConCode;
}
void RuleRecommendListArr::ReInitRuleArr()//重新初始化RuleArr，其中重新读取工况，读取推荐公式
{
	delConData();
	readConData();
	RuleRecommendList *pRuleRecom;
	for(int i=0;i<RuleArr.GetSize();i++)
	{
		pRuleRecom=RuleArr[i];
		pRuleRecom->ReInitConArr();
	}
}

void RuleRecommendListArr::DelRuleRecomFunArrForView()
{
	RuleRecomFunTag *pRuleRecomFunTag;
	for(int i=0;i<RuleRecomFunTagArrForView.GetSize();i++)
	{
		pRuleRecomFunTag=RuleRecomFunTagArrForView[i];
		if(pRuleRecomFunTag!=NULL)
		{
			delete pRuleRecomFunTag;
			pRuleRecomFunTag=NULL;
		}
	}
	RuleRecomFunTagArrForView.RemoveAll();
}

void RuleRecommendListArr::WriteRuleFunArrForView()//将修改后的规则推荐方法写入到数据库
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
    tableName.Format("t_u%ds%d_rule_recommended_funvar",m_iUnit,m_iSet);
	
	_variant_t RecordsAffected;
	CString strSQL1,strSQL;
	strSQL1.Format("TRUNCATE TABLE `%s`",tableName);//debug
	m_pCon->Execute((_bstr_t)strSQL1,&RecordsAffected,adCmdText); //清空

	RuleRecomFunTag *pRuleRecomFunTag;
	for(int i=0;i<RuleRecomFunTagArrForView.GetSize();i++)
	{
		pRuleRecomFunTag=RuleRecomFunTagArrForView[i];
		strSQL.Format("INSERT INTO %s (Rule_List_ID,CGroup_ID,Expression,Desp)values(\'%d\',\'%d\',\'%s\',\'%s\')",//table
			tableName,pRuleRecomFunTag->ListID,pRuleRecomFunTag->CGroup_ID,pRuleRecomFunTag->Expression,pRuleRecomFunTag->Desp);		
		m_pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //添加记录	
	}
}

void RuleRecommendListArr::SetRuleListID(int ruleID)
{
	this->m_iRuleID=ruleID;
}
