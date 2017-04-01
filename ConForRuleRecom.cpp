// ConForRuleRecom.cpp: implementation of the ConForRuleRecom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ConForRuleRecom.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConForRuleRecom::ConForRuleRecom()
{

}

ConForRuleRecom::~ConForRuleRecom()
{
	removeTargetValueArr();
	delRuleRemResultArr();
}
void ConForRuleRecom::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
CString ConForRuleRecom::GetSubGMarkBySGID(int subGID)
{
	CString subGMark="";
	//...........找该自组对应的复合组ID
	int comGID=-1;
	RuleFB *pRuleFB;
	bool beFind=false;
	for(int i=0;i<(*pRuleFBArr).GetSize();i++)
	{
		if(beFind)
			break;
		pRuleFB=(*pRuleFBArr)[i];
		SubGroupInfTag *pSubGTag;
		for(int j=0;j<pRuleFB->BSGInfTagArr.GetSize();j++)
		{
			pSubGTag=pRuleFB->BSGInfTagArr[j];
			if(atoi(pSubGTag->ID)==subGID)
			{
				comGID=pRuleFB->BID;
				beFind=true;
			}
		}
		if(beFind)
			break;
		for(j=0;j<pRuleFB->FSGInfTagArr.GetSize();j++)
		{
			pSubGTag=pRuleFB->FSGInfTagArr[j];
			if(atoi(pSubGTag->ID)==subGID)
			{
				comGID=pRuleFB->FID;
				beFind=true;
			}
		}
	}
	//...........找该自组对应的复合组ID
	RuleRecomResult *pRuleRecomResult;
	for(i=0;i<RuleRecomResultArr.GetSize();i++)
	{
		pRuleRecomResult=RuleRecomResultArr[i];
		if(pRuleRecomResult!=NULL)
		{
			subGMark=pRuleRecomResult->GetSubGMarkBySGID(comGID,subGID);
			if(subGMark!="")
			{
				break;
			}
		}
	}
	return subGMark;
}
void ConForRuleRecom::SetListID(int listID)
{
	this->ListID=listID;
}
void ConForRuleRecom::SetConDataTag(ConDataTag *pConDataTag)
{
	this->pConDataTag=pConDataTag;
}
void ConForRuleRecom::SetRuleFBArr(CArray<RuleFB*,RuleFB*&> *pRuleFBArr)
{
	this->pRuleFBArr=pRuleFBArr;
}
void ConForRuleRecom::SetRuleRecomFunArr(CArray<RuleRecomFunTag*,RuleRecomFunTag*&> *pRuleRecomFunArr)
{
	this->pRuleRecomFunArr=pRuleRecomFunArr;
}
void ConForRuleRecom::SetDllPtrArr(CPtrArray *pListPoteDll)
{
	this->pListPoteDll=pListPoteDll;
}
void ConForRuleRecom::RunRecom()
{
	InitTargetValueArr();//读取目标值分类。
	RuleRecomFunTag *pRuleRecomFunTag;
	for(int i=0;i<(*pRuleRecomFunArr).GetSize();i++)//两条公式则推荐两次，依次求交集
	{
		pRuleRecomFunTag=(*pRuleRecomFunArr)[i];
		runOneByRecomFun(pRuleRecomFunTag);
	}
	BuildRule();
}
void ConForRuleRecom::SaveRunRecom()
{
	RuleRecomResult *pRuleRecomResult;
	for(int i=0;i<RuleRecomResultArr.GetSize();i++)
	{
		pRuleRecomResult=RuleRecomResultArr[i];
		pRuleRecomResult->SaveToRunRecom();
	}
}

