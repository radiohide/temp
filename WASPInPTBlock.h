// WASPInPTBlock.h: interface for the CWASPInPTBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPTBLOCK_H__D1515129_73F0_40CE_9EB8_4D5868A76CF5__INCLUDED_)
#define AFX_WASPINPTBLOCK_H__D1515129_73F0_40CE_9EB8_4D5868A76CF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPTBlock : public CWASPCalcBlock  
{
public:
	CWASPInPTBlock();
	virtual ~CWASPInPTBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINPTBLOCK_H__D1515129_73F0_40CE_9EB8_4D5868A76CF5__INCLUDED_)
