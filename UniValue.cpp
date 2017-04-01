// UniValue.cpp: implementation of the CUniValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
//#include "CalcServ.h"
#include "UniValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUniValue::CUniValue()
{
	m_type = VALUE_DOUBLE;
	//////////////////////////////////////
    m_pdValueData=NULL;
	m_plTimeData=NULL;
	m_plrealTimeData=NULL;
	m_pStatusData=NULL;
	m_lSize=0;//  ��ǰ��ʵ�����ݴ�С
	m_lBufSize=0;//������Ŀռ�
	m_lBufIncSize=1;//�λ��������ĳ���ֵ
}
CUniValue::CUniValue(int type)
{
	m_type = type;
   m_pdValueData=NULL;
	m_plTimeData=NULL;
	m_plrealTimeData=NULL;
	m_pStatusData=NULL;
	m_lSize=0;//  ��ǰ��ʵ�����ݴ�С
	m_lBufSize=0;//������Ŀռ�
	m_lBufIncSize=1;//�λ��������ĳ���ֵ

}
/*
CUniValue::CUniValue(int type)
{
	m_type = type;
	m_pdValueData=NULL;
	m_plTimeData=NULL;
	InitValue();
}
CUniValue::CUniValue(double value)
{
	m_type = VALUE_DOUBLE;
	CUniValue(m_type);
	m_pdValueData[0] =value;


}
CUniValue::CUniValue(double value,long ltime)
{
	m_type = VALUE_DOUBLE;
	InitValue();
	m_pdValueData[0] =value;
	m_plTimeData[0] = ltime;
}
*/
CUniValue::~CUniValue()
{
	DeleteValuePrt();
}

CUniValue::CUniValue(const CUniValue& RightValue)
{
	m_pdValueData=NULL;
	m_plTimeData=NULL;
	m_plrealTimeData=NULL;
	m_pStatusData=NULL;
	*this = RightValue;
}

CUniValue& CUniValue::operator =(const CUniValue& RightValue)
{
	/////////////////////////////////////////////////////////////////////////////////
	//���Ƴ����Ա
	m_type = RightValue.m_type;
	m_lSize = RightValue.m_lSize;
	m_lBufSize = RightValue.m_lBufSize;
	m_lBufIncSize = RightValue.m_lBufIncSize;
	//����ͳ�ƶ���
	m_StatDataArr=RightValue.m_StatDataArr;
	//��ʼ���ƶ���
	if(m_lSize>0)
	{
		long*  m_plTimeData_Temp = new long[RightValue.m_lBufSize];
        long*  m_plrealTimeData_Temp = new long[RightValue.m_lBufSize];
		double* m_pdValueData_Temp= new double[RightValue.m_lBufSize];
		short* m_pStatusData_Temp=new short[RightValue.m_lBufSize];

		memcpy(m_pdValueData_Temp,RightValue.m_pdValueData,sizeof(double)*m_lBufSize);
		memcpy(m_plTimeData_Temp,RightValue.m_plTimeData,sizeof(long)*m_lBufSize);
		memcpy(m_plrealTimeData_Temp,RightValue.m_plrealTimeData,sizeof(long)*m_lBufSize);
		memcpy(m_pStatusData_Temp,RightValue.m_pStatusData,sizeof(short)*m_lBufSize);

		delete []m_pdValueData; 
		delete []m_plTimeData;
		delete []m_plrealTimeData;
		delete []m_pStatusData;
		m_pdValueData = m_pdValueData_Temp;
		m_plTimeData = m_plTimeData_Temp;
		m_plrealTimeData = m_plrealTimeData_Temp;
		m_pStatusData=m_pStatusData_Temp;
	}
    return *this;
///////////////////////////////////////////////////////////////////////////////////////////////////////

}


void CUniValue::SetValueType(int type)
{
    m_type = type;
}

int CUniValue::GetValueType()
{
     return m_type;
}

void CUniValue::DeleteValuePrt()
{
	if(m_plTimeData!=NULL) 
	{
		delete[] m_plTimeData;
		m_plTimeData=NULL;
	}
	if(m_plrealTimeData!=NULL) 
	{
		delete[] m_plrealTimeData;
		m_plrealTimeData=NULL;
	}
	if(m_pdValueData!=NULL) 
	{
		delete[] m_pdValueData;
		m_pdValueData=NULL;
	}
	if(m_pStatusData!=NULL)
	{
		delete[] m_pStatusData;
		m_pStatusData=NULL;
	}
	m_StatDataArr.clear();//���ͳ������
}

