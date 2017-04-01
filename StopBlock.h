// StopBlock.h: interface for the CStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_)
#define AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStopBlock : public CCalcBlock  
{
public:
	CStopBlock();
	virtual ~CStopBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
	//��������ʾ��ͬ�����Զ���Ի���
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
};

#endif // !defined(AFX_STOPBLOCK_H__D08891A2_5DDF_4207_9126_D10E248040A0__INCLUDED_)
