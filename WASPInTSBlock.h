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
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
};

#endif // !defined(AFX_WASPINTSBLOCK_H__BCABCE41_4A35_471D_99D7_B408AC293836__INCLUDED_)
