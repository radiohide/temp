// TimeOffsetBlock.cpp: implementation of the CTimeOffsetBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "TimeOffsetBlock.h"
#include "PropertyDlgTimeOffset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeOffsetBlock::CTimeOffsetBlock()
{
	CTime t(2012,1,1,0,0,0);

	m_iOffsetType= 0;//0:��;1Сʱ;2:��;3:��;4:��;5:��;
	m_iSecond=3600;//��

	m_iHour=0;//Сʱ
	m_HTime=t.GetTime();//Сʱ-ʱ��

	m_iDay=0;//��
	m_DTime=t.GetTime();//��-ʱ��

	m_iWeek=0;//��
	m_iWeekDay=0;//��-��?
	m_WTime=t.GetTime();//��-ʱ��

	
	m_iMonth=0;//��
	m_iMonthDay=1;//��-��?
	m_MTime=t.GetTime();//��-ʱ��
	
	m_iYear=0;//��
	m_iYearMonth=1;//��-��?
	m_iYearDay=1;//��-��?
	m_YTime=t.GetTime();//��-ʱ��

	SetBlockType(_T( "TimeOffset" ) );
	ConstructBlockInputOutput();
}

CTimeOffsetBlock::~CTimeOffsetBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CTimeOffsetBlock::CreateFromString( const CString& strType )
{
	CTimeOffsetBlock* obj = new CTimeOffsetBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CTimeOffsetBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("InTime"), VALUE_DOUBLE);
	AddOutputPort("OutTime",VALUE_DOUBLE,"out");
}
//��ʼ������ĺ���������ĳ�ʼ��
void CTimeOffsetBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CTimeOffsetBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInPortObj = GetInputPortObj(0);
	long lInTime;
	long lOutTime=0;
	double dValue=0;
	pInPortObj->GetPortUniValue().GetDoubleVal(&dValue);
	lInTime=(long)dValue;
	CTime tNoWrong(1990,1,1,0,0,0);
	if(lInTime<tNoWrong.GetTime())//����ʱ�䲻��С��1990,1,1,0,0,0
	{
		lInTime=tNoWrong.GetTime();
	}
	if(m_iOffsetType==0)//��ƫ��
	{
		lOutTime=lInTime-m_iSecond;
	}
	else if(m_iOffsetType==1)//Сʱƫ��
	{
		CTimeSpan ts(0,m_iHour,0,0);
		lInTime-=ts.GetTotalSeconds();
		CTime inTimeSubedTime(lInTime);
		CTime outTime(inTimeSubedTime.GetYear(),inTimeSubedTime.GetMonth(),
			inTimeSubedTime.GetDay(),inTimeSubedTime.GetHour(),
			m_HTime.GetMinute(),m_HTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==2)//��ƫ��
	{
		CTimeSpan ts(m_iDay,0,0,0);
		long lLength=ts.GetTotalSeconds();
		lInTime-=lLength;
		CTime inTimeSubedDay(lInTime);
		CTime outTime(inTimeSubedDay.GetYear(),inTimeSubedDay.GetMonth(),
			inTimeSubedDay.GetDay(),m_DTime.GetHour(),
			m_DTime.GetMinute(),m_DTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==3)//��ƫ��
	{
		CTimeSpan ts(m_iWeek*7,0,0,0);
		long lLength=ts.GetTotalSeconds();
		lInTime-=lLength;
		CTime inTimeSubed(lInTime);
		int currWeek=inTimeSubed.GetDayOfWeek();//����=1,��1=2,...
		int setWeek=m_iWeekDay+1;
		if(setWeek>currWeek)
		{
			CTimeSpan ts2(setWeek-currWeek,0,0,0);//
			inTimeSubed+=ts2;
		}
		else if(setWeek<currWeek)
		{
			CTimeSpan ts2(currWeek-setWeek,0,0,0);//
			inTimeSubed-=ts2;
		}
		CTime outTime(inTimeSubed.GetYear(),inTimeSubed.GetMonth(),
			inTimeSubed.GetDay(),m_WTime.GetHour(),
			m_WTime.GetMinute(),m_WTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==4)//��ƫ��
	{
		CTime inTime(lInTime);
		int inYear=inTime.GetYear();
		int inMonth=inTime.GetMonth();
		if(inMonth<=m_iMonth)//
		{	
			inYear-=m_iMonth/12;
			int iLeftMonth=m_iMonth%12;
			if(inMonth<=iLeftMonth)
			{
				inYear-=1;
				inMonth=12-(iLeftMonth-inMonth);
			}
			else
			{
				inMonth-=iLeftMonth;
			}
		}
		else
		{
			inMonth-=m_iMonth;
		}
		int iDay=getDayOfTheMonth(inYear,inMonth,m_iMonthDay+1);

		CTime outTime(inYear,inMonth,iDay,
			m_MTime.GetHour(),m_MTime.GetMinute(),m_MTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==5)//��ƫ��
	{
		CTime inTime(lInTime);
		int inYear=inTime.GetYear();
		int iSubYear=inYear-m_iYear;
		int iDay=getDayOfTheMonth(iSubYear,m_iYearMonth,m_iYearDay+1);
		CTime outTime(iSubYear,m_iYearMonth+1,iDay,
			m_YTime.GetHour(),m_YTime.GetMinute(),m_YTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	CTime t(lOutTime);
	CString str;
	str=t.Format("%Y-%m-%d %H:%M:%S %W-%A"); 
	//AfxMessageBox(str);

	CUniValue UniVar;
	UniVar.AddData(lOutTime);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
int CTimeOffsetBlock::getDayOfTheMonth(int iYear,int iMonth,int iDayIn)
{
	int iDay=0;
	if((iMonth==1)||(iMonth==3)||(iMonth==5)||(iMonth==7)||(iMonth==8)||(iMonth==10)||(iMonth==12))
	{
		iDay=iDayIn;
	}
	else if((iMonth==4)||(iMonth==6)||(iMonth==9)||(iMonth==11))
	{
		if(iDayIn==31)
		{
			iDay=30;
		}
		else
		{
			iDay=iDayIn;
		}
	}
	else//2��
	{
		int iTagRunNian=iYear%4;
		if(iTagRunNian==0)//����
		{
			if(iDayIn<=29)
			{
				iDay=iDayIn;
			}
			else
			{
				iDay=29;
			}
		}
		else
		{
			if(iDayIn<=28)
			{
				iDay=iDayIn;
			}
			else
			{
				iDay=28;
			}
		}
	}
	return iDay;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CTimeOffsetBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Type")==0)
	{
		if(!strItem1.IsEmpty())  {m_iOffsetType=atoi(strItem1);}
	}
	else if(strPropName.Compare("SecondAndDay")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSecond=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_DTime=CTime(atoi(strItem3));}
	}
	else if(strPropName.Compare("HourAndWeek")==0)
	{
		if(!strItem1.IsEmpty())  {m_iHour=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_HTime=CTime(atoi(strItem2));}

		if(!strItem3.IsEmpty())  {m_iWeek=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iWeekDay=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_WTime=CTime(atoi(strItem5));}
	}
	else if(strPropName.Compare("Month")==0)
	{
		if(!strItem1.IsEmpty())  {m_iMonth=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iMonthDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_MTime=CTime(atoi(strItem3));}
	}
	else if(strPropName.Compare("Year")==0)
	{
		if(!strItem1.IsEmpty())  {m_iYear=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iYearDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iYearMonth=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_YTime=CTime(atoi(strItem4));}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CTimeOffsetBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgTimeOffset dlg;
	dlg.m_iOffsetType=m_iOffsetType;//0:��;1Сʱ;2:��;3:��;4:��;5:��;
	dlg.m_iSecond=m_iSecond;//��
	dlg.m_iHour=m_iHour;//Сʱ
	dlg.m_HTime=m_HTime;//Сʱ-ʱ��
	dlg.m_iDay=m_iDay;//��
	dlg.m_DTime=m_DTime;//��-ʱ��
	dlg.m_iWeek=m_iWeek;//��
	dlg.m_iWeekDay=m_iWeekDay;//��-��
	dlg.m_WTime=m_WTime;//��-ʱ��	
	dlg.m_iMonth=m_iMonth;//��
	dlg.m_iMonthDay=m_iMonthDay;//��-��
	dlg.m_MTime=m_MTime;//��-ʱ��	
	dlg.m_iYear=m_iYear;//��
	dlg.m_iYearDay=m_iYearDay;//��-��
	dlg.m_iYearMonth=m_iYearMonth;//��-��
	dlg.m_YTime=m_YTime;//��-ʱ��	
	if(dlg.DoModal()==IDOK)
	{
		m_iOffsetType=dlg.m_iOffsetType;//0:��;1Сʱ;2:��;3:��;4:��;5:��;
		m_iSecond=dlg.m_iSecond;//��
		m_iHour=dlg.m_iHour;//Сʱ
		m_HTime=dlg.m_HTime;//Сʱ-ʱ��
		m_iDay=dlg.m_iDay;//��
		m_DTime=dlg.m_DTime;//��-ʱ��
		m_iWeek=dlg.m_iWeek;//��
		m_iWeekDay=dlg.m_iWeekDay;//��-��
		m_WTime=dlg.m_WTime;//��-ʱ��	
		m_iMonth=dlg.m_iMonth;//��
		m_iMonthDay=dlg.m_iMonthDay;//��-��
		m_MTime=dlg.m_MTime;//��-ʱ��	
		m_iYear=dlg.m_iYear;//��
		m_iYearDay=dlg.m_iYearDay;//��-��
		m_iYearMonth=dlg.m_iYearMonth;//��-��
		m_YTime=dlg.m_YTime;//��-ʱ��	

		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CTimeOffsetBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="Type";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iOffsetType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SecondAndDay";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iSecond); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iDay); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_DTime.GetTime()); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="HourAndWeek";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iHour); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_HTime.GetTime()); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iWeek); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iWeekDay); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_WTime.GetTime()); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="Month";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iMonth); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iMonthDay); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_MTime.GetTime()); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="Year";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iYear); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iYearDay); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iYearMonth); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_YTime.GetTime()); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CTimeOffsetBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iOffsetType=((CTimeOffsetBlock*)pBlock)->m_iOffsetType;//0:��;1Сʱ;2:��;3:��;4:��;5:��;
	m_iSecond=((CTimeOffsetBlock*)pBlock)->m_iSecond;//��
	m_iHour=((CTimeOffsetBlock*)pBlock)->m_iHour;//Сʱ
	m_HTime=((CTimeOffsetBlock*)pBlock)->m_HTime;//Сʱ-ʱ��
	m_iDay=((CTimeOffsetBlock*)pBlock)->m_iDay;//��
	m_DTime=((CTimeOffsetBlock*)pBlock)->m_DTime;//��-ʱ��
	m_iWeek=((CTimeOffsetBlock*)pBlock)->m_iWeek;//��
	m_iWeekDay=((CTimeOffsetBlock*)pBlock)->m_iWeekDay;//��-��
	m_WTime=((CTimeOffsetBlock*)pBlock)->m_WTime;//��-ʱ��	
	m_iMonth=((CTimeOffsetBlock*)pBlock)->m_iMonth;//��
	m_iMonthDay=((CTimeOffsetBlock*)pBlock)->m_iMonthDay;//��-��
	m_MTime=((CTimeOffsetBlock*)pBlock)->m_MTime;//��-ʱ��	
	m_iYear=((CTimeOffsetBlock*)pBlock)->m_iYear;//��
	m_iYearDay=((CTimeOffsetBlock*)pBlock)->m_iYearDay;//��-��
	m_iYearMonth=((CTimeOffsetBlock*)pBlock)->m_iYearMonth;//��-��
	m_YTime=((CTimeOffsetBlock*)pBlock)->m_YTime;//��-ʱ��	
}
//�����Է��ô���������
void CTimeOffsetBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Type");
	
	CString str;
	str.Format("%d",m_iOffsetType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SecondAndDay");
	
	str.Format("%d",m_iSecond);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_DTime.GetTime());
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"HourAndWeek");
	
	str.Format("%d",m_iHour);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_HTime.GetTime());
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iWeek);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iWeekDay);
	strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_WTime.GetTime());
	strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Month");
	
	str.Format("%d",m_iMonth);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iMonthDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_MTime.GetTime());
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Year");
	
	str.Format("%d",m_iYear);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iYearDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iYearMonth);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_YTime.GetTime());
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}