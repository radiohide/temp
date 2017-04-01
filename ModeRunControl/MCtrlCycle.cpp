// MCtrlCycle.cpp: implementation of the MCtrlCycle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlCycle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlCycle::MCtrlCycle(CString modelName):MCtrlType(modelName)
{
	ResetCtrlData();
}

MCtrlCycle::~MCtrlCycle()
{
}
//第一次计算的数据时间为CTime(0);第一次计算的当前时间为CTime(0);

//返回值0，即该任务生命周期结束。
//返回值1，进行下一次定时。
//返回值2, 数据处理完毕
//返回值3，没有定时
/*
CTime *m_pTRunAbs_STime;
CTime *m_pTRunAbs_ETime;
CTime *m_pTCalcDataAll_STime;
CTime *m_pTCalcDataAll_ETime;
*/

int MCtrlCycle::GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
								CTime &nextCalAbsTime,CTime &nextCalDataTime,
								CTime &nextDataSTime,CTime &nextDataETime)
{
     int ret=-1;
	 //计算下一次要计算的数据时刻
	 if(curCalDataTime.GetTime()==0)//第一次计算，取数据的总时间的开始
	 {	
		 if((*m_pTCalcDataAll_STime).GetTime()==0)//数据时间从当前时刻
		 {
			 CTime now=CTime::GetCurrentTime();
			int year=now.GetYear();
			int month=now.GetMonth();
			int day=now.GetDay();
			int hour=now.GetHour();
			int minute=now.GetMinute();
			int second=now.GetSecond();

			 if(m_iUnit==0)//单位为秒
			 {
				 nextCalDataTime=CTime::GetCurrentTime();//取当前时间
			 }
			 else if(m_iUnit==1)////单位为分
			 {
				 int passMinute=(minute/m_lCycle_Length+1)*m_lCycle_Length;
				 nextCalDataTime=CTime(year,month,day,hour,0,0);
				 nextCalDataTime+=CTimeSpan( 0, 0, passMinute, 0);
			 }
			 else if(m_iUnit==2)////单位为小时
			 {
				 int passHour=(hour/m_lCycle_Length+1)*m_lCycle_Length;
				 nextCalDataTime=CTime(year,month,day,0,0,0);
				 nextCalDataTime+=CTimeSpan( 0, passHour, 0, 0);
			 }
			 else if(m_iUnit==3)////单位为天
			 {
				 nextCalDataTime=CTime(year,month,day,0,0,0);
				 nextCalDataTime+=CTimeSpan( 1, 0, 0, 0);
			 }
		 }
		 else
		 {
			 nextCalDataTime=(*m_pTCalcDataAll_STime);//取数据的开始时间
		 }
	 }
	 else//不是第一次计算，则数据时间增加一个周期。
	 {
		 if(m_iUnit==0)//单位为秒
		 {
			 nextCalDataTime+=CTimeSpan( 0, 0, 0, m_lCycle_Length);
		 }
		 else if(m_iUnit==1)////单位为分
		 {
			 nextCalDataTime+=CTimeSpan( 0, 0, m_lCycle_Length, 0);
		 }
		 else if(m_iUnit==2)////单位为小时
		 {
			 nextCalDataTime+=CTimeSpan( 0, m_lCycle_Length, 0, 0);
		 }
		 else if(m_iUnit==3)////单位为天
		 {
			 nextCalDataTime+=CTimeSpan(m_lCycle_Length, 0, 0, 0);
		 }
	 }
	 //数据时刻是否在总数据时间内
	 int ret2=getTheTimeInOrOutSETime(nextCalDataTime,*(this->m_pTCalcDataAll_STime),*(this->m_pTCalcDataAll_ETime));
	 if(ret2==2)
	 {
		 ret=2;//数据处理完毕
		 return ret;
	 }
	 MCtrlOffsetSE *pMCtrlOffsetSE=getOffsetObjPtrByIndex(0);
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
void MCtrlCycle::DelCtrlDataIoDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol_cycle where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}
}
void MCtrlCycle::SaveCtrlDataToDB()
{
	DelCtrlDataIoDB();//先删除记录

	////打开表记录，然后
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_cycle"); //写信息到模块列表数据表
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
	pRecord->AddNew();
	MCtrlOffsetSE *pMCtrlOffsetSE;
	pMCtrlOffsetSE=m_ArrOffsetSE[0];
	pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //放Name
	pRecord->PutCollect(_T("Unit"),_variant_t((short)this->m_iUnit) ); //放DESP
	pRecord->PutCollect(_T("Cycle_Length"),_variant_t(this->m_lCycle_Length) ); //放
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
	pRecord->Close();
}
void MCtrlCycle::ReadCtrlData()
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM mode_runcontrol_cycle where Mode_Name='%s'",this->m_strModelName);
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
	if(!pRecord->adoEOF)
	{
		CString Unit="";
		CString Cycle_Length="";
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
			Tempvalue = pRecord->GetCollect("Unit");
			if(Tempvalue.vt != VT_NULL) Unit = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("Cycle_Length");
			if(Tempvalue.vt != VT_NULL) Cycle_Length = (LPCSTR)_bstr_t(Tempvalue);
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
			this->m_iUnit=atoi(Unit);
			if(Cycle_Length!="")
				this->m_lCycle_Length=atol(Cycle_Length);
			pMCtrlOffsetSE=m_ArrOffsetSE[0];
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
		}
	}
	pRecord->Close();
}
void MCtrlCycle::ResetCtrlData()
{
	m_iUnit=0;
	m_lCycle_Length=5;
	MCtrlOffsetSE *pMCtrlOffsetSE;
	if(m_ArrOffsetSE.GetSize()==0)
	{
		pMCtrlOffsetSE=new MCtrlOffsetSE();
		m_ArrOffsetSE.Add(pMCtrlOffsetSE);
	}
	else if(m_ArrOffsetSE.GetSize()>=0)
	{
		pMCtrlOffsetSE=m_ArrOffsetSE[0];
		pMCtrlOffsetSE->ResetCtrlData();
	}
}