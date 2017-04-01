// MCtrlDuty.cpp: implementation of the MCtrlDuty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "MCtrlDuty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MCtrlDuty::MCtrlDuty(CString modelName):MCtrlType(modelName)
{
	tagBeReadAllDuty=false;
}

MCtrlDuty::~MCtrlDuty()
{
	delAllCtrlData();
}
void MCtrlDuty::ResetCtrlData()
{
	m_ArrDutyNum.RemoveAll();
	m_ArrDutySOrE.RemoveAll();
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
void MCtrlDuty::SaveCtrlDataToDB()
{
	DelCtrlDataIoDB();//先删除记录

	////打开表记录，然后
	CString strSQL;
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol_duty"); //写信息到模块列表数据表
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
	for(int i=0;i<this->m_ArrDutyNum.GetSize();i++)
	{
		pRecord->AddNew();
		pMCtrlOffsetSE=m_ArrOffsetSE[i];
		pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //放Name
		pRecord->PutCollect(_T("Duty_Name"),_variant_t(m_ArrDutyNum[i])); //放DESP
		pRecord->PutCollect(_T("Duty_SOrE"),_variant_t((short)m_ArrDutySOrE[i])); //放DESP
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
void MCtrlDuty::DelCtrlDataIoDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol_duty where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}
}
void MCtrlDuty::ReadCtrlData()
{
	ResetCtrlData();
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM mode_runcontrol_duty where Mode_Name='%s'",this->m_strModelName);
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
		CString Duty_Name="";
		CString Duty_SOrE="";
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
			Tempvalue = pRecord->GetCollect("Duty_Name");
			if(Tempvalue.vt != VT_NULL) Duty_Name = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("Duty_SOrE");
			if(Tempvalue.vt != VT_NULL) Duty_SOrE = (LPCSTR)_bstr_t(Tempvalue);

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
			this->m_ArrDutyNum.Add(Duty_Name);
			m_ArrDutySOrE.Add(atoi(Duty_SOrE));
			this->m_ArrOffsetSE.Add(pMCtrlOffsetSE);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
}

void MCtrlDuty::ReadAllDuty()
{
	if(tagBeReadAllDuty==true)
		return;
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//检索到所有类型
	strSQL.Format("SELECT * FROM dutytime");
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	m_ArrAllDuty.RemoveAll();
	while(!pRecord->adoEOF)
	{
		CString DUTY_NAME;
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("DUTY_NAME");
			if(Tempvalue.vt != VT_NULL) DUTY_NAME = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			m_ArrAllDuty.Add(DUTY_NAME);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();
	tagBeReadAllDuty=true;
}
void MCtrlDuty::delAllCtrlData()
{
	ResetCtrlData();
}