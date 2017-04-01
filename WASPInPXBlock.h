// WASPInPXBlock.h: interface for the CWASPInPXBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPXBLOCK_H__929DE7B3_AEF9_4921_A8D0_C63CBC2CCAA4__INCLUDED_)
#define AFX_WASPINPXBLOCK_H__929DE7B3_AEF9_4921_A8D0_C63CBC2CCAA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPXBlock : public CWASPCalcBlock 
{
public:
	CWASPInPXBlock();
	virtual ~CWASPInPXBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINPXBLOCK_H__929DE7B3_AEF9_4921_A8D0_C63CBC2CCAA4__INCLUDED_)
