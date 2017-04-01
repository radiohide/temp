// ModelCalcControl.h: interface for the ModelCalcControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELCALCCONTROL_H__B8A10473_AFB0_4C17_8128_9D20C4C3C0EF__INCLUDED_)
#define AFX_MODELCALCCONTROL_H__B8A10473_AFB0_4C17_8128_9D20C4C3C0EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MCtrlType.h"
#include "MCtrlCycle.h"
#include "MCtrlDay.h"
#include "MCtrlDuty.h"
#include "MCtrlMonth.h"
#include "MCtrlWeek.h"
#include <afxtempl.h>

class ModelCalcControl
{
public:
	ModelCalcControl(CString modelName);
	virtual ~ModelCalcControl();
public:
	void SetDefaulControlData();
	int GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
		CTime &nextCalAbsTime,CTime &nextCalDataTime,CTime &nextDataSTime,CTime &nextDataETime);

	void SetConPtr(_ConnectionPtr &pCon);
	void SetModeRunType(int type);
	void SaveCtrlDataToDB();
	void SaveAllCtrlDataToDB();
	void DelCtrlDataInDB();
	CString GetMLifeStr();
	CString GetCalcDataSETimeStr();
	void ReadCtrlData();

	CTime m_tRunAbs_STime;
	CTime m_tRunAbs_ETime;
	CTime m_tCalcDataAll_STime;
	CTime m_tCalcDataAll_ETime;
	int m_iMode_RunType;
	_ConnectionPtr pCon;
	CArray<MCtrlType*,MCtrlType*&> m_ArrMCtrl;
public:
	CString m_strModelName;
private:
	CTime strTimeToCTime(CString strTime);

};

#endif // !defined(AFX_MODELCALCCONTROL_H__B8A10473_AFB0_4C17_8128_9D20C4C3C0EF__INCLUDED_)
