// ModuleStopBlock.h: interface for the CModuleStopBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_)
#define AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CModuleStopBlock : public CCalcBlock  
{
public:
	CModuleStopBlock();
	virtual ~CModuleStopBlock();
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

#endif // !defined(AFX_MODULESTOPBLOCK_H__1FB12056_A6C4_4DBB_8FCD_113191A08A27__INCLUDED_)
