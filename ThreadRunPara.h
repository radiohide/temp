// ThreadRunPara.h: interface for the ThreadRunPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADRUNPARA_H__E12F2780_974A_4CB6_A8A8_98BEAA2FC6B0__INCLUDED_)
#define AFX_THREADRUNPARA_H__E12F2780_974A_4CB6_A8A8_98BEAA2FC6B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcModel.h"

class ThreadRunPara  
{
public:
	CCalcModel*  m_pModel;  //线程对应的模型
	HWND m_hWnd;  //向外传送消息的窗口
	//------------------------------------------
	//计算模式，
	//模式0：实时计算；--> 需要定时
	//模式1：历史段模拟实时计算； --> 需要定时
	//模式2：历史段直接按时刻计算，每次计算都快速完成（每次设定模型的时刻，时段）；--> 不需要定时
	int CalcMode;       //运行模式
	int intCycle;          //实时的计算周期
	int TimeInterval;  //时间间隔
	CTime startTime;  //开始时间
	CTime endTime;  //结束时间
	CTimeSpan cTSpan; 

public:
	ThreadRunPara();
	virtual ~ThreadRunPara();
	void Calc();
 //protected:
	//关于实时的参数
	//关于计划任务的参数
};

#endif // !defined(AFX_THREADRUNPARA_H__E12F2780_974A_4CB6_A8A8_98BEAA2FC6B0__INCLUDED_)
