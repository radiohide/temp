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
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTXBLOCK_H__7C1D7197_E2DE_4271_8E45_BF61FA42A9E8__INCLUDED_)
