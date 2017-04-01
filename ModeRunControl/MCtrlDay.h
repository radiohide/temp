// MCtrlDay.h: interface for the MCtrlDay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLDAY_H__A4B63A39_7539_4DBA_A85D_8E24EC80C86F__INCLUDED_)
#define AFX_MCTRLDAY_H__A4B63A39_7539_4DBA_A85D_8E24EC80C86F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MCtrlType.h"

class MCtrlDay : public MCtrlType  
{
public:
	MCtrlDay(CString modelName);
	virtual ~MCtrlDay();
	virtual int GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
								CTime &nextCalAbsTime,CTime &nextCalDataTime,
								CTime &nextDataSTime,CTime &nextDataETime);
	virtual void ReadCtrlData();
	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();
public:
	CStringArray m_ArrRunTime;
private:
	virtual void delAllCtrlData();
	CTime getNextMostRecentTime(CTime time,int tagBeEql);
	int getTimeIndexInArrRunTime(CTime time);
};

#endif // !defined(AFX_MCTRLDAY_H__A4B63A39_7539_4DBA_A85D_8E24EC80C86F__INCLUDED_)
