// H8CalcBlock.h: interface for the CH8CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_)
#define AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CH8CalcBlock : public CCalcBlock  
{
public:
	CH8CalcBlock();
	virtual ~CH8CalcBlock();

	virtual void ConstructBlockInputOutput(); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
};

#endif // !defined(AFX_H8CALCBLOCK_H__A4851716_51F9_446F_9F85_BE54420AC9CC__INCLUDED_)
