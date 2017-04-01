// MCtrlDay.cpp: implementation of the MCtrlDay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlDay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


MCtrlDay::MCtrlDay(CString modelName):MCtrlType(modelName)
{

}

MCtrlDay::~MCtrlDay()
{
	delAllCtrlData();
}
//tagBeEql==1，若配置时刻和当前时刻相同，则取之；
//tagBeEql==0，去给定时刻的下一时刻
CTime MCtrlDay::getNextMostRecentTime(CTime time,int tagBeEql)
{
	CArray<long,long> timeLongArr;
	for(int i=0;i<m_ArrRunTime.GetSize();i++)//得到配置时刻对应的当前time的具体的时间long值
	{
		CString strTime=m_ArrRunTime[i];
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(strTime, "%d:%d:%d",&nHour,&nMin,&nSec);  
		CTime everyTime=CTime(time.GetYear(),time.GetMonth(),time.GetDay(),nHour,nMin,nSec);
		timeLongArr.Add(everyTime.GetTime());
	}
	for(i=0;i<timeLongArr.GetSize();i++)//从小到达排序
	{
		for(int j=i+1;j<timeLongArr.GetSize();j++)
		{
			if(timeLongArr[i]>timeLongArr[j])
			{
				long temp=timeLongArr[i];
				timeLongArr[i]=timeLongArr[j];
				timeLongArr[j]=temp;
			}
		}
	}
	bool beFind=false;
	for(i=0;i<timeLongArr.GetSize();i++)//寻找下一个临近的值
	{
		if(tagBeEql==0)
		{
			if(timeLongArr[i]>time.GetTime())
			{
				beFind=true;
				time=CTime(timeLongArr[i]);
				break;
			}
		}
		else
		{
			if(timeLongArr[i]>=time.GetTime())
			{
				beFind=true;
				time=CTime(timeLongArr[i]);
				break;
			}
		}

	}
	if(!beFind)
	{
		for(i=0;i<timeLongArr.GetSize();i++)//寻找下一天的临近的值
		{
			timeLongArr[i]+=24*60*60;
			if(tagBeEql==0)
			{
				if(timeLongArr[i]>time.GetTime())
				{
					beFind=true;
					time=CTime(timeLongArr[i]);
					break;
				}
			}
			else
			{
				if(timeLongArr[i]>=time.GetTime())
				{
					beFind=true;
					time=CTime(timeLongArr[i]);
					break;
				}
			}
		}
	}
	return time;
}
int MCtrlDay::getTimeIndexInArrRunTime(CTime time)
{
	int index=-1;
	for(int i=0;i<m_ArrRunTime.GetSize();i++)//得到配置时刻对应的当前time的具体的时间long值
	{
		CString strTimeInArr=m_ArrRunTime[i];
		CString str;
		str.Format("%d:%d:%d",time.GetHour(),time.GetMinute(),time.GetSecond());
		if(strTimeInArr==str)
		{
			index=i;
		}
	}
	return index;
}
int MCtrlDay::GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
								CTime &nextCalAbsTime,CTime &nextCalDataTime,
								CTime &nextDataSTime,CTime &nextDataETime)
{
	//CString strTest=curCalDataTime.Format("%Y-%m-%d %H:%M:%S");
	//AfxMessageBox(strTest);
     int ret=-1;
	 if(m_ArrRunTime.GetSize()==0)
	 {
		 ret=3;
		 return ret;//返回值3，没有定时
	 }
	 //计算下一次要计算的数据时刻
	 if(curCalDataTime.GetTime()==0)//第一次计算，取数据的总时间的开始
	 {
		 if((*m_pTCalcDataAll_STime).GetTime()==0)//数据时间从当前时刻
		 {
			 nextCalDataTime=CTime::GetCurrentTime();//取当前时间
		 }
		 else
		 {
			 nextCalDataTime=(*m_pTCalcDataAll_STime);//取数据的开始时间
		 }
		 nextCalDataTime=getNextMostRecentTime(nextCalDataTime,1);//若配置时刻和数据开始时刻相同则可取之
	 }
	 else
	 {
		nextCalDataTime=getNextMostRecentTime(curCalDataTime,0);
	 }
	 //数据时刻是否在总数据时间内
	 int ret2=getTheTimeInOrOutSETime(nextCalDataTime,*(this->m_pTCalcDataAll_STime),*(this->m_pTCalcDataAll_ETime));
	 if(ret2==2)
	 {
		 ret=2;//数据处理完毕
		 return ret;
	 }
	 int index=getTimeIndexInArrRunTime(nextCalDataTime);
	 MCtrlOffsetSE *pMCtrlOffsetSE=getOffsetObjPtrByIndex(index);
	 if(pMCtrlOffsetSE!=NULL)
	 {
		 pMCtrlOffsetSE->GetSETime(nextCalDataTime,nextDataSTime,nextDataETime);
	 }
	 //判断要计算的数据时刻是否在过去(包括当前时刻)
	 CTime curTime=CTime::GetCurrentTime();
	 if(nextCalDataTime.GetTime()<=curTime.GetTime())//要计算的数据时刻在过去
	 {
		 nextCalAbsTime=CTime::GetCurrentTime();//下次计算在当前时刻
	 }
	 else//要计算的数据时刻在未来
	 {
		 nextCalAbsTime=nextCalDataTime;//下次计算时刻为，要计算的数据时刻。
	 }
	 //判断下次计算时刻是否在生命周期内
	 ret2=getTheTimeInOrOutSETime(nextCalAbsTime,*(this->m_pTRunAbs_STime),*(this->m_pTRunAbs_ETime));
	 if(ret2==2)
	 {
		 ret=0;//生命结束
		 return ret;
	 }
	 else if(ret2==1)
	 {
		 ret=1;//进入定时
	 }
	 else if(ret2==0)
	 {
		 ret=1;//进入定时
		 nextCalAbsTime=(*m_pTRunAbs_STime);
	 }
	 return ret;
}
void MCtrlDay::ResetCtrlData()
{
	m_ArrRunTime.RemoveAll();
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<m_ArrOffsetSE.GetSize();i++)
	{
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		if(pMCtrlOffsetSE==NULL)
		{
			delete pMCtrlOffsetSE;
			pMCtrlOffsetSE=NULL;
		}
	}
	m_ArrOffsetSE.RemoveAll();
}
void MCtrlDay::SaveCtrlDataToDB()
{
	DelCtrlDataIoDB();//先删除记录

	////打开表记录，然后
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_day"); //写信息到模块列表数据表
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}	
    //不断写入
	if(!pRecord->adoEOF) pRecord->MoveLast();
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<this->m_ArrRunTime.GetSize();i++)
	{
		pRecord->AddNew();
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //放Name
		pRecord->PutCollect(_T("Time"),_variant_t(m_ArrRunTime[i])); //放DESP
		pRecord->PutCollect(_T("SOffset_Type"),_variant_t((short)pMCtrlOffsetSE->m_iSOffset_Type)); //放
		
		pRecord->PutCollect(_T("SOffset_Value1"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value1) ); //放
		pRecord->PutCollect(_T("SOffset_Value2"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value2) ); //放
		pRecord->PutCollect(_T("SOffset_Value3"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value3) ); //放
		pRecord->PutCollect(_T("SOffset_Value4"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value4) ); //放
		pRecord->PutCollect(_T("SOffset_Value5"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value5) ); //放
		if(pMCtrlOffsetSE->m_strS_Time!="")
			pRecord->PutCollect(_T("S_Time"),_variant_t(pMCtrlOffsetSE->m_strS_Time) ); //放

		pRecord->PutCollect(_T("EOffset_Type"),_variant_t((short)pMCtrlOffsetSE->m_iEOffset_Type) ); //放
		pRecord->PutCollect(_T("EOffset_Value1"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value1) ); //放
		pRecord->PutCollect(_T("EOffset_Value2"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value2) ); //放
		pRecord->PutCollect(_T("EOffset_Value3"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value3) ); //放	
		pRecord->PutCollect(_T("EOffset_Value4"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value4) ); //放
		pRecord->PutCollect(_T("EOffset_Value5"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value5) ); //放
		if(pMCtrlOffsetSE->m_strE_Time!="")
			pRecord->PutCollect(_T("E_Time"),_variant_t(pMCtrlOffsetSE->m_strE_Time) ); //放
		pRecord->Update();
	}
	pRecord->Close();
}
void MCtrlDay::DelCtrlDataIoDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol_day where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}
}

