// RuleData.cpp: implementation of the CRuleData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuleData::CRuleData()
{
	m_iDiffValue=-1;
}

CRuleData::~CRuleData()
{

}

void CRuleData::CalcDiffValue()
{
	if(this->m_strConCode!=(*m_pStrInPutConCode))
		return ;
	if((*m_piTagToFOrB)==0)//前向
	{
		m_iDiffValue=calcArrDif(m_NameFArr,m_pTagBeInFArr,m_piDisValueFArr);
	}
	else
	{
		m_iDiffValue=calcArrDif(m_NameBArr,m_pTagBeInBArr,m_piDisValueBArr);
	}
}
int CRuleData::calcArrDif(CStringArray *nameArr,CArray<int,int> *pTagBeInArr,CArray<int,int> *piDisValueArr)
{
	int difSum=0;
	for(int i=0;i<(*nameArr).GetSize();i++)
	{
		CString strName=(*nameArr)[i];
		int iDisValue=(*piDisValueArr)[i];
		if((*pTagBeInArr)[i]!=1)
			continue;
		if(beDiffValueByName(strName,iDisValue))
		{
			difSum++;
		}
	}
	return difSum;
}
bool CRuleData::beDiffValueByName(CString strName,int iDisValue)//若有差异返回true
{
	bool ret=false;
	for(int i=0;i<(*m_pStrNameArrDB).GetSize();i++)
	{
		if((*m_pStrNameArrDB)[i]==strName)
		{
			if(m_iDisValue[i]!=iDisValue)
			{
				ret=true;
			}
			break;
		}
	}
	return ret;
} 
int CRuleData::GetDisValueByName(CString strVarName)
{
	int ret=0;
	for(int i=0;i<(*m_pStrNameArrDB).GetSize();i++)
	{
		if((*m_pStrNameArrDB)[i]==strVarName)
		{
			ret=m_iDisValue[i];
			break;
		}
	}
	return ret;
}