// ThreadRunPara.cpp: implementation of the ThreadRunPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "ThreadRunPara.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreadRunPara::ThreadRunPara()
{
	m_hWnd = NULL;
	m_pModel = NULL;

	CalcMode = 0 ;
	intCycle = 1 ;
	TimeInterval = 30;

	CTime tm = CTime::GetCurrentTime();
	CTime tm1 = CTime(tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),0,0); //
	endTime = tm1;
	startTime = endTime - CTimeSpan(1,0,0,0);

}

ThreadRunPara::~ThreadRunPara()
{

}
void ThreadRunPara::Calc()
{
       //AfxMessageBox("calc once");
} 
