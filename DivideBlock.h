// DivideBlock.h: interface for the CDivideBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_)
#define AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDivideBlock : public CCalcBlock  
{
public:
	CDivideBlock();
	virtual ~CDivideBlock();
//��ӱ�Ҫ���麯����
//-----------------------------------------------------------------------------------------------------------
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
};

#endif // !defined(AFX_DIVIDEBLOCK_H__D07E3948_523A_4619_B321_CE49E3A684B7__INCLUDED_)
