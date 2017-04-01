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
	CCalcModel*  m_pModel;  //�̶߳�Ӧ��ģ��
	HWND m_hWnd;  //���⴫����Ϣ�Ĵ���
	//------------------------------------------
	//����ģʽ��
	//ģʽ0��ʵʱ���㣻--> ��Ҫ��ʱ
	//ģʽ1����ʷ��ģ��ʵʱ���㣻 --> ��Ҫ��ʱ
	//ģʽ2����ʷ��ֱ�Ӱ�ʱ�̼��㣬ÿ�μ��㶼������ɣ�ÿ���趨ģ�͵�ʱ�̣�ʱ�Σ���--> ����Ҫ��ʱ
	int CalcMode;       //����ģʽ
	int intCycle;          //ʵʱ�ļ�������
	int TimeInterval;  //ʱ����
	CTime startTime;  //��ʼʱ��
	CTime endTime;  //����ʱ��
	CTimeSpan cTSpan; 

public:
	ThreadRunPara();
	virtual ~ThreadRunPara();
	void Calc();
 //protected:
	//����ʵʱ�Ĳ���
	//���ڼƻ�����Ĳ���
};

#endif // !defined(AFX_THREADRUNPARA_H__E12F2780_974A_4CB6_A8A8_98BEAA2FC6B0__INCLUDED_)
