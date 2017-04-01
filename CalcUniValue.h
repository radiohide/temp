// CalcUniValue.h: interface for the CCalcUniValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCUNIVALUE_H__C2146166_CC1F_4C66_A4E3_CB7D71C8C596__INCLUDED_)
#define AFX_CALCUNIVALUE_H__C2146166_CC1F_4C66_A4E3_CB7D71C8C596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//����һ�������ɶ����������͵���ֵ����
#include "DataBuffer.h"
#include "StdAfx.h"	// Added by ClassView

class CCalcUniValue:public CObject  
{
public:
	CCalcUniValue();
	CCalcUniValue(int type);
	//copy consturct
	CCalcUniValue(const CCalcUniValue& value);
	virtual ~CCalcUniValue();
public:
	TagData GetRTPointValue();
	double GetDoubleValue();
	long   GetLongTimeValue();
	void SetDoubleValue(double value);

	CDataBuffer* GetRTBufferPtr();
	void AddData(double value,long ltime=0,int state=1);
	void SetValueType(int type);
    CCalcUniValue& operator =(const CCalcUniValue& RightValue);
	//CCalcUniValue operator +(CCalcUniValue&);
	//CCalcUniValue operator -(CCalcUniValue&);
	/////////////////����///////////////////////
	int m_type;    //���ݵ�����
	/////////////////����///////////////////////
	void InitDataType();
protected:
	
	void DeleteValuePrt();
	//�洢��ָ��
    double   *m_pdValue;  // to VALUE_DOUBLE
	TagData  *m_pRTValue; // TO VALUE_RTVALUE;
	CDataBuffer* m_pRTDataBuffer; // to rt array data
	
};

#endif // !defined(AFX_CALCUNIVALUE_H__C2146166_CC1F_4C66_A4E3_CB7D71C8C596__INCLUDED_)
