// WindPowerTag.h: interface for the WindPowerTag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDPOWERTAG_H__178C028C_2B8B_4784_8895_7B019005DA84__INCLUDED_)
#define AFX_WINDPOWERTAG_H__178C028C_2B8B_4784_8895_7B019005DA84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class WindPowerTag  
{
public:
	WindPowerTag();
	virtual ~WindPowerTag();

public:
	double dWindSpeed;
	double dPower;
	long lTime;	
	long lrealTime;

public:
	bool bDel;
	double dPowerAvg;
	double dPowerVar;
	int iBinNum;//bin分组号
	double dMidSpeed;//中心风速
};

#endif // !defined(AFX_WINDPOWERTAG_H__178C028C_2B8B_4784_8895_7B019005DA84__INCLUDED_)