//�������ͳ�ʼ������,δ�� 2014.7.24  rao
void CUniValue::InitValue()
{
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 if(m_pdValueData!=NULL) { delete m_pdValueData;m_pdValueData=NULL;}
			 m_pdValueData = new double[1];
			 m_pdValueData[0] =0;

			  if(m_plTimeData!=NULL) { delete m_plTimeData;m_plTimeData=NULL;}
			 m_plTimeData = new long[1];
			 m_plTimeData[0] =0;

			 m_lSize = 1; //   ��ǰ��ʵ�����ݴ�С
			 m_lBufSize =1; //�Ѿ�����Ŀռ�
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 if(m_pdValueData!=NULL) { delete m_pdValueData;m_pdValueData=NULL;}
			 m_pdValueData = new double[1];
			 m_pdValueData[0] =0;
			 //
			 if(m_plTimeData!=NULL) { delete m_plTimeData;m_plTimeData=NULL;}
			 m_plTimeData = new long[1];
			 m_plTimeData[0] =0;

			 m_lSize = 1; //   ��ǰ��ʵ�����ݴ�С
			 m_lBufSize =1; //�Ѿ�����Ŀռ�
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 if(m_pdValueData!=NULL) { delete m_pdValueData;m_pdValueData=NULL;}
			 m_pdValueData = new double[1];
			 m_pdValueData[0] =0;
			 //
			 if(m_plTimeData!=NULL) { delete m_plTimeData;m_plTimeData=NULL;}
			 m_plTimeData = new long[1];
			 m_plTimeData[0] =0;

			 m_lSize = 0; //   ��ǰ��ʵ�����ݴ�С
			 m_lBufSize =1; //�Ѿ�����Ŀռ�

			 break;
		 }
	}
}	


///////////////////////////////////////////
long  CUniValue::GetDataSize()//����������ĳ���
{
	return m_lSize;
}
long CUniValue::GetStatDataSize()//��ȡͳ�����ݵĳ���
{
	return m_StatDataArr.size();
}
int CUniValue::GetStatData(StatData* pStatData)
{
	int ret=-1;
	if(m_StatDataArr.size()>0)
	{
		*pStatData=m_StatDataArr[0];
		ret=0;
	}
	return ret;
}

int CUniValue::GetDoubleVal(double* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =0; //����Ϊƥ���
			 *result = m_pdValueData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =1; //����ȫƥ�䣬��Ҳ��
		 	 *result = m_pdValueData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = -1; //���Ͳ�ƥ��
			 *result = m_pdValueData[0];
			 break;
		 }
	}
	return ret;
}
int CUniValue::GetTimeVal(long* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =0; //����Ϊƥ���
			 *result = m_plTimeData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =1; //����ȫƥ�䣬��Ҳ��
		 	 *result = m_plTimeData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = -1; //���Ͳ�ƥ��
			 *result = m_plTimeData[0];
			 break;
		 }
	}
	return ret;
}

int CUniValue::GetRealTimeVal(long* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		{
			ret =0; //����Ϊƥ���
			*result = m_plrealTimeData[0];
			break;
		}
	case VALUE_RTVALUE:
		{
			ret =1; //����ȫƥ�䣬��Ҳ��
			*result = m_plrealTimeData[0];
			break;
		}
	case VALUE_RTVALUE_SERIES:
		{
			ret = -1; //���Ͳ�ƥ��
			*result = m_plrealTimeData[0];
			break;
		}
	}
	return ret;
}

int  CUniValue::GetStatusVal(short* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =1; //����Ϊƥ���
			 *result = m_pStatusData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =0; //����ȫƥ�䣬��Ҳ��
		 	 *result = m_pStatusData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = -1; //���Ͳ�ƥ��
			 *result = m_pStatusData[0];
			 break;
		 }
	}
	return ret;
}
int CUniValue::GetStatDataAt(long index,StatData* pStatData)
{
	int ret=-1;
	if((index>=0)&&(index<m_StatDataArr.size()))
	{
		*pStatData=m_StatDataArr[index];
		ret=0;
	}
	return ret;
}
int CUniValue::GetTimeValAt(long index,long* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =-1; 
			 *result = m_plTimeData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =-2; 
		 	 *result = m_plTimeData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = 0; //����ƥ��
			 if(index>=0 && index<m_lSize)
			     *result = m_plTimeData[index];
			 break;
		 }
	}
	return ret;
}

int CUniValue::GetRealTimeValAt(long index,long* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		{
			ret =-1; 
			*result = m_plTimeData[0];
			break;
		}
	case VALUE_RTVALUE:
		{
			ret =-2; 
			*result = m_plTimeData[0];
			break;
		}
	case VALUE_RTVALUE_SERIES:
		{
			ret = 0; //����ƥ��
			if(index>=0 && index<m_lSize)
				*result = m_plrealTimeData[index];
			break;
		}
	}
	return ret;
}

int CUniValue::GetStatusValAt(long index,short* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =-1; 
			 *result = m_pStatusData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =-2; 
		 	 *result = m_pStatusData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = 0; //����ƥ��
			 if(index>=0 && index<m_lSize)
			     *result = m_pStatusData[index];
			 break;
		 }
	}
	return ret;
}

int  CUniValue::GetDoubleValAt(long index,double* result)
{
	int ret=0;
	switch(m_type)
	{
	case VALUE_DOUBLE:
		 {
			 ret =-1; 
			 *result = m_pdValueData[0];
			 break;
		 }
	case VALUE_RTVALUE:
		 {
			 ret =-2; 
		 	 *result = m_pdValueData[0];
			 break;
		 }
	case VALUE_RTVALUE_SERIES:
		 {
			 ret = 0; //����ƥ��
			 if(index>=0 && index<m_lSize)
			     *result = m_pdValueData[index];
			 break;
		 }
	}
	return ret;
}

void CUniValue::AddStatData(const StatData &statData)
{
	m_StatDataArr.push_back(statData);
}
void CUniValue::AddData(double dvalue,long lTime,short state,long lrealTime)
{
	  if(m_type == VALUE_DOUBLE)
	 {
		  m_lBufIncSize =1;
		  if(m_lSize == m_lBufSize) //��Ҫ���·����ڴ�
		  {
			  double* pNewHistData = new double[m_lSize+m_lBufIncSize] ;
			  long* pNewHistTime = new long[m_lSize+m_lBufIncSize] ;
			  long* pNewHistrealTime = new long[m_lSize+m_lBufIncSize] ;
			  short* pNewHistStatus = new short[m_lSize+m_lBufIncSize] ;

			  memcpy(pNewHistData,m_pdValueData,m_lBufSize);
			  memcpy(pNewHistTime,m_plTimeData,m_lBufSize);
			  memcpy(pNewHistrealTime,m_plrealTimeData,m_lBufSize);
			  memcpy(pNewHistStatus,m_pStatusData,m_lBufSize);
			  if(m_pdValueData != NULL)
				  delete[] m_pdValueData;
			  if(m_plTimeData != NULL)
				  delete[] m_plTimeData;
			  if(m_plrealTimeData != NULL)
				  delete[] m_plrealTimeData;
			  if(m_pStatusData !=NULL)
				  delete[] m_pStatusData;
			  
			  m_pdValueData=pNewHistData;
			  m_plTimeData = pNewHistTime;
              m_plrealTimeData = pNewHistrealTime;
			  m_pStatusData = pNewHistStatus;
			  m_lBufSize += m_lBufIncSize;
			  m_lSize=1;
		  }
		 m_pdValueData[0]  = dvalue;
		 m_plTimeData[0]  = lTime;
		 m_plrealTimeData[0]  = lrealTime;
		 m_pStatusData[0] = state;
	  }
	 else if(m_type == VALUE_RTVALUE_SERIES)
	 {
		 m_lBufIncSize =1000;
		 if(m_lSize == m_lBufSize) //��Ҫ���·����ڴ�
		 {
			 double* pNewHistData = new double[m_lSize+m_lBufIncSize] ;
			 long* pNewHistTime = new long[m_lSize+m_lBufIncSize] ;
			 long* pNewHistrealTime = new long[m_lSize+m_lBufIncSize] ;
			 short* pNewHistStatus = new short[m_lSize+m_lBufIncSize] ;
			 memcpy(pNewHistData,m_pdValueData,m_lBufSize);
			 memcpy(pNewHistTime,m_plTimeData,m_lBufSize);
			 memcpy(pNewHistrealTime,m_plrealTimeData,m_lBufSize);
			 memcpy(pNewHistStatus,m_pStatusData,m_lBufSize);
			 if(m_pdValueData != NULL)
				 delete[] m_pdValueData;
			 if(m_plTimeData != NULL)
				 delete[] m_plTimeData;
			 if(m_plrealTimeData != NULL)
				 delete[] m_plrealTimeData;
			 if(m_pStatusData !=NULL)
				 delete[] m_pStatusData;
			 
			 m_pdValueData=pNewHistData;
			 m_plTimeData = pNewHistTime;
			 m_plrealTimeData = pNewHistrealTime;
			 m_pStatusData = pNewHistStatus;
			 
			 m_lBufSize += m_lBufIncSize;
		 }
		 m_pdValueData[m_lSize]  = dvalue;
		 m_plTimeData[m_lSize]  = lTime;
		 m_plrealTimeData[m_lSize]  = lrealTime;
		 m_pStatusData[m_lSize] = state;
		 m_lSize++;
	 }
	 ////////////////////////////////////
}

double CUniValue::GetValueDouble()
{
	double dRet=0;
	 if(GetDataSize()>0)
	 {
		 dRet = m_pdValueData[0];
	 }
	 return dRet;
}

