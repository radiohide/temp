// WASPInPVBlock.h: interface for the CWASPInPVBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPVBLOCK_H__2BD9117C_6064_4912_9DAE_A26F0F1133FF__INCLUDED_)
#define AFX_WASPINPVBLOCK_H__2BD9117C_6064_4912_9DAE_A26F0F1133FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPVBlock : public CWASPCalcBlock  
{
public:
	CWASPInPVBlock();
	virtual ~CWASPInPVBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINPVBLOCK_H__2BD9117C_6064_4912_9DAE_A26F0F1133FF__INCLUDED_)
