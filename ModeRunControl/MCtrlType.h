// MCtrlType.h: interface for the MCtrlType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLTYPE_H__3499C583_212C_4848_9A27_68D4EFC07C0A__INCLUDED_)
#define AFX_MCTRLTYPE_H__3499C583_212C_4848_9A27_68D4EFC07C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MCtrlOffsetSE.h"

class MCtrlType  
{
public:
	MCtrlType(CString modelName);
	virtual ~MCtrlType();
public:
	CTime *m_pTRunAbs_STime;
	CTime *m_pTRunAbs_ETime;
	CTime *m_pTCalcDataAll_STime;
	CTime *m_pTCalcDataAll_ETime;

	CString m_strModelName;
	CArray<MCtrlOffsetSE*,MCtrlOffsetSE*&> m_ArrOffsetSE;
	_ConnectionPtr pCon;
public:
	void SetRunAbsSETimeAndDataAllSETimePtr(CTime *m_pTRunAbs_STime,CTime *m_pTRunAbs_ETime,
											CTime *m_pTCalcDataAll_STime,CTime *m_pTCalcDataAll_ETime);

	//返回值0，即该任务生命周期结束。
	//返回值1，进行下一次定时。
	//返回值2, 数据处理完毕
	//返回值3，没有定时
	virtual int GetNextCalcTime(CTime curCalAbsTime,CTime curCalDataTime,
								CTime &nextCalAbsTime,CTime &nextCalDataTime,
								CTime &nextDataSTime,CTime &nextDataETime);
	virtual void ReadCtrlData();
	virtual void ResetCtrlData();
	virtual void SaveCtrlDataToDB();
	virtual void DelCtrlDataIoDB();

	void SetConPtr(_ConnectionPtr &pCon);
protected:
	//返回值0，theTime在时间段开始之前；
	//返回值1，theTime在时间段内；
	//返回值2，theTime在时间段结束之后；
	int getTheTimeInOrOutSETime(const CTime &theTime,const CTime &sTime,const CTime &eTime);
	virtual void delAllCtrlData();
	virtual MCtrlOffsetSE * getOffsetObjPtrByIndex(int index);
	
};

#endif // !defined(AFX_MCTRLTYPE_H__3499C583_212C_4848_9A27_68D4EFC07C0A__INCLUDED_)
