// StatAverBlock.h: interface for the CStatAverBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_)
#define AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatCalcBlock.h"

class CStatAverBlock : public CStatCalcBlock  
{
public:
	CStatAverBlock();
	virtual ~CStatAverBlock();
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
public:
	CArray<double,double> m_dAvgArr;
	CArray<double,double> m_dMaxArr;
	CArray<double,double> m_dMinArr;
private:
	void writeDataToDB(CString strVarName,CString strUnitSet,
						long lSTime,long lETime,
						double dAvgValue,double dMaxValue,double dMinValue);
};

#endif // !defined(AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_)
