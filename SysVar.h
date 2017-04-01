// SysVar.h: interface for the SysVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_)
#define AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include   <math.h> 
class CCalcModel;
class SysVar  
{
public:
	CString ID;
	CString Name;
	CString Desp;
	CString unit;
	CString var_expression;
	int type;//数字模拟
	double category_endpoint;//端点
	double fixed_interval;//固定步长
	double relative_interval;//相对步长
	CString cluster_source;//离散化方式
public:
	SysVar();
	virtual ~SysVar();
	void SetValues(double category_endpoint,double fixed_interval,
		double relative_interval,CString cluster_source);
	int UpDateDB();

	int GetSelfDisInterval(int iLeftNum,//左离散化个数
						int iRightNum,//右离散化个数
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

	int GetSelfDisInterval(double dSValue,//离散化范围的开始
						double dEValue,//离散化范围的结束
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

	int GetDisInterval(	CString strComFun, 
						double dPointValue,
						double dStepValue,	
						int iLeftNum,//左离散化个数
						int iRightNum,//右离散化个数
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);
	
	int GetDisInterval(	CString strComFun, 
						double dPointValue,
						double dStepValue,	
						double dSValue,//离散化范围的开始
						double dEValue,//离散化范围的结束
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

};

#endif // !defined(AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_)