void ConForRuleRecom::delRuleRemResultArr()
{
	RuleRecomResult *pRuleRecomResult;
	for(int i=0;i<RuleRecomResultArr.GetSize();i++)
	{
		pRuleRecomResult=RuleRecomResultArr[i];
		if(pRuleRecomResult!=NULL)
		{
			delete pRuleRecomResult;
			pRuleRecomResult=NULL;
		}
	}
	RuleRecomResultArr.RemoveAll();
}
void ConForRuleRecom::BuildRule()//构建规则各个前后件的规则
{
	delRuleRemResultArr();
	//.......................得到目标复合组分类CStringArray
	CStringArray targetMarkArr;
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		targetMarkArr.Add(RuleRTargetValueArr[i]->CGMark);
	}
	//.......................
	RuleFB *pRuleFB;
	RuleRecomResult *pRuleRecomResult;
	for(i=0;i<(*pRuleFBArr).GetSize();i++)
	{
		pRuleFB=(*pRuleFBArr)[i];
		pRuleRecomResult=new RuleRecomResult;
		pRuleRecomResult->SetConDataTag(this->pConDataTag);
		pRuleRecomResult->SetRuleFB(pRuleFB);
		pRuleRecomResult->SetListID(this->ListID);
		pRuleRecomResult->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
		if(i==0)
		{
			pRuleRecomResult->SetBTargetMarkArr(&targetMarkArr);
		}
		else 
		{
			pRuleRecomResult->SetBTargetMarkArr(&(RuleRecomResultArr[RuleRecomResultArr.GetSize()-1]->FMarkArr));//取前一个(偏性能)环节的前件做该环节的后件
		}
		pRuleRecomResult->ReadRuleByBTargetMarkArr();
		RuleRecomResultArr.Add(pRuleRecomResult);
	}
}
void ConForRuleRecom::runOneByRecomFun(RuleRecomFunTag *pRuleRecomFunTag)//扩展方法在此扩展
{
	//......计算目标值
	RuleRTargetValue *pRuleRTargetValue;
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		pRuleRTargetValue=RuleRTargetValueArr[i];
		pRuleRTargetValue->Reset();//复位
		pRuleRTargetValue->TargetFunExp=pRuleRecomFunTag->TargetFunExp;//更新计算目标值的计算公式
		pRuleRTargetValue->AnalyExp();//解析公式
		pRuleRTargetValue->ReadEValue();//读取特征值
		pRuleRTargetValue->Calc();//计算目标值
	}
	
	//....计算约束,将不满足约束条件的从RuleRTargetValueArr
	orderRRTargetObj();//从小到大排序
	if(pRuleRecomFunTag->ConstraintsExp=="Min")
	{
		for(int i=(int)pRuleRecomFunTag->ParaItemArr[0];i<RuleRTargetValueArr.GetSize();)//删除大的目标值
		{
			pRuleRTargetValue=RuleRTargetValueArr[i];
			if(pRuleRTargetValue!=NULL)
			{
				delete pRuleRTargetValue;
				pRuleRTargetValue=NULL;
			}
			RuleRTargetValueArr.RemoveAt(i);
		}
	}
	else if(pRuleRecomFunTag->ConstraintsExp=="Max")
	{
		int delCount=(int)(RuleRTargetValueArr.GetSize()-pRuleRecomFunTag->ParaItemArr[0]);
		for(int i=0;i<delCount;i++)//删除小的目标值
		{
			pRuleRTargetValue=RuleRTargetValueArr[0];
			if(pRuleRTargetValue!=NULL)
			{
				delete pRuleRTargetValue;
				pRuleRTargetValue=NULL;
			}
			RuleRTargetValueArr.RemoveAt(0);
		}
	}
	else if(pRuleRecomFunTag->ConstraintsExp=="MaxUp")
	{
		for(int k=0;k<RuleRTargetValueArr.GetSize();k++)
		{
			pRuleRTargetValue=RuleRTargetValueArr[k];
		}
		int count=RuleRTargetValueArr.GetSize();
		for(int i=0;i<count;i++)
		{
			pRuleRTargetValue=RuleRTargetValueArr[0];
			if(pRuleRecomFunTag->ParaItemArr[1]==1)//要取大于等于为结果
			{
				if(pRuleRTargetValue->TargetValue<pRuleRecomFunTag->ParaItemArr[0])
				{
					if(pRuleRTargetValue!=NULL)
					{
						delete pRuleRTargetValue;
						pRuleRTargetValue=NULL;
					}
					RuleRTargetValueArr.RemoveAt(0);
				}
				else
				{
					break;
				}
			}
			else if(pRuleRecomFunTag->ParaItemArr[1]==0)//要与大于为结果
			{
				if(pRuleRTargetValue->TargetValue<=pRuleRecomFunTag->ParaItemArr[0])
				{
					if(pRuleRTargetValue!=NULL)
					{
						delete pRuleRTargetValue;
						pRuleRTargetValue=NULL;
					}
					RuleRTargetValueArr.RemoveAt(0);
				}
				else
					break;
			}
		}
	}
	else if(pRuleRecomFunTag->ConstraintsExp=="MinUp")
	{
		int i=RuleRTargetValueArr.GetSize()-1;
		for(;i>=0;i--)
		{
			i=RuleRTargetValueArr.GetSize()-1;
			pRuleRTargetValue=RuleRTargetValueArr[i];
			if(pRuleRecomFunTag->ParaItemArr[1]==1)//要取小于等于为结果
			{
				if(pRuleRTargetValue->TargetValue>pRuleRecomFunTag->ParaItemArr[0])
				{
					if(pRuleRTargetValue!=NULL)
					{
						delete pRuleRTargetValue;
						pRuleRTargetValue=NULL;
					}
					RuleRTargetValueArr.RemoveAt(i);
				}
				else
				{
					break;
				}
			}
			else if(pRuleRecomFunTag->ParaItemArr[1]==0)//要取小于为结果
			{
				if(pRuleRTargetValue->TargetValue>=pRuleRecomFunTag->ParaItemArr[0])
				{
					if(pRuleRTargetValue!=NULL)
					{
						delete pRuleRTargetValue;
						pRuleRTargetValue=NULL;
					}
					RuleRTargetValueArr.RemoveAt(i);
				}
				else
					break;
			}
		}
	}
	else if(pRuleRecomFunTag->ConstraintsExp=="Closest")
	{
		int closestIndex=-1;
		double colsetValue=pRuleRecomFunTag->ParaItemArr[0];
		if(RuleRTargetValueArr.GetSize()>1)//找到最接近的目标对象在Arr中的索引。
		{
			RuleRTargetValue *pTResult=RuleRTargetValueArr[0];
			closestIndex=0;
			for(int i=1;i<RuleRTargetValueArr.GetSize();i++)
			{
				pRuleRTargetValue=RuleRTargetValueArr[i];
				if(abs((int)((pRuleRTargetValue->TargetValue-colsetValue))<abs((int)(pTResult->TargetValue-colsetValue))))
				{
					pTResult=pRuleRTargetValue;
					closestIndex=i;
				}
			}
		}
		if(closestIndex!=-1)
		{
			double maxCount,minCount;
			if(pRuleRecomFunTag->ParaItemArr[3]==0)//最接近
			{
				maxCount=0;
				minCount=0;
			}
			else if(pRuleRecomFunTag->ParaItemArr[3]==1)//按照个数限制进行
			{
				maxCount=pRuleRecomFunTag->ParaItemArr[1];
				minCount=pRuleRecomFunTag->ParaItemArr[2];
			}
			for(int i=RuleRTargetValueArr.GetSize()-1;i>=closestIndex+1+maxCount;i--)//删除大于该索引的对象
			{
				pRuleRTargetValue=RuleRTargetValueArr[i];
				if(pRuleRTargetValue!=NULL)
				{
					delete pRuleRTargetValue;
					pRuleRTargetValue=NULL;
				}
				RuleRTargetValueArr.RemoveAt(i);
			}
			for(i=0;i<closestIndex-minCount;i++)
			{
				pRuleRTargetValue=RuleRTargetValueArr[0];
				if(pRuleRTargetValue!=NULL)
				{
					delete pRuleRTargetValue;
					pRuleRTargetValue=NULL;
				}
				RuleRTargetValueArr.RemoveAt(0);
			}
		}
	}
}
void ConForRuleRecom::orderRRTargetObj()
{
	CArray<RuleRTargetValue*,RuleRTargetValue*&> RuleRTValueTempArr;//推荐规则链条目标值
	RuleRTargetValue *pRRTValue,*pRRTValueTemp;
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		pRRTValue=RuleRTargetValueArr[i];
		int index=0;
		for(int j=0;j<RuleRTValueTempArr.GetSize();j++)
		{
			pRRTValueTemp=RuleRTValueTempArr[j];
			if(pRRTValue->TargetValue<pRRTValueTemp->TargetValue)
			{
				index=j;
				break;
			}
			index++;
		}
		RuleRTValueTempArr.InsertAt(index,pRRTValue);
	}
	RuleRTargetValueArr.RemoveAll();
	RuleRTargetValueArr.Copy(RuleRTValueTempArr);
	RuleRTValueTempArr.RemoveAll();
	for(int k=0;k<RuleRTargetValueArr.GetSize();k++)
	{
		pRRTValue=RuleRTargetValueArr[k];
	}
	/*
	RuleRTargetValue *pRRTValueF,*pRRTValueB,*pRRTValueTemp
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		pRRTValueF=RuleRTargetValueArr[i];
		for(int j=i+1;j<RuleRTargetValueArr.GetSize();j++)
		{
			pRRTValueB=RuleRTargetValueArr[j];
			if(pRRTValueF->TargetValue>pRRTValueB->TargetValue)
			{
				pRRTValueTemp=pRRTValueF;
				pRRTValueF=pRRTValueB;
				pRRTValueB=pRRTValueTemp;
			}
		}
	}
	*/
}
void ConForRuleRecom::InitTargetValueArr()//初始化目标值序列，读取目标复合组分类
{
	removeTargetValueArr();
	readTargetCGMark();
}

