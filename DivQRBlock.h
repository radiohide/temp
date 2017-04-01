// DivQRBlock.h: interface for the CDivQRBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_)
#define AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDivQRBlock : public CCalcBlock  
{
public:
	CDivQRBlock();
	virtual ~CDivQRBlock();
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

#endif // !defined(AFX_DIVQRBLOCK_H__F29E7DD5_1061_414D_B1C1_6F6C0B901DD4__INCLUDED_)
