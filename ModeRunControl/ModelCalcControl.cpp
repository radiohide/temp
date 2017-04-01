// ModelCalcControl.cpp: implementation of the ModelCalcControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "ModelCalcControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ModelCalcControl::ModelCalcControl(CString modelName)
{
	m_strModelName=modelName;
	m_iMode_RunType=-1;

	CTime t(0);
	m_tRunAbs_STime=t;
	m_tRunAbs_ETime=t;
	m_tCalcDataAll_STime=t;
	m_tCalcDataAll_ETime=t;

	MCtrlType *pMCtrl;
	pMCtrl=new MCtrlCycle(m_strModelName);//����
	pMCtrl->SetRunAbsSETimeAndDataAllSETimePtr(&m_tRunAbs_STime,&m_tRunAbs_ETime,&m_tCalcDataAll_STime,&m_tCalcDataAll_ETime);
	m_ArrMCtrl.Add(pMCtrl);
	pMCtrl=new MCtrlDay(m_strModelName);//��
	pMCtrl->SetRunAbsSETimeAndDataAllSETimePtr(&m_tRunAbs_STime,&m_tRunAbs_ETime,&m_tCalcDataAll_STime,&m_tCalcDataAll_ETime);
	m_ArrMCtrl.Add(pMCtrl);
	pMCtrl=new MCtrlWeek(m_strModelName);//��
	pMCtrl->SetRunAbsSETimeAndDataAllSETimePtr(&m_tRunAbs_STime,&m_tRunAbs_ETime,&m_tCalcDataAll_STime,&m_tCalcDataAll_ETime);
	m_ArrMCtrl.Add(pMCtrl);
	pMCtrl=new MCtrlMonth(m_strModelName);//��
	pMCtrl->SetRunAbsSETimeAndDataAllSETimePtr(&m_tRunAbs_STime,&m_tRunAbs_ETime,&m_tCalcDataAll_STime,&m_tCalcDataAll_ETime);
	m_ArrMCtrl.Add(pMCtrl);
	pMCtrl=new MCtrlDuty(m_strModelName);//ֵ
	pMCtrl->SetRunAbsSETimeAndDataAllSETimePtr(&m_tRunAbs_STime,&m_tRunAbs_ETime,&m_tCalcDataAll_STime,&m_tCalcDataAll_ETime);
	m_ArrMCtrl.Add(pMCtrl);
}

ModelCalcControl::~ModelCalcControl()
{
	
}
//��һ�μ��������ʱ��ΪCTime(0);��һ�μ���ĵ�ǰʱ��ΪCTime(0);
//����ֵ0�����������������ڽ�����
//����ֵ1��������һ�ζ�ʱ��
//����ֵ2, ���ݴ������
int ModelCalcControl::GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
									  CTime &nextCalAbsTime,CTime &nextCalDataTime,
									  CTime &nextDataSTime,CTime &nextDataETime)
{
	int ret=-1;
	for(int i=0;i<m_ArrMCtrl.GetSize();i++)
	{
		if(i==m_iMode_RunType)
		{
			ret=m_ArrMCtrl[i]->GetNextCalcTime(curCalAbsTime,curCalDataTime,nextCalAbsTime,nextCalDataTime,nextDataSTime,nextDataETime);
			break;
		}
	}
	return ret;
}
void ModelCalcControl::ReadCtrlData()
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//��������������
	strSQL.Format("SELECT * FROM mode_runcontrol where Mode_Name='%s'",this->m_strModelName);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	if(!pRecord->adoEOF)
	{
		CString Mode_RunType="";
		CString RunAbs_STime="";
		CString RunAbs_ETime="";
		CString CalcDataAll_STime="";
		CString CalcDataAll_ETime="";

		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("Mode_RunType");
			if(Tempvalue.vt != VT_NULL) Mode_RunType = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RunAbs_STime");
			if(Tempvalue.vt != VT_NULL) RunAbs_STime = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RunAbs_ETime");
			if(Tempvalue.vt != VT_NULL) RunAbs_ETime = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("CalcDataAll_STime");
			if(Tempvalue.vt != VT_NULL) CalcDataAll_STime = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("CalcDataAll_ETime");
			if(Tempvalue.vt != VT_NULL) CalcDataAll_ETime = (LPCSTR)_bstr_t(Tempvalue);	
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			this->m_iMode_RunType=atoi(Mode_RunType);
			if(RunAbs_STime!="")
				m_tRunAbs_STime=strTimeToCTime(RunAbs_STime);
			if(RunAbs_ETime!="")
				m_tRunAbs_ETime=strTimeToCTime(RunAbs_ETime);
			if(RunAbs_STime!="")
				m_tCalcDataAll_STime=strTimeToCTime(CalcDataAll_STime);
			if(RunAbs_ETime!="")
				m_tCalcDataAll_ETime=strTimeToCTime(CalcDataAll_ETime);
		}
	}
	pRecord->Close();
	for(int i=0;i<m_ArrMCtrl.GetSize();i++)
	{
		if(i==this->m_iMode_RunType)
		{
			m_ArrMCtrl[i]->ReadCtrlData();
			break;
		}
	}
}

