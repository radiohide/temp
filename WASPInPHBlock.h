// WASPInPHBlock.h: interface for the CWASPInPHBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_)
#define AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPHBlock : public CWASPCalcBlock  
{
public:
	CWASPInPHBlock();
	virtual ~CWASPInPHBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
};

#endif // !defined(AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_)
