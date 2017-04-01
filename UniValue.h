// UniValue.h: interface for the CUniValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIVALUE_H__9A703009_36E6_4622_95CA_F70E43126D4E__INCLUDED_)
#define AFX_UNIVALUE_H__9A703009_36E6_4622_95CA_F70E43126D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
/*//////////
 ////////////////////////////////
	CUniValue var1(VALUE_DOUBLE);
	var1.AddData(120.0);
	var1.AddData(190.0);

	CUniValue var2(var1);
	double aa = var2.GetValueDouble();
	////////////////////////////////
////////*/
class CUniValue : public CObject  
{
public:
	CUniValue();
	CUniValue(const CUniValue& value);
	CUniValue(int type);
	//用数值进行构造
	//CUniValue(double value);
	//CUniValue(double value,long ltime);

	virtual ~CUniValue();
	
public:
	CUniValue& operator =( const CUniValue& RightValue);
public:
	double GetValueDouble();
	void AddStatData(const StatData &statData);//加入统计数据
	void AddData(double dvalue,long lTime=0,short state=0,long lrealTime=0);
	void InitValue();
	//类型
	int GetValueType();
	void SetValueType(int type);
	 //获得数据的函数
	
    long  GetDataSize(); //获得数据区的长度
	long  GetStatDataSize();//获取统计数据的长度
	
	int GetDoubleVal(double* pDouble);
	int GetTimeVal(long* pLong);
	int GetRealTimeVal(long* result);
	int GetStatusVal(short* pShort);
	int GetDoubleValAt(long index,double* pDouble);
	int  GetTimeValAt(long index,long* pTime);
	int  GetRealTimeValAt(long index,long* pTime);
	int GetStatusValAt(long index,short* result);

	int GetStatData(StatData* pStatData);//获取统计数据
	int GetStatDataAt(long index,StatData* pStatData);//获取指定统计数据
	//
protected:
	void DeleteValuePrt();
	int m_type;

	long   m_lSize; //   当前的实际数据大小
	long   m_lBufSize; //已经分配的空间
	long   m_lBufIncSize; //每次缓存增长的长度值

	long*       m_plTimeData;
	long*       m_plrealTimeData;
	double*  m_pdValueData;
	short*  m_pStatusData;
	std::vector<StatData> m_StatDataArr;
};

#endif // !defined(AFX_UNIVALUE_H__9A703009_36E6_4622_95CA_F70E43126D4E__INCLUDED_)