CTime ModelCalcControl::strTimeToCTime(CString strTime)
{
	//ȥ������
	CString strRet=_T("");
	CString strChinese=_T("");
	for(int i=0;i<strTime.GetLength();i++)
	{
		if ((UCHAR)strTime.GetAt(i)<128)
			strRet+=strTime.GetAt(i);
		else
		{
			strChinese+=strTime.GetAt(i);
			i++;  
			strChinese+=strTime.GetAt(i);
		}
	}
	//
	strTime=strRet;
	int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0; 
	if(strTime.Find('/')>=0)
		sscanf(strTime,   "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
	else
		sscanf(strTime,   "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);   
	if(strChinese=="����")
	{
		nHour+=12;//�����ʮ��Сʱ�ƶ���Сʱ����12
	}
	
	CTime   t(nYear,   nMonth,   nDate,   nHour,   nMin,   nSec);
	return t;
}
void ModelCalcControl::SetConPtr(_ConnectionPtr &pCon)
{
	this->pCon=pCon;

	MCtrlType *pMCtrl;
	for(int i=0;i<m_ArrMCtrl.GetSize();i++)
	{
		pMCtrl=m_ArrMCtrl[i];
		if(pMCtrl!=NULL)
		{
			pMCtrl->SetConPtr(this->pCon);
		}
	}
}
CString ModelCalcControl::GetCalcDataSETimeStr()
{
	CString str="";
	if(this->m_tCalcDataAll_STime.GetTime()==0)
	{
		str="�ӵ�ǰʱ�̿�ʼ!";
	}
	else
	{
		CString strS = m_tCalcDataAll_STime.Format("%Y-%m-%d %H:%M:%S");
		if(this->m_tCalcDataAll_ETime.GetTime()==0)
		{
			str.Format("�� %s ʱ�̿�ʼ!",strS);
		}
		else
		{
			CString strE=m_tCalcDataAll_ETime.Format("%Y-%m-%d %H:%M:%S");
			str.Format("�� %s ʱ�̿�ʼ�� %s ʱ�̽���!",strS,strE);
		}
	}
	return str;
}
CString ModelCalcControl::GetMLifeStr()
{
	CString str="";
	if(this->m_tRunAbs_STime.GetTime()==0)
	{
		str="��������!";
	}
	else
	{
		CString strS = m_tRunAbs_STime.Format("%Y-%m-%d %H:%M:%S");
		str.Format("�� %s ʱ������!",strS);
	}
	return str;
}

void ModelCalcControl::SaveCtrlDataToDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}

	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from mode_runcontrol"); //д��Ϣ��ģ���б����ݱ�
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
	pRecord->AddNew();
	
	pRecord->PutCollect(_T("Mode_Name"),_variant_t(this->m_strModelName) ); //��Name
	pRecord->PutCollect(_T("Mode_RunType"),_variant_t((short)this->m_iMode_RunType) ); //��DESP
	pRecord->PutCollect(_T("RunAbs_STime"),_variant_t(this->m_tRunAbs_STime.Format("%Y-%m-%d %H:%M:%S")) ); //��
	pRecord->PutCollect(_T("RunAbs_ETime"),_variant_t(this->m_tRunAbs_ETime.Format("%Y-%m-%d %H:%M:%S")) ); //��
	
	pRecord->PutCollect(_T("CalcDataAll_STime"),_variant_t(this->m_tCalcDataAll_STime.Format("%Y-%m-%d %H:%M:%S")) ); //��
	pRecord->PutCollect(_T("CalcDataAll_ETime"),_variant_t(this->m_tCalcDataAll_ETime.Format("%Y-%m-%d %H:%M:%S")) ); //��
	
	pRecord->Update();
	pRecord->Close();
}
void ModelCalcControl::DelCtrlDataInDB()
{
	CString strSQL;
	strSQL.Format("delete from mode_runcontrol where Mode_Name='%s'",this->m_strModelName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
	}
	for(int i=0;i<m_ArrMCtrl.GetSize();i++)
	{
		m_ArrMCtrl[i]->DelCtrlDataIoDB();
	}
}
void ModelCalcControl::SaveAllCtrlDataToDB()
{
	SaveCtrlDataToDB();
	for(int i=0;i<m_ArrMCtrl.GetSize();i++)
	{
		if(i==this->m_iMode_RunType)
		{
			m_ArrMCtrl[i]->SaveCtrlDataToDB();
		}
		else
		{
			m_ArrMCtrl[i]->DelCtrlDataIoDB();
		}
	}
}
void ModelCalcControl::SetModeRunType(int type)
{
	m_iMode_RunType=type;
}

void ModelCalcControl::SetDefaulControlData()
{
	SetModeRunType(0);
}
