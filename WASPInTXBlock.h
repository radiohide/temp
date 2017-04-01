// WASPInTXBlock.h: interface for the CWASPInTXBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINTXBLOCK_H__7C1D7197_E2DE_4271_8E45_BF61FA42A9E8__INCLUDED_)
#define AFX_WASPINTXBLOCK_H__7C1D7197_E2DE_4271_8E45_BF61FA42A9E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInTXBlock : public CWASPCalcBlock  
{
public:
	CWASPInTXBlock();
	virtual ~CWASPInTXBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTXBLOCK_H__7C1D7197_E2DE_4271_8E45_BF61FA42A9E8__INCLUDED_)
