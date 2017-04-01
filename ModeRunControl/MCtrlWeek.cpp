// MCtrlWeek.cpp: implementation of the MCtrlWeek class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlWeek.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlWeek::MCtrlWeek(CString modelName):MCtrlType(modelName)
{
	ResetCtrlData();
}

MCtrlWeek::~MCtrlWeek()
{
	delAllCtrlData();
}
void MCtrlWeek::DelRecordByInIndex(int inindex)
{
	MCtrlOffsetSE *pMCtrlOffsetSE;
	for(int i=0;i<this->m_ArrInIndex.GetSize();i++)
	{
		if(m_ArrInIndex[i]==inindex)
		{
			if(beLaseOneRecord(m_ArrWeek[i]))
			{
				pMCtrlOffsetSE=	m_ArrOffsetSE[i];
				pMCtrlOffsetSE->ResetCtrlData();
				m_ArrRunTime[i]="";
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
				m_ArrWeek.RemoveAt(i);
			}
		}
	}
}
BOOL MCtrlWeek::beLaseOneRecord(int wNum)
{
	int count=0;
	for(int i=0;i<m_ArrWeek.GetSize();i++)
	{
		if(m_ArrWeek[i]==wNum)
			count++;
	}
	if(count>1)
		return false;
	else
		return true;
}

void MCtrlWeek::GetTheRecordDataByInIndex(int inIndex,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE)
{
	pStrTime=&(m_ArrRunTime[inIndex]);
	pMCtrlOffsetSE=m_ArrOffsetSE[inIndex];
}
void MCtrlWeek::GetTheWeekData(int wInNum,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE)
{
	bool beFind=false;
	for(int i=0;i<m_ArrWeek.GetSize();i++)
	{
		int wNum=m_ArrWeek[i];
		CString strTime=m_ArrRunTime[i];
		if((wInNum==wNum)&&(strTime==""))
		{
			beFind=true;
			pStrTime=&(m_ArrRunTime[i]);
			pMCtrlOffsetSE=m_ArrOffsetSE[i];
			break;
		}
	}
	if(!beFind)
	{
		AddOneRecord(wInNum);
		pStrTime=&(m_ArrRunTime[m_ArrRunTime.GetSize()-1]);
		pMCtrlOffsetSE=m_ArrOffsetSE[m_ArrOffsetSE.GetSize()-1];
	}
}
void MCtrlWeek::SetNewWeekNums(CArray<int,int> &arrWeekArr)
{	
	CArray<int,int> needBeDelWNum;
	for(int i=0;i<m_ArrWeek.GetSize();i++)//找需要删除的星期
	{
		int wNum=m_ArrWeek[i];
		bool beFind=false;
		for(int j=0;j<arrWeekArr.GetSize();j++)
		{
			int wNewWeek=arrWeekArr[j];
			if(wNum==wNewWeek)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			needBeDelWNum.Add(wNum);
	}
	for(i=0;i<needBeDelWNum.GetSize();i++)
	{
		DelRecordByWNum(needBeDelWNum[i]);
	}
	for(i=0;i<arrWeekArr.GetSize();i++)//检查是否有新增星期，若有增加记录
	{
		int wNewWeek=arrWeekArr[i];
		bool beFind=false;
		for(int j=0;j<m_ArrWeek.GetSize();j++)
		{
			int wNum=m_ArrWeek[j];
			if(wNum==wNewWeek)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			AddOneRecord(wNewWeek);
	}
}
void MCtrlWeek::GetWeekNums(CArray<int,int> &arrWeekArr)
{
	arrWeekArr.RemoveAll();
	for(int i=0;i<m_ArrWeek.GetSize();i++)
	{
		int wNum=m_ArrWeek[i];
		bool beFind=false;
		for(int j=0;j<arrWeekArr.GetSize();j++)
		{
			int getWNum=arrWeekArr[j];
			if(getWNum==wNum)
			{
				beFind=true;
				break;
			}
		}
		if(!beFind)
			arrWeekArr.Add(wNum);
	}
	for(i=0;i<arrWeekArr.GetSize();i++)
	{
		for(int j=i+1;j<arrWeekArr.GetSize();j++)
		{
			if(arrWeekArr[i]>arrWeekArr[j])
			{
				int temp=arrWeekArr[i];
				arrWeekArr[i]=arrWeekArr[j];
				arrWeekArr[j]=temp;
			}
		}
	}
}
void MCtrlWeek::DelRecordByWNum(int weekNum)
{
	for(int i=m_ArrInIndex.GetSize()-1;i>=0;i--)
	{
		if(m_ArrWeek[i]==weekNum)
		{
			m_ArrWeek.RemoveAt(i);
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
void MCtrlWeek::AddOneRecord(int weekNum)
{
	m_ArrWeek.Add(weekNum);
	m_ArrRunTime.Add("");
	MCtrlOffsetSE *pMCtrlOffsetSE=new MCtrlOffsetSE;
	m_ArrOffsetSE.Add(pMCtrlOffsetSE);
	
	m_ArrInIndex.Add(m_ArrWeek.GetSize()-1);//存储当前的索引号
}
void MCtrlWeek::ResetCtrlData()
{
	delAllCtrlData();
	AddOneRecord(0);//默认只有周一
}
void MCtrlWeek::delAllCtrlData()
{
	m_ArrInIndex.RemoveAll();
	m_ArrWeek.RemoveAll();
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
void MCtrlWeek::SaveCtrlDataToDB()
{
	DelCtrlDataIoDB();
	////打开表记录，然后
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_week"); //写信息到模块列表数据表
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
		pRecord->PutCollect(_T("WeekDay"),_variant_t((short)m_ArrWeek[i])); //放
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
void MCtrlWeek::DelCtrlDataIoDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol_week where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}	
}
void MCtrlWeek::ReadCtrlData()
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM mode_runcontrol_week where Mode_Name='%s'",this->m_strModelName);
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
		CString WeekDay="";
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
			Tempvalue = pRecord->GetCollect("WeekDay");
			if(Tempvalue.vt != VT_NULL) WeekDay = (LPCSTR)_bstr_t(Tempvalue);
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
			this->m_ArrWeek.Add(atoi(WeekDay));
			this->m_ArrRunTime.Add(Time);
			this->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
			m_ArrInIndex.Add(m_ArrWeek.GetSize()-1);//存储当前的索引号
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}
