// WASPInPHBlock.h: interface for the CWASPInPHBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_)
#define AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPHBlock : public CWASPCalcBlock  
{
public:
	CWASPInPHBlock();
	virtual ~CWASPInPHBlock();
public:
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
};

#endif // !defined(AFX_WASPINPHBLOCK_H__F13C475A_89E8_4CFC_8DD1_7BA1F3A4A96E__INCLUDED_)
