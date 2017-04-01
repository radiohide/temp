// wMnStmCalcBlock.h: interface for the CwMnStmCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_)
#define AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CycleBlock.h"

class CwMnStmCalcBlock : public CCycleBlock  
{
public:
	CwMnStmCalcBlock();
	virtual ~CwMnStmCalcBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//����ѭ������
	virtual void CalcIter();
private:
	CStringArray m_VarArr;//����ǰ����
	CStringArray m_FormulaArr;
	CArray<int,int> m_TypeArr;//0,��ֵ��1,����
private:
	void buildInsideEle();

};

#endif // !defined(AFX_WMNSTMCALCBLOCK_H__09746588_3821_4C48_8D48_FA457AD477D9__INCLUDED_)
