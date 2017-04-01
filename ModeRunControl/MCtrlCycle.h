// MCtrlCycle.h: interface for the MCtrlCycle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLCYCLE_H__0FBAF60F_86EC_4CFF_8BD1_5C89132ADD7D__INCLUDED_)
#define AFX_MCTRLCYCLE_H__0FBAF60F_86EC_4CFF_8BD1_5C89132ADD7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MCtrlType.h"

class MCtrlCycle : public MCtrlType  
{
public:
	MCtrlCycle(CString modelName);
	virtual ~MCtrlCycle();
	virtual int GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
								CTime &nextCalAbsTime,CTime &nextCalDataTime,
								CTime &nextDataSTime,CTime &nextDataETime);
	virtual void ReadCtrlData();
	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();
public:
	int m_iUnit;
	long m_lCycle_Length;
};

#endif // !defined(AFX_MCTRLCYCLE_H__0FBAF60F_86EC_4CFF_8BD1_5C89132ADD7D__INCLUDED_)
