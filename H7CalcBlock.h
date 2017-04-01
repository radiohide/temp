// H7CalcBlock.h: interface for the CH7CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_)
#define AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CH7CalcBlock : public CCalcBlock  
{
public:
	CH7CalcBlock();
	virtual ~CH7CalcBlock();

	virtual void ConstructBlockInputOutput(); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
};

#endif // !defined(AFX_H7CALCBLOCK_H__4310CCD2_65A0_426C_AED9_F783CD457875__INCLUDED_)
