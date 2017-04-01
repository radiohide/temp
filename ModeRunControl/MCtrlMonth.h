// MCtrlMonth.h: interface for the MCtrlMonth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLMONTH_H__683DE1AF_31DE_4F69_AE62_9022F23FA95C__INCLUDED_)
#define AFX_MCTRLMONTH_H__683DE1AF_31DE_4F69_AE62_9022F23FA95C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MCtrlType.h"


class MCtrlMonth : public MCtrlType  
{
public:
	MCtrlMonth(CString modelName);
	virtual ~MCtrlMonth();
	virtual void ReadCtrlData();
	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();

	void GetTheRecordDataByInIndex(int inIndex,int *&pDay,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE);
	void GetTheMonthData(int mNum,int *&pDay,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE);
	void AddOneRecord(int monthNum);
	void GetMonthNums(CArray<int,int> &arrMonthArr);
	void DelRecordByMNum(int monthNum);
	void DelRecordByInIndex(int inindex);
	void SetNewMonthNums(CArray<int,int> &arrMonthArr);
private:
	BOOL beLaseOneRecord(int wNum);
	virtual void delAllCtrlData();
public:
	CArray<int,int> m_ArrInIndex;//ÄÚ²¿±àºÅ
	CArray<int,int> m_ArrMonth;
	CArray<int,int> m_ArrDay;
	CStringArray m_ArrRunTime;

};

#endif // !defined(AFX_MCTRLMONTH_H__683DE1AF_31DE_4F69_AE62_9022F23FA95C__INCLUDED_)
