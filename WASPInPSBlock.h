// WASPInPSBlock.h: interface for the CWASPInPSBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPSBLOCK_H__CB756E13_2FED_41AF_ACA0_A4B1B4CEB7AB__INCLUDED_)
#define AFX_WASPINPSBLOCK_H__CB756E13_2FED_41AF_ACA0_A4B1B4CEB7AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPSBlock : public CWASPCalcBlock  
{
public:
	CWASPInPSBlock();
	virtual ~CWASPInPSBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINPSBLOCK_H__CB756E13_2FED_41AF_ACA0_A4B1B4CEB7AB__INCLUDED_)
