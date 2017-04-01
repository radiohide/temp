// WASPInPTLBlock.h: interface for the CWASPInPTLBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_)
#define AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPTLBlock : public CWASPCalcBlock  
{
public:
	CWASPInPTLBlock();
	virtual ~CWASPInPTLBlock();
public:
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
};

#endif // !defined(AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_)
