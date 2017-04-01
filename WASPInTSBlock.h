// WASPInTSBlock.h: interface for the CWASPInTSBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINTSBLOCK_H__BCABCE41_4A35_471D_99D7_B408AC293836__INCLUDED_)
#define AFX_WASPINTSBLOCK_H__BCABCE41_4A35_471D_99D7_B408AC293836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInTSBlock : public CWASPCalcBlock  
{
public:
	CWASPInTSBlock();
	virtual ~CWASPInTSBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTSBLOCK_H__BCABCE41_4A35_471D_99D7_B408AC293836__INCLUDED_)
