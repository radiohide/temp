// MCtrlWeek.h: interface for the MCtrlWeek class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLWEEK_H__FF3FF4BF_3D1A_4733_9795_A1962C2AA33C__INCLUDED_)
#define AFX_MCTRLWEEK_H__FF3FF4BF_3D1A_4733_9795_A1962C2AA33C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MCtrlType.h"

class MCtrlWeek : public MCtrlType  
{
public:
	MCtrlWeek(CString modelName);
	virtual ~MCtrlWeek();
	virtual void ReadCtrlData();
	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();
	
	void GetTheRecordDataByInIndex(int inIndex,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE);
	void GetTheWeekData(int wNum,CString *&pStrTime,MCtrlOffsetSE *&pMCtrlOffsetSE);
	void GetWeekNums(CArray<int,int> &arrWeekArr);
	void SetNewWeekNums(CArray<int,int> &arrWeekArr);
	void AddOneRecord(int weekNum);
	void DelRecordByWNum(int weekNum);
	void DelRecordByInIndex(int inindex);
private:
	BOOL beLaseOneRecord(int wNum);
	virtual void delAllCtrlData();
public:
	CArray<int,int> m_ArrWeek;
	CStringArray m_ArrRunTime;
	CArray<int,int> m_ArrInIndex;//ÄÚ²¿±àºÅ
};

#endif // !defined(AFX_MCTRLWEEK_H__FF3FF4BF_3D1A_4733_9795_A1962C2AA33C__INCLUDED_)
