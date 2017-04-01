// StatStatSumBlock.h: interface for the CStatStatSumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATSTATSUMBLOCK_H__FCBA0C89_898A_447C_956D_9D4122776C93__INCLUDED_)
#define AFX_STATSTATSUMBLOCK_H__FCBA0C89_898A_447C_956D_9D4122776C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatProAvgBlock.h"

class CStatStatSumBlock : public CStatProAvgBlock  
{
public:
	CStatStatSumBlock(int iTimeSelfOrInput);
	virtual ~CStatStatSumBlock();
public:
	static CCalcBlock* CreateFromString( const CString& strType );
	virtual void Calc(); //进行计算的函数,需要重载
	virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
private:
	void statData(CUniValue  &CUV,
							 double &dSum,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
};

#endif // !defined(AFX_STATSTATSUMBLOCK_H__FCBA0C89_898A_447C_956D_9D4122776C93__INCLUDED_)
