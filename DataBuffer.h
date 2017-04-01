// DataBuffer.h: interface for the CDataBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABUFFER_H__7C1F9869_57DC_4FA6_9FFB_E76CB2F3D754__INCLUDED_)
#define AFX_DATABUFFER_H__7C1F9869_57DC_4FA6_9FFB_E76CB2F3D754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct cpRTVALUE;

class CDataBuffer :public CObject  
{
public:
	CDataBuffer();
	virtual ~CDataBuffer();
	CDataBuffer(const CDataBuffer& buffer);
	CDataBuffer operator =(CDataBuffer&);
	//CDataBuffer operator +(CDataBuffer&);
	//CDataBuffer operator -(CDataBuffer&);
public:
	//void  AddDataInSeries(long lTime,double dValue);
	void   AddData(long time,double data);
	double GetDataTimeAt(long index);
	double GetDataValueAt(long index);
	void   SetDataValueAt(long index,double value);
    long   GetDataLongTime(long index);
	void InitialData();  //用于清除和初始化数据，每次在重新添加数据时应该调用
	long GetDataSize() {return m_lSize;} 
	long GetBufferDataSize() {return m_lBufSize;} 
	void SetBufferDataSize(long size);
	void SetBufIncSize(long inc) { m_lBufIncSize = inc;}
	double GetMax() {return m_dmaxvalue;}
	double GetMin() {return m_dminvalue;}
	double GetSum() {return m_dSumValue;}

	cpRTVALUE* GetDataBufferPtr() {return m_pDNAHistData;}
//增加一些统计和计算类的功能
	//方差，均值
protected:
    // ×××××××××××××××××××××××××××××××××××××
	long   m_lSize; //   当前的实际数据大小
	long   m_lBufSize; //已经分配的空间
	long   m_lBufIncSize;
	cpRTVALUE* m_pDNAHistData; //新的数据方式
	double m_dmaxvalue;
	double m_dminvalue;
	double m_dSumValue;
};

#endif // !defined(AFX_DATABUFFER_H__7C1F9869_57DC_4FA6_9FFB_E76CB2F3D754__INCLUDED_)
