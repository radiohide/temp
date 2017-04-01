// StatStatAvgBlock.h: interface for the CStatStatAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATSTATAVGBLOCK_H__A76C4762_CDDB_401A_B0C2_8A21B6C2EAB8__INCLUDED_)
#define AFX_STATSTATAVGBLOCK_H__A76C4762_CDDB_401A_B0C2_8A21B6C2EAB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatProAvgBlock.h"

class CStatStatAvgBlock : public CStatProAvgBlock  
{
public:
	CStatStatAvgBlock(int iTimeSelfOrInput);
	virtual ~CStatStatAvgBlock();
public:
	static CCalcBlock* CreateFromString( const CString& strType );
	virtual void Calc(); //进行计算的函数,需要重载
	virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
private:
	void statData(CUniValue  &CUV,
							 double &dAvg,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
};

#endif // !defined(AFX_STATSTATAVGBLOCK_H__A76C4762_CDDB_401A_B0C2_8A21B6C2EAB8__INCLUDED_)
