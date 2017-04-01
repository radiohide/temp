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
//tagBeEql==1��������ʱ�̺͵�ǰʱ����ͬ����ȡ֮��
//tagBeEql==0��ȥ����ʱ�̵���һʱ��
CTime MCtrlDay::getNextMostRecentTime(CTime time,int tagBeEql)
{
	CArray<long,long> timeLongArr;
	for(int i=0;i<m_ArrRunTime.GetSize();i++)//�õ�����ʱ�̶�Ӧ�ĵ�ǰtime�ľ����ʱ��longֵ
	{
		CString strTime=m_ArrRunTime[i];
		int nHour=0;
		int nMin=0;
		int nSec=0;   
		sscanf(strTime, "%d:%d:%d",&nHour,&nMin,&nSec);  
		CTime everyTime=CTime(time.GetYear(),time.GetMonth(),time.GetDay(),nHour,nMin,nSec);
		timeLongArr.Add(everyTime.GetTime());
	}
	for(i=0;i<timeLongArr.GetSize();i++)//��С��������
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
	for(i=0;i<timeLongArr.GetSize();i++)//Ѱ����һ���ٽ���ֵ
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
		for(i=0;i<timeLongArr.GetSize();i++)//Ѱ����һ����ٽ���ֵ
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
	for(int i=0;i<m_ArrRunTime.GetSize();i++)//�õ�����ʱ�̶�Ӧ�ĵ�ǰtime�ľ����ʱ��longֵ
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
		 return ret;//����ֵ3��û�ж�ʱ
	 }
	 //������һ��Ҫ���������ʱ��
	 if(curCalDataTime.GetTime()==0)//��һ�μ��㣬ȡ���ݵ���ʱ��Ŀ�ʼ
	 {
		 if((*m_pTCalcDataAll_STime).GetTime()==0)//����ʱ��ӵ�ǰʱ��
		 {
			 nextCalDataTime=CTime::GetCurrentTime();//ȡ��ǰʱ��
		 }
		 else
		 {
			 nextCalDataTime=(*m_pTCalcDataAll_STime);//ȡ���ݵĿ�ʼʱ��
		 }
		 nextCalDataTime=getNextMostRecentTime(nextCalDataTime,1);//������ʱ�̺����ݿ�ʼʱ����ͬ���ȡ֮
	 }
	 else
	 {
		nextCalDataTime=getNextMostRecentTime(curCalDataTime,0);
	 }
	 //����ʱ���Ƿ���������ʱ����
	 int ret2=getTheTimeInOrOutSETime(nextCalDataTime,*(this->m_pTCalcDataAll_STime),*(this->m_pTCalcDataAll_ETime));
	 if(ret2==2)
	 {
		 ret=2;//���ݴ������
		 return ret;
	 }
	 int index=getTimeIndexInArrRunTime(nextCalDataTime);
	 MCtrlOffsetSE *pMCtrlOffsetSE=getOffsetObjPtrByIndex(index);
	 if(pMCtrlOffsetSE!=NULL)
	 {
		 pMCtrlOffsetSE->GetSETime(nextCalDataTime,nextDataSTime,nextDataETime);
	 }
	 //�ж�Ҫ���������ʱ���Ƿ��ڹ�ȥ(������ǰʱ��)
	 CTime curTime=CTime::GetCurrentTime();
	 if(nextCalDataTime.GetTime()<=curTime.GetTime())//Ҫ���������ʱ���ڹ�ȥ
	 {
		 nextCalAbsTime=CTime::GetCurrentTime();//�´μ����ڵ�ǰʱ��
	 }
	 else//Ҫ���������ʱ����δ��
	 {
		 nextCalAbsTime=nextCalDataTime;//�´μ���ʱ��Ϊ��Ҫ���������ʱ�̡�
	 }
	 //�ж��´μ���ʱ���Ƿ�������������
	 ret2=getTheTimeInOrOutSETime(nextCalAbsTime,*(this->m_pTRunAbs_STime),*(this->m_pTRunAbs_ETime));
	 if(ret2==2)
	 {
		 ret=0;//��������
		 return ret;
	 }
	 else if(ret2==1)
	 {
		 ret=1;//���붨ʱ
	 }
	 else if(ret2==0)
	 {
		 ret=1;//���붨ʱ
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
	DelCtrlDataIoDB();//��ɾ����¼

	////�򿪱��¼��Ȼ��
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_day"); //д��Ϣ��ģ���б����ݱ�
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}	
    //����д��
	if(!pRecord->adoEOF) pRecord->MoveLast();
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<this->m_ArrRunTime.GetSize();i++)
	{
		pRecord->AddNew();
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //��Name
		pRecord->PutCollect(_T("Time"),_variant_t(m_ArrRunTime[i])); //��DESP
		pRecord->PutCollect(_T("SOffset_Type"),_variant_t((short)pMCtrlOffsetSE->m_iSOffset_Type)); //��
		
		pRecord->PutCollect(_T("SOffset_Value1"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value1) ); //��
		pRecord->PutCollect(_T("SOffset_Value2"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value2) ); //��
		pRecord->PutCollect(_T("SOffset_Value3"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value3) ); //��
		pRecord->PutCollect(_T("SOffset_Value4"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value4) ); //��
		pRecord->PutCollect(_T("SOffset_Value5"),_variant_t((long)pMCtrlOffsetSE->m_iSOffset_Value5) ); //��
		if(pMCtrlOffsetSE->m_strS_Time!="")
			pRecord->PutCollect(_T("S_Time"),_variant_t(pMCtrlOffsetSE->m_strS_Time) ); //��

		pRecord->PutCollect(_T("EOffset_Type"),_variant_t((short)pMCtrlOffsetSE->m_iEOffset_Type) ); //��
		pRecord->PutCollect(_T("EOffset_Value1"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value1) ); //��
		pRecord->PutCollect(_T("EOffset_Value2"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value2) ); //��
		pRecord->PutCollect(_T("EOffset_Value3"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value3) ); //��	
		pRecord->PutCollect(_T("EOffset_Value4"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value4) ); //��
		pRecord->PutCollect(_T("EOffset_Value5"),_variant_t((long)pMCtrlOffsetSE->m_iEOffset_Value5) ); //��
		if(pMCtrlOffsetSE->m_strE_Time!="")
			pRecord->PutCollect(_T("E_Time"),_variant_t(pMCtrlOffsetSE->m_strE_Time) ); //��
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
	//��������������
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
