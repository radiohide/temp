// StatImptBlock.h: interface for the CStatImptBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATIMPTBLOCK_H__EE67E7F3_CA10_4946_A6EC_52B534D38A51__INCLUDED_)
#define AFX_STATIMPTBLOCK_H__EE67E7F3_CA10_4946_A6EC_52B534D38A51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatCalcBlock.h"

class CStatImptBlock : public CStatCalcBlock  
{
public:
	CStatImptBlock();
	virtual ~CStatImptBlock();
	virtual void Calc(); //进行计算的函数,需要重载
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
public:
	CArray<double,double> m_dValueArr;
private:
	void writeDataToDB(CString strVarName,CString strUnitSet,
		long lSTime,long lETime,double dValue);

};

#endif // !defined(AFX_STATIMPTBLOCK_H__EE67E7F3_CA10_4946_A6EC_52B534D38A51__INCLUDED_)
