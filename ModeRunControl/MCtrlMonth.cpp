// MCtrlMonth.cpp: implementation of the MCtrlMonth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlMonth.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlMonth::MCtrlMonth(CString modelName):MCtrlType(modelName)
{

}

MCtrlMonth::~MCtrlMonth()
{
	delAllCtrlData();
}
void MCtrlMonth::SetNewMonthNums(CArray<int,int> &arrMonthArr)
{
	CArray<int,int> needBeDelMNum;
	for(int i=0;i<m_ArrMonth.GetSize();i++)//找需要删除的月份
	{
		int mNum=m_ArrMonth[i];
		bool beFind=false;
		for(int j=0;j<arrMonthArr.GetSize();j++)
		{
			int mNewWeek=arrMonthArr[j];
			if(mNum==mNewWeek)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			needBeDelMNum.Add(mNum);
	}
	for(i=0;i<needBeDelMNum.GetSize();i++)
	{
		DelRecordByMNum(needBeDelMNum[i]);
	}
	for(i=0;i<arrMonthArr.GetSize();i++)//检查是否有新增星期，若有增加记录
	{
		int mNewWeek=arrMonthArr[i];
		bool beFind=false;
		for(int j=0;j<m_ArrMonth.GetSize();j++)
		{
			int mNum=m_ArrMonth[j];
			if(mNum==mNewWeek)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			AddOneRecord(mNewWeek);
	}
}
void MCtrlMonth::GetTheRecordDataByInIndex(int inIndex,int *&pDay,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE)
{
	pDay=&(m_ArrDay[inIndex]);
	pStrTime=&(m_ArrRunTime[inIndex]);
	pMCtrlOffsetSE=m_ArrOffsetSE[inIndex];
}
void MCtrlMonth::GetTheMonthData(int mInNum,int *&pDay,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE)
{
	bool beFind=false;
	for(int i=0;i<m_ArrMonth.GetSize();i++)
	{
		int mNum=m_ArrMonth[i];
		CString strTime=m_ArrRunTime[i];
		if((mInNum==mNum)&&(strTime==""))
		{
			beFind=true;
			pDay=&(m_ArrDay[i]);
			pStrTime=&(m_ArrRunTime[i]);
			pMCtrlOffsetSE=m_ArrOffsetSE[i];
			break;
		}
	}
	if(!beFind)
	{
		AddOneRecord(mInNum);
		pDay=&(m_ArrDay[m_ArrDay.GetSize()-1]);
		pStrTime=&(m_ArrRunTime[m_ArrRunTime.GetSize()-1]);
		pMCtrlOffsetSE=m_ArrOffsetSE[m_ArrOffsetSE.GetSize()-1];
	}	
}
void MCtrlMonth::DelRecordByMNum(int monthNum)
{
	for(int i=m_ArrInIndex.GetSize()-1;i>=0;i--)
	{
		if(m_ArrMonth[i]==monthNum)
		{
			m_ArrMonth.RemoveAt(i);
			m_ArrDay.RemoveAt(i);
			m_ArrRunTime.RemoveAt(i);
			m_ArrInIndex.RemoveAt(i);
			MCtrlOffsetSE *pMCtrlOffsetSE;
			pMCtrlOffsetSE=m_ArrOffsetSE[i];
			if(pMCtrlOffsetSE!=NULL)
			{
				delete pMCtrlOffsetSE;
				pMCtrlOffsetSE=NULL;
			}
			m_ArrOffsetSE.RemoveAt(i);
		}
	}
}
void MCtrlMonth::DelRecordByInIndex(int inindex)
{
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<this->m_ArrInIndex.GetSize();i++)
	{
		if(m_ArrInIndex[i]==inindex)
		{
			if(beLaseOneRecord(m_ArrMonth[i]))
			{
				pMCtrlOffsetSE=	m_ArrOffsetSE[i];
				pMCtrlOffsetSE->ResetCtrlData();
				m_ArrRunTime[i]="";
				m_ArrDay[i]=-1;
			}
			else
			{
				pMCtrlOffsetSE=	m_ArrOffsetSE[i];
				if(pMCtrlOffsetSE!=NULL)
				{
					delete pMCtrlOffsetSE;
					pMCtrlOffsetSE=NULL;
				}
				m_ArrOffsetSE.RemoveAt(i);
				m_ArrRunTime.RemoveAt(i);
				m_ArrInIndex.RemoveAt(i);
				m_ArrMonth.RemoveAt(i);
				m_ArrDay.RemoveAt(i);
			}
		}
	}
}
BOOL MCtrlMonth::beLaseOneRecord(int wNum)
{
	int count=0;
	for(int i=0;i<m_ArrMonth.GetSize();i++)
	{
		if(m_ArrMonth[i]==wNum)
			count++;
	}
	if(count>1)
		return false;
	else
		return true;
}
void MCtrlMonth::GetMonthNums(CArray<int,int> &arrMonthArr)
{
	arrMonthArr.RemoveAll();
	for(int i=0;i<m_ArrMonth.GetSize();i++)
	{
		int wNum=m_ArrMonth[i];
		bool beFind=false;
		for(int j=0;j<arrMonthArr.GetSize();j++)
		{
			int getWNum=arrMonthArr[j];
			if(getWNum==wNum)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			arrMonthArr.Add(wNum);
	}
	for(i=0;i<arrMonthArr.GetSize();i++)
	{
		for(int j=i+1;j<arrMonthArr.GetSize();j++)
		{
			if(arrMonthArr[i]>arrMonthArr[j])
			{
				int temp=arrMonthArr[i];
				arrMonthArr[i]=arrMonthArr[j];
				arrMonthArr[j]=temp;
			}
		}
	}
}
void MCtrlMonth::AddOneRecord(int monthNum)
{	
	m_ArrMonth.Add(monthNum);
	m_ArrDay.Add(-1);
	m_ArrRunTime.Add("");
	m_ArrInIndex.Add(m_ArrMonth.GetSize()-1);

	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	m_ArrOffsetSE.Add(pMCtrlOffsetSE);
}
void MCtrlMonth::ResetCtrlData()
{
	delAllCtrlData();
	AddOneRecord(0);
	AddOneRecord(1);
	AddOneRecord(2);
	AddOneRecord(3);
	AddOneRecord(4);
	AddOneRecord(5);
	AddOneRecord(6);
	AddOneRecord(7);
	AddOneRecord(8);
	AddOneRecord(9);
	AddOneRecord(10);
	AddOneRecord(11);
}
void MCtrlMonth::SaveCtrlDataToDB()
{
	DelCtrlDataIoDB();
	////打开表记录，然后
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_month"); //写信息到模块列表数据表
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
		if(m_ArrRunTime[i]=="")
			continue;
		pRecord->AddNew();
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //放Name WeekDay
		pRecord->PutCollect(_T("MonthName"),_variant_t((short)m_ArrMonth[i])); //放
		pRecord->PutCollect(_T("DayNum"),_variant_t((short)m_ArrDay[i])); //放
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
void MCtrlMonth::DelCtrlDataIoDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol_month where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}
}

void MCtrlMonth::ReadCtrlData()
{
	delAllCtrlData();
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM mode_runcontrol_month where Mode_Name='%s'",this->m_strModelName);
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
		CString MonthName="";
		CString DayNum="";
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
			Tempvalue = pRecord->GetCollect("MonthName");
			if(Tempvalue.vt != VT_NULL) MonthName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("DayNum");
			if(Tempvalue.vt != VT_NULL) DayNum = (LPCSTR)_bstr_t(Tempvalue);
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
			this->m_ArrMonth.Add(atoi(MonthName));
			this->m_ArrDay.Add(atoi(DayNum));
			this->m_ArrRunTime.Add(Time);
			this->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
			this->m_ArrInIndex.Add(m_ArrMonth.GetSize()-1);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
void MCtrlMonth::delAllCtrlData()
{
	m_ArrInIndex.RemoveAll();
	m_ArrMonth.RemoveAll();
	m_ArrDay.RemoveAll();
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