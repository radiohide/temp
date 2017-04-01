// DataBuffer.cpp: implementation of the CDataBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataBuffer::CDataBuffer()
{
    m_lSize = 0; //   ��ǰ��ʵ�����ݴ�С
	m_lBufSize =0; //�Ѿ�����Ŀռ�
	m_lBufIncSize=1000;
	m_pDNAHistData = NULL; 
	m_dmaxvalue = 0 ;
	m_dminvalue = 0 ;
	m_dSumValue = 0;
}
CDataBuffer::CDataBuffer(const CDataBuffer& buffer)
{
    m_lSize = buffer.m_lSize; //   ��ǰ��ʵ�����ݴ�С
	m_lBufSize =buffer.m_lBufSize; //�Ѿ�����Ŀռ�
	m_lBufIncSize=buffer.m_lBufIncSize;
	
	m_dmaxvalue = buffer.m_dmaxvalue ;
	m_dminvalue = buffer.m_dminvalue ;
	m_dSumValue = buffer.m_dSumValue;

	//m_pDNAHistData = buffer.m_lSizeNULL; 
	m_pDNAHistData = new cpRTVALUE[m_lBufSize] ;
	memcpy(m_pDNAHistData,buffer.m_pDNAHistData ,sizeof(cpRTVALUE)*m_lBufSize);
	
}
CDataBuffer::~CDataBuffer()
{
    InitialData();
}
void CDataBuffer::InitialData() //��������ͳ�ʼ�����ݣ�ÿ���������������ʱӦ�õ���
{
	if(m_pDNAHistData!=NULL)
	{
		delete[] m_pDNAHistData;
	    m_pDNAHistData = NULL;
	}

	m_lSize = 0;
	m_lBufSize =0;
    
	m_dSumValue = 0;

}

CDataBuffer CDataBuffer::operator =(CDataBuffer& inBuffer)
{
	//���������ݿ����������ȥ
	CDataBuffer result;
	result.SetBufferDataSize(inBuffer.GetBufferDataSize());
	//get the result data points
	cpRTVALUE* pResultData =  result.GetDataBufferPtr();
	cpRTVALUE* pInBufferData = inBuffer.GetDataBufferPtr();
	long lCopySize = inBuffer.GetDataSize();
	memcpy(pResultData,pInBufferData,sizeof(cpRTVALUE)*lCopySize);
	return result;
}
/*
CDataBuffer CDataBuffer::operator +(CDataBuffer& buffer1)
{
	CDataBuffer result;
	long size1 = GetDataSize();
	long size2 = buffer1.GetDataSize();
	long size = size1<=size2 ? size1:size2 ;
	result.SetBufferDataSize(size);
	for(long i=0;i<size;i++)
	{
		double value = GetDataValueAt(i) + buffer1.GetDataValueAt(i);
		long ltime = GetDataLongTime(i);
		result.AddData(ltime,value);
	}
	return result;
}
CDataBuffer CDataBuffer::operator -(CDataBuffer& buffer1)
{
	CDataBuffer result;
	long size1 = GetDataSize();
	long size2 = buffer1.GetDataSize();
	long size = size1<=size2 ? size1:size2 ;
	result.SetBufferDataSize(size);
	for(long i=0;i<size;i++)
	{
		double value = GetDataValueAt(i) - buffer1.GetDataValueAt(i);
		long ltime = GetDataLongTime(i);
		result.AddData(ltime,value);
	}
	return result;
}
*/
//����ӵ�ֱ�Ӽ����ݵ������е�
void CDataBuffer::AddData(long time,double data)
{
     if(m_lSize == m_lBufSize) //��Ҫ���·����ڴ�
	 {
          cpRTVALUE* pNewHistData = new cpRTVALUE[m_lSize+m_lBufIncSize] ;
		  memcpy(pNewHistData,m_pDNAHistData,sizeof(cpRTVALUE)*m_lBufSize);
		  if(m_pDNAHistData != NULL)
		     delete[] m_pDNAHistData;

		  m_pDNAHistData=pNewHistData;

		  m_lBufSize += m_lBufIncSize;
	 }
	 m_pDNAHistData[m_lSize].ltime  = time;
     m_pDNAHistData[m_lSize].dValue  = data;
	 if(m_lSize ==0)
	 {
		 m_dmaxvalue = data;
		 m_dminvalue = data;
	 }
	 else
	 {
		 if(m_dmaxvalue < data) m_dmaxvalue = data;
		 if(m_dminvalue > data) m_dminvalue = data;
	 }
	 m_lSize ++;

	 m_dSumValue += data;
	 ////////////////////////////////////
}

double CDataBuffer::GetDataTimeAt(long i)
{
	COleDateTime oleTime(m_pDNAHistData[i].ltime);
	return oleTime.m_dt;
}
long   CDataBuffer::GetDataLongTime(long index)  
{
	return m_pDNAHistData[index].ltime;
}
double CDataBuffer::GetDataValueAt(long i)
{
   return m_pDNAHistData[i].dValue ;
}
void CDataBuffer::SetDataValueAt(long index,double value)
{
	m_pDNAHistData[index].dValue = value;
}
void CDataBuffer::SetBufferDataSize(long size)
{
	if(m_pDNAHistData!=NULL)
	{
		delete[] m_pDNAHistData;
	    m_pDNAHistData = NULL;
	}
	m_lBufSize =size;
	m_pDNAHistData = new cpRTVALUE[m_lBufSize] ;
	m_lSize = 0;
	m_dSumValue = 0;
}

