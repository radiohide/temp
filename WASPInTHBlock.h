// WASPInTHBlock.h: interface for the CWASPInTHBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINTHBLOCK_H__5E3CB49C_B66B_4F4B_AF45_ABEA89554262__INCLUDED_)
#define AFX_WASPINTHBLOCK_H__5E3CB49C_B66B_4F4B_AF45_ABEA89554262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInTHBlock : public CWASPCalcBlock  
{
public:
	CWASPInTHBlock();
	virtual ~CWASPInTHBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTHBLOCK_H__5E3CB49C_B66B_4F4B_AF45_ABEA89554262__INCLUDED_)
