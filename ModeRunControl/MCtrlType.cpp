// MCtrlType.cpp: implementation of the MCtrlType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlType::MCtrlType(CString modelName)
{
	m_strModelName=modelName;
	m_pTRunAbs_STime=NULL;
	m_pTRunAbs_ETime=NULL;
	m_pTCalcDataAll_STime=NULL;
	m_pTCalcDataAll_ETime=NULL;
}

MCtrlType::~MCtrlType()
{
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<m_ArrOffsetSE.GetSize();i++)
	{
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		if(pMCtrlOffsetSE!=NULL)
		{
			delete pMCtrlOffsetSE;
			pMCtrlOffsetSE=NULL;
		}
	}
	m_ArrOffsetSE.RemoveAll();
}
void MCtrlType::SetRunAbsSETimeAndDataAllSETimePtr(CTime *m_pTRunAbs_STime,CTime *m_pTRunAbs_ETime,
												   CTime *m_pTCalcDataAll_STime,CTime *m_pTCalcDataAll_ETime)
{
	this->m_pTRunAbs_STime=m_pTRunAbs_STime;
	this->m_pTRunAbs_ETime=m_pTRunAbs_ETime;
	this->m_pTCalcDataAll_STime=m_pTCalcDataAll_STime;
	this->m_pTCalcDataAll_ETime=m_pTCalcDataAll_ETime;
}
int MCtrlType::GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
					CTime &nextCalAbsTime,CTime &nextCalDataTime,
					CTime &nextDataSTime,CTime &nextDataETime)
{
	return 1;
}
void MCtrlType::ReadCtrlData()
{

}
void MCtrlType::ResetCtrlData()
{

}
void MCtrlType::SaveCtrlDataToDB()
{

}
void MCtrlType::DelCtrlDataIoDB()
{
	
}
void MCtrlType::delAllCtrlData()
{

}
void MCtrlType::SetConPtr(_ConnectionPtr &pCon)
{
	this->pCon=pCon;
}
MCtrlOffsetSE* MCtrlType::getOffsetObjPtrByIndex(int index)
{
	MCtrlOffsetSE *p=NULL;
	if((index<m_ArrOffsetSE.GetSize())&&(index>=0))
	{
		p=m_ArrOffsetSE[index];
	}
	return p;
}
int MCtrlType::getTheTimeInOrOutSETime(const CTime &theTime,const CTime &sTime,const CTime &eTime)
{
	int ret=-1;
	if(theTime.GetTime()<sTime.GetTime())
	{
		ret=0;//在开始之前
	}
	else if(theTime.GetTime()>=sTime.GetTime())//在开始之后（包括开始）
	{
		if(eTime.GetTime()==0)
		{
			ret=1;//在开始之内
		}
		else 
		{
			if(theTime.GetTime()<=eTime.GetTime())
			{
				ret=1;//在开始之内
			}
			else
			{
				ret=2;//在结束之后
			}
		}
	}
	return ret;
}