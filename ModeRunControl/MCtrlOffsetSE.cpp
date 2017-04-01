// MCtrlOffsetSE.cpp: implementation of the MCtrlOffsetSE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlOffsetSE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlOffsetSE::MCtrlOffsetSE()
{
	ResetCtrlData();
}

MCtrlOffsetSE::~MCtrlOffsetSE()
{

}
MCtrlOffsetSE& MCtrlOffsetSE::operator =(const MCtrlOffsetSE& RightValue)
{
	/////////////////////////////////////////////////////////////////////////////////
	//复制常规成员
	this->m_iSOffset_Type=RightValue.m_iSOffset_Type;
	this->m_iSOffset_Value1=RightValue.m_iSOffset_Value1;
	this->m_iSOffset_Value2=RightValue.m_iSOffset_Value2;
	this->m_iSOffset_Value3=RightValue.m_iSOffset_Value3;
	this->m_iSOffset_Value4=RightValue.m_iSOffset_Value4;
	this->m_iSOffset_Value5=RightValue.m_iSOffset_Value5;
	this->m_strS_Time=RightValue.m_strS_Time;

	this->m_iEOffset_Type=RightValue.m_iEOffset_Type;
	this->m_iEOffset_Value1=RightValue.m_iEOffset_Value1;
	this->m_iEOffset_Value2=RightValue.m_iEOffset_Value2;
	this->m_iEOffset_Value3=RightValue.m_iEOffset_Value3;
	this->m_iEOffset_Value4=RightValue.m_iEOffset_Value4;
	this->m_iEOffset_Value5=RightValue.m_iEOffset_Value5;
	this->m_strE_Time=RightValue.m_strE_Time;
    return *this;
///////////////////////////////////////////////////////////////////////////////////////////////////////

}
void MCtrlOffsetSE::GetSETime(CTime calDataTime,CTime &sTime,CTime &eTime)
{
	CString str;
	if(m_iSOffset_Type==0)
	{
		sTime=calDataTime-CTimeSpan( 0, 0, 0, this->m_iSOffset_Value1);
	}
	else if(m_iSOffset_Type==1)
	{	
		sTime=calDataTime-CTimeSpan(this->m_iSOffset_Value1, 0, 0,0);
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strS_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		sTime=CTime(sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iSOffset_Type==2)
	{
		sTime=calDataTime-CTimeSpan(this->m_iSOffset_Value1*7, 0, 0,0);
		int cTimeWeek=m_iSOffset_Value2+2;
		if(cTimeWeek>7)
			cTimeWeek=1;//周日
		sTime-=CTimeSpan(sTime.GetDayOfWeek()-1, 0, 0,0);
		for(int i=1;i<7;i++)
		{
			int dOfWeek=sTime.GetDayOfWeek();
			if(dOfWeek==cTimeWeek)
				break;
			sTime+=CTimeSpan(1, 0, 0,0);
		}
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strS_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		sTime=CTime(sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iSOffset_Type==3)
	{
		sTime=calDataTime;
		int month=sTime.GetMonth();
		int changeCount=0;//通过循环移动天而移动n个月
		if(m_iSOffset_Value1!=0)//移动到
		{
			while(m_iSOffset_Value1!=changeCount)
			{
				int tempMonth1=sTime.GetMonth();
				if(m_iSOffset_Value1>0)//前n个月
				{
					sTime-=CTimeSpan(1, 0, 0,0);
					int tempMonth2=sTime.GetMonth();
					if(tempMonth1!=tempMonth2)
						changeCount++;
				}
				else
				{
					sTime+=CTimeSpan(1, 0, 0,0);
					int tempMonth2=sTime.GetMonth();
					if(tempMonth1!=tempMonth2)
						changeCount--;
				}
			}
		}
		else//月内移动,移动到月末。
		{
			for(int i=0;i<32;i++)
			{
				int tempMonth1=sTime.GetMonth();
				sTime+=CTimeSpan(1, 0, 0,0);
				int tempMonth2=sTime.GetMonth();
				if(tempMonth2!=tempMonth1)
				{
					sTime-=CTimeSpan(1, 0, 0,0);
					break;
				}
			}
		}
		if(m_iSOffset_Value1>=0)// 前N月
		{
			CTime tempTime=sTime;//记录该月月末最后一天。如果设置为2月31号，则取该月最后一天。
			bool beFind=false;
			int maxDay=sTime.GetDay();
			for(int i=maxDay;i>=0;i--)
			{
				if(sTime.GetDay()==m_iSOffset_Value2+1)
				{
					beFind=true;
					break;
				}
				sTime-=CTimeSpan(1, 0, 0,0);
			}
			if(!beFind)
				sTime=tempTime;
		}
		else//后N月
		{
			bool beFind=false;
			for(int i=0;i<32;i++)//从1号往后数，直到到达指定的天
			{
				if(sTime.GetDay()==m_iSOffset_Value2+1)
				{
					beFind=true;
					break;
				}
				int tempMonth1=sTime.GetMonth();
				sTime+=CTimeSpan(1, 0, 0,0);
				int tempMonth2=sTime.GetMonth();
				if(tempMonth2!=tempMonth1)
				{
					sTime-=CTimeSpan(1, 0, 0,0);
					break;
				}
			}
		}
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strS_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		sTime=CTime(sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iSOffset_Type==4)//to do
	{
		sTime=calDataTime;
		eTime=calDataTime;
	}
	if(m_iEOffset_Type==0)
	{
		eTime=calDataTime-CTimeSpan( 0, 0, 0, this->m_iEOffset_Value1);
	}
	else if(m_iEOffset_Type==1)
	{
		eTime=calDataTime-CTimeSpan(this->m_iEOffset_Value1, 0, 0,0);
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strE_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		eTime=CTime(eTime.GetYear(),eTime.GetMonth(),eTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iEOffset_Type==2)
	{
		eTime=calDataTime-CTimeSpan(this->m_iEOffset_Value1*7, 0, 0,0);
		int cTimeWeek=m_iEOffset_Value2+2;
		if(cTimeWeek>7)
			cTimeWeek=1;//周日
		eTime-=CTimeSpan(eTime.GetDayOfWeek()-1, 0, 0,0);
		for(int i=1;i<7;i++)
		{
			int dOfWeek=eTime.GetDayOfWeek();
			if(dOfWeek==cTimeWeek)
				break;
			eTime+=CTimeSpan(1, 0, 0,0);
		}
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strE_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		eTime=CTime(eTime.GetYear(),eTime.GetMonth(),eTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iEOffset_Type==3)
	{
		eTime=calDataTime;
		int month=eTime.GetMonth();
		int changeCount=0;//通过循环移动天而移动n个月
		if(m_iEOffset_Value1!=0)//移动到
		{
			while(m_iEOffset_Value1!=changeCount)
			{
				int tempMonth1=eTime.GetMonth();
				if(m_iEOffset_Value1>0)//前n个月
				{
					eTime-=CTimeSpan(1, 0, 0,0);
					int tempMonth2=eTime.GetMonth();
					if(tempMonth1!=tempMonth2)
						changeCount++;
				}
				else
				{
					eTime+=CTimeSpan(1, 0, 0,0);
					int tempMonth2=eTime.GetMonth();
					if(tempMonth1!=tempMonth2)
						changeCount--;
				}
			}
		}
		else//月内移动,移动到月末。
		{
			for(int i=0;i<32;i++)
			{
				int tempMonth1=eTime.GetMonth();
				eTime+=CTimeSpan(1, 0, 0,0);
				int tempMonth2=eTime.GetMonth();
				if(tempMonth2!=tempMonth1)
				{
					eTime-=CTimeSpan(1, 0, 0,0);
					break;
				}
			}
		}
		if(m_iEOffset_Value1>=0)// 前N月
		{
			CTime tempTime=eTime;//记录该月月末最后一天。如果设置为2月31号，则取该月最后一天。
			bool beFind=false;
			int maxDay=eTime.GetDay();
			for(int i=maxDay;i>=0;i--)
			{
				if(eTime.GetDay()==m_iEOffset_Value2+1)
				{
					beFind=true;
					break;
				}
				eTime-=CTimeSpan(1, 0, 0,0);
			}
			if(!beFind)
				eTime=tempTime;
		}
		else//后N月
		{
			bool beFind=false;
			for(int i=0;i<32;i++)//从1号往后数，直到到达指定的天
			{
				if(eTime.GetDay()==m_iEOffset_Value2+1)
				{
					beFind=true;
					break;
				}
				int tempMonth1=eTime.GetMonth();
				eTime+=CTimeSpan(1, 0, 0,0);
				int tempMonth2=eTime.GetMonth();
				if(tempMonth2!=tempMonth1)
				{
					eTime-=CTimeSpan(1, 0, 0,0);
					break;
				}
			}
		}
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(this->m_strE_Time, "%d:%d:%d",&nHour,&nMin,&nSec);  
		eTime=CTime(eTime.GetYear(),eTime.GetMonth(),eTime.GetDay(),nHour,nMin,nSec);
	}
	else if(m_iEOffset_Type==4)//to do
	{
		sTime=calDataTime;
		eTime=calDataTime;
	}
}
CString MCtrlOffsetSE::GetSTimeStrDesp()
{
	CString str;
	if(m_iSOffset_Type==0)
	{
		str.Format("前%d秒",this->m_iSOffset_Value1);
	}
	else if(m_iSOffset_Type==1)
	{
		str.Format("前%d天,在%s",this->m_iSOffset_Value1,this->m_strS_Time);
	}
	else if(m_iSOffset_Type==2)
	{
		str.Format("前%d周,在周%d的%s",this->m_iSOffset_Value1,m_iSOffset_Value2+1,this->m_strS_Time);
	}
	else if(m_iSOffset_Type==3)
	{
		str.Format("前%d月,在%d号的%s",this->m_iSOffset_Value1,m_iSOffset_Value2+1,this->m_strS_Time);
	}
	else if(m_iSOffset_Type==4)
	{
		if(m_iSOffset_Value2==0)//
		{
			str.Format("前%d值,该值开始时刻",this->m_iSOffset_Value1);
		}
		else
		{
			str.Format("前%d值,该值终止时刻",this->m_iSOffset_Value1);
		}
	}
	return str;
}
CString MCtrlOffsetSE::GetETimeStrDesp()
{
	CString str;
	if(m_iEOffset_Type==0)
	{
		str.Format("前%d秒",this->m_iEOffset_Value1);
	}
	else if(m_iEOffset_Type==1)
	{
		str.Format("前%d天,在%s",this->m_iEOffset_Value1,this->m_strE_Time);
	}
	else if(m_iEOffset_Type==2)
	{
		str.Format("前%d周,在周%d的%s",this->m_iEOffset_Value1,m_iEOffset_Value2+1,this->m_strE_Time);
	}
	else if(m_iEOffset_Type==3)
	{
		str.Format("前%d月,在%d号的%s",this->m_iEOffset_Value1,m_iEOffset_Value2+1,this->m_strE_Time);
	}
	else if(m_iEOffset_Type==4)
	{
		if(m_iEOffset_Value2==0)//
		{
			str.Format("前%d值,该值开始时刻",this->m_iEOffset_Value1);
		}
		else
		{
			str.Format("前%d值,该值终止时刻",this->m_iEOffset_Value1);
		}
	}
	return str;
}
void MCtrlOffsetSE::ResetCtrlData()
{
	m_iSOffset_Type=0;
	m_iSOffset_Value1=0;
	m_iSOffset_Value2=0;
	m_iSOffset_Value3=0;
	m_iSOffset_Value4=0;
	m_iSOffset_Value5=0;
	m_strS_Time="";

	m_iEOffset_Type=0;
	m_iEOffset_Value1=0;
	m_iEOffset_Value2=0;
	m_iEOffset_Value3=0;
	m_iEOffset_Value4=0;
	m_iEOffset_Value5=0;
	m_strE_Time="";
}	
