// WASPInTVBlock.h: interface for the CWASPInTVBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINTVBLOCK_H__87ED07BD_8FE3_43E1_BA60_0E0669C71B14__INCLUDED_)
#define AFX_WASPINTVBLOCK_H__87ED07BD_8FE3_43E1_BA60_0E0669C71B14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInTVBlock : public CWASPCalcBlock  
{
public:
	CWASPInTVBlock();
	virtual ~CWASPInTVBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTVBLOCK_H__87ED07BD_8FE3_43E1_BA60_0E0669C71B14__INCLUDED_)