void ConForRuleRecom::removeTargetValueArr()//清空目标值序列
{
	RuleRTargetValue *pRuleRTargetValue;
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		pRuleRTargetValue=RuleRTargetValueArr[i];
		if(pRuleRTargetValue!=NULL)
		{
			delete pRuleRTargetValue;
			pRuleRTargetValue=NULL;
		}
	}
	RuleRTargetValueArr.RemoveAll();
}

void ConForRuleRecom::readTargetCGMark()//读取目标复合组分类
{
	if((*pRuleFBArr).GetSize()<=0)//该链条无前后件则不去读取。
		return;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName,tableNameRecom;
	tableName.Format("t_u%ds%d_f2b_rule",m_iUnit,m_iSet);
	tableNameRecom.Format("t_u%ds%d_rule_recommended",m_iUnit,m_iSet);
	
	try
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		//strSql.Format("SELECT * FROM `%s` WHERE Rule_Num=%d AND condition_code='%s'",
		//	tableName,((*pRuleFBArr)[0])->RuleID,pConDataTag->condition_code);

		strSql.Format("SELECT * FROM %s where ID=ANY(SELECT Rule_ID FROM %s WHERE Rule_List_ID = %d AND(optimum=0 OR optimum=1))And Rule_Num =%d And condition_code='%s'",
			tableName,tableNameRecom,this->ListID,((*pRuleFBArr)[0])->RuleID,this->pConDataTag->condition_code);

        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	RuleRTargetValue *pRuleRTargetValue;
	while(!m_pRec->adoEOF)
	{
		CString strCGMark=(char *)_bstr_t(m_pRec->GetCollect("BehindMode_Code"));
		if(!beInRuleRTValueArrByCGMark(strCGMark))
		{
			pRuleRTargetValue=new RuleRTargetValue;
			pRuleRTargetValue->CGMark=strCGMark;
			pRuleRTargetValue->SetDllPtrArr(pListPoteDll);
			pRuleRTargetValue->SetRuleFBPtr((*pRuleFBArr)[0]);
			pRuleRTargetValue->SetRDBInf(this->m_pModel,this->m_strDBName,this->m_iUnit,this->m_iSet);
			RuleRTargetValueArr.Add(pRuleRTargetValue);
		}
		m_pRec->MoveNext();
	}
}
bool ConForRuleRecom::beInRuleRTValueArrByCGMark(CString cGMark)
{
	bool result=false;
	RuleRTargetValue *pRuleRTargetValue;
	for(int i=0;i<RuleRTargetValueArr.GetSize();i++)
	{
		pRuleRTargetValue=RuleRTargetValueArr[i];
		if(pRuleRTargetValue->CGMark==cGMark)
		{
			result=true;
			break;
		}
	}
	return result;
}