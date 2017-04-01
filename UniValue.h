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
	//����ֵ���й���
	//CUniValue(double value);
	//CUniValue(double value,long ltime);

	virtual ~CUniValue();
	
public:
	CUniValue& operator =( const CUniValue& RightValue);
public:
	double GetValueDouble();
	void AddStatData(const StatData &statData);//����ͳ������
	void AddData(double dvalue,long lTime=0,short state=0,long lrealTime=0);
	void InitValue();
	//����
	int GetValueType();
	void SetValueType(int type);
	 //������ݵĺ���
	
    long  GetDataSize(); //����������ĳ���
	long  GetStatDataSize();//��ȡͳ�����ݵĳ���
	
	int GetDoubleVal(double* pDouble);
	int GetTimeVal(long* pLong);
	int GetRealTimeVal(long* result);
	int GetStatusVal(short* pShort);
	int GetDoubleValAt(long index,double* pDouble);
	int  GetTimeValAt(long index,long* pTime);
	int  GetRealTimeValAt(long index,long* pTime);
	int GetStatusValAt(long index,short* result);

	int GetStatData(StatData* pStatData);//��ȡͳ������
	int GetStatDataAt(long index,StatData* pStatData);//��ȡָ��ͳ������
	//
protected:
	void DeleteValuePrt();
	int m_type;

	long   m_lSize; //   ��ǰ��ʵ�����ݴ�С
	long   m_lBufSize; //�Ѿ�����Ŀռ�
	long   m_lBufIncSize; //ÿ�λ��������ĳ���ֵ

	long*       m_plTimeData;
	long*       m_plrealTimeData;
	double*  m_pdValueData;
	short*  m_pStatusData;
	std::vector<StatData> m_StatDataArr;
};

#endif // !defined(AFX_UNIVALUE_H__9A703009_36E6_4622_95CA_F70E43126D4E__INCLUDED_)