void MCtrlDay::ReadCtrlData()
{
	delAllCtrlData();
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM mode_runcontrol_day where Mode_Name='%s'",this->m_strModelName);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	MCtrlOffsetSE *pMCtrlOffsetSE;
	while(!pRecord->adoEOF)
	{
		CString Time="";
		CString SOffset_Type="";
		CString SOffset_Value1="";
		CString SOffset_Value2="";
		CString SOffset_Value3="";
		CString SOffset_Value4="";
		CString SOffset_Value5="";
		CString S_Time="";
		CString EOffset_Type="";
		CString EOffset_Value1="";
		CString EOffset_Value2="";
		CString EOffset_Value3="";
		CString EOffset_Value4="";
		CString EOffset_Value5="";
		CString E_Time="";
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("Time");
			if(Tempvalue.vt != VT_NULL) Time = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SOffset_Type");
			if(Tempvalue.vt != VT_NULL) SOffset_Type = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("SOffset_Value1");
			if(Tempvalue.vt != VT_NULL) SOffset_Value1 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SOffset_Value2");
			if(Tempvalue.vt != VT_NULL) SOffset_Value2 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SOffset_Value3");
			if(Tempvalue.vt != VT_NULL) SOffset_Value3 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SOffset_Value4");
			if(Tempvalue.vt != VT_NULL) SOffset_Value4 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SOffset_Value5");
			if(Tempvalue.vt != VT_NULL) SOffset_Value5 = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("S_Time");
			if(Tempvalue.vt != VT_NULL) S_Time = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("EOffset_Type");
			if(Tempvalue.vt != VT_NULL) EOffset_Type = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("EOffset_Value1");
			if(Tempvalue.vt != VT_NULL) EOffset_Value1 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("EOffset_Value2");
			if(Tempvalue.vt != VT_NULL) EOffset_Value2 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("EOffset_Value3");
			if(Tempvalue.vt != VT_NULL) EOffset_Value3 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("EOffset_Value4");
			if(Tempvalue.vt != VT_NULL) EOffset_Value4 = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("EOffset_Value5");
			if(Tempvalue.vt != VT_NULL) EOffset_Value5 = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("E_Time");
			if(Tempvalue.vt != VT_NULL) E_Time = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			pMCtrlOffsetSE=new MCtrlOffsetSE;
			pMCtrlOffsetSE->m_iSOffset_Type=atoi(SOffset_Type);
			pMCtrlOffsetSE->m_iSOffset_Value1=atoi(SOffset_Value1);
			pMCtrlOffsetSE->m_iSOffset_Value2=atoi(SOffset_Value2);
			pMCtrlOffsetSE->m_iSOffset_Value3=atoi(SOffset_Value3);
			pMCtrlOffsetSE->m_iSOffset_Value4=atoi(SOffset_Value4);
			pMCtrlOffsetSE->m_iSOffset_Value5=atoi(SOffset_Value5);
			pMCtrlOffsetSE->m_strS_Time=S_Time;
			pMCtrlOffsetSE->m_iEOffset_Type=atoi(EOffset_Type);
			pMCtrlOffsetSE->m_iEOffset_Value1=atoi(EOffset_Value1);
			pMCtrlOffsetSE->m_iEOffset_Value2=atoi(EOffset_Value2);
			pMCtrlOffsetSE->m_iEOffset_Value3=atoi(EOffset_Value3);
			pMCtrlOffsetSE->m_iEOffset_Value4=atoi(EOffset_Value4);
			pMCtrlOffsetSE->m_iEOffset_Value5=atoi(EOffset_Value5);
			pMCtrlOffsetSE->m_strE_Time=E_Time;
			this->m_ArrRunTime.Add(Time);
			this->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}

void MCtrlDay::delAllCtrlData()
{
	ResetCtrlData();
}
