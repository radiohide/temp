// CalcUniValue.cpp: implementation of the CCalcUniValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "CalcUniValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCalcUniValue::CCalcUniValue()
{
	 m_pdValue=NULL;
	 m_pRTValue=NULL;
	 m_pRTDataBuffer=NULL;
	 m_type = 1;
	 InitDataType();
}

CCalcUniValue::CCalcUniValue(int type)
{
	 m_pdValue=NULL;
	 m_pRTValue=NULL;
	 m_pRTDataBuffer=NULL;

	 m_type = type;
	 InitDataType();
}

CCalcUniValue::CCalcUniValue(const CCalcUniValue& value1)
{
	DeleteValuePrt();
	*this = value1;
}

CCalcUniValue::~CCalcUniValue()
{
	DeleteValuePrt();
}

CCalcUniValue& CCalcUniValue::operator =(const CCalcUniValue& RightValue)
{
	CCalcUniValue* result = new CCalcUniValue (RightValue.m_type);

	switch(result->m_type)
	{
	case VALUE_DOUBLE:
		 {
			 *result->m_pdValue  = *RightValue.m_pdValue;
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 result->m_pRTValue->value =  RightValue.m_pRTValue->value  ;
			 result->m_pRTValue->time =  RightValue.m_pRTValue->time  ;
			 result->m_pRTValue->status =  RightValue.m_pRTValue->status  ;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 /*CDataBuffer* m_pRTDataBuffer_temp = new CDataBuffer;
			 (*m_pRTDataBuffer_temp) = (*RightValue.m_pRTDataBuffer) ;
			 delete m_pRTDataBuffer;
             m_pRTDataBuffer = m_pRTDataBuffer_temp;*/
			 break;
		 }
	}
	return *result;

}

void CCalcUniValue::InitDataType()
{
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 if(m_pdValue!=NULL) { delete m_pdValue;m_pdValue=NULL;}
			 m_pdValue = new double[1];
			 (*m_pdValue) =0;
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 m_pRTValue = new TagData;
			 m_pRTValue->value =0;
			 m_pRTValue->status=1;
			 m_pRTValue->time = 0;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 m_pRTDataBuffer = new CDataBuffer;
			 m_pRTDataBuffer->InitialData();
			 break;
		 }
	}
   
}
void CCalcUniValue::DeleteValuePrt()
{
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 delete m_pdValue;
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 delete m_pRTValue;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 delete m_pRTDataBuffer;
			 break;
		 }
	}


}

void CCalcUniValue::SetValueType(int type)
{
	//DeleteValuePrt();
	m_type = type;
    InitDataType();
}

void CCalcUniValue::AddData(double value, long ltime, int state)
{
    switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 (*m_pdValue) =value;
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 m_pRTValue->value =value;
			 m_pRTValue->status=state;
			 m_pRTValue->time = ltime;
			/* m_pRTValue->value =value;
			 m_pRTValue->status=state;
			 m_pRTValue->time = ltime;*/
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 m_pRTDataBuffer->AddData(ltime,value);
			 break;
		 }
	}
}

TagData CCalcUniValue::GetRTPointValue()
{
	 TagData rt;
     if(m_type==VALUE_DOUBLE)
	 {
		 rt.value  = m_pRTValue->value;
		 rt.time = m_pRTValue->time;
		 rt.status = m_pRTValue->status;
	 }
	 return rt;
}
void CCalcUniValue::SetDoubleValue(double value)
{
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 (*m_pdValue)=value;
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 m_pRTValue->value = value;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 //value = m_pRTDataBuffer->GetDataValueAt(0);
			 break;
		 }
	}
}
double CCalcUniValue::GetDoubleValue()
{
	double value;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 value = (*m_pdValue);
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 value = m_pRTValue->value;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 if(m_pRTDataBuffer->GetDataSize()>0)
			    value = m_pRTDataBuffer->GetDataValueAt(0);
			 break;
		 }
	}
	return value;
}
long   CCalcUniValue::GetLongTimeValue()
{
	long value=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 value = m_pRTValue->time ;
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 if(m_pRTDataBuffer->GetDataSize()>0) value = m_pRTDataBuffer->GetDataLongTime(0);
			 break;
		 }
	}
	return value;
}
CDataBuffer* CCalcUniValue::GetRTBufferPtr()
{
	return m_pRTDataBuffer;
}

