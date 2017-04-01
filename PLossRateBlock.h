// PLossRateBlock.h: interface for the CPLossRateBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_)
#define AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CPLossRateBlock : public CCalcBlock  
{
public:
	CPLossRateBlock();
	virtual ~CPLossRateBlock();
public:
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
};

#endif // !defined(AFX_PLOSSRATEBLOCK_H__231D81C4_EF93_4A48_9796_1945F33ADEC9__INCLUDED_)
