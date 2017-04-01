// MCtrlDuty.h: interface for the MCtrlDuty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLDUTY_H__C6B662C5_8EE3_4660_9338_3C2D41E60D8E__INCLUDED_)
#define AFX_MCTRLDUTY_H__C6B662C5_8EE3_4660_9338_3C2D41E60D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MCtrlType.h"

class MCtrlDuty : public MCtrlType  
{
public:
	MCtrlDuty(CString modelName);
	virtual ~MCtrlDuty();
	virtual void ReadCtrlData();

	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();

	void ReadAllDuty();
public:
	CStringArray m_ArrDutyNum;
	CArray<int,int> m_ArrDutySOrE;//该值的开始或者结束。
	CStringArray m_ArrAllDuty;//所有的班值
private:
	bool tagBeReadAllDuty;
private:
	virtual void delAllCtrlData();

};

#endif // !defined(AFX_MCTRLDUTY_H__C6B662C5_8EE3_4660_9338_3C2D41E60D8E__INCLUDED_)
