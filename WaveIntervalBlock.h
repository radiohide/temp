// WaveIntervalBlock.h: interface for the CWaveIntervalBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_)
#define AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWaveIntervalBlock : public CCalcBlock  
{
public:
	CWaveIntervalBlock();
	virtual ~CWaveIntervalBlock();
public:
//-----------------------------------------------------------------------------------------------------------
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	//根据类型新建对象的函数
	static CCalcBlock* CreateFromString( const CString& strType );
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//计算函数，实现本模块的计算
	virtual void Calc(); //进行计算的函数,需要重载
    //用于根据参数项和值进行属性的设置，当读取值要用
	virtual void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="");
	//重载以显示不同的属性对象对话框
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
	//重载以将本模块写入到数据库中
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord);
	//复制图形时，需要拷贝属性。
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//将属性放置传输数据中
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
	BOOL getSmoothData(
		const CArray<double,double> &dOrigValueArr,
		const CArray<long,long> &dOrigTimeArr,
		const CArray<short,short> &dOrigStatusArr,
		int iSmoothWidth,
		int iSmoothStep,
		CArray<double,double> &dSmoothValueArr,
		CArray<long,long> &dSmoothTimeArr,
		CArray<short,short> &dSmoothStatusArr
		);
private:
	bool m_bBeFirstCalc;//是否第一计算
	double m_dDisValue;//记录最近一次所得到的离散值。
	
	//***********属性*************
	int m_iSmoothWidth;//滑动宽度(秒)
	int m_iSmoothStep;//滑动步长(秒)
	double m_dSpanIntPercent;//跨至临近区间超过边界的百分比
	int m_iSpanKeepTime;//跨至设定区间并持续的时间长度
	CArray<double,double> m_dArrDisValue;//离散值
	CArray<double,double> m_dArrIntStart;//区间开始值
	CArray<double,double> m_dArrIntEnd;//区间结束值
	//***********属性*************
private:
	CArray<double,double> m_dSmoothValueArr;
	CArray<long,long> m_dSmoothTimeArr;
	CArray<short,short> m_dSmoothStatusArr;
private:
	double getFirstCalcDisValue();
	int getDisValue(double &disValue);
	int getDisValueByValueArr(
		const CArray<double,double> &tempValueArr,
		const CArray<double,double> &dArrDisValue,
		const CArray<double,double> &dArrIntStart,
		const CArray<double,double> &dArrIntEnd,
		double &disValue);
	void getDisValueByValue(
		double dAvg,
		const CArray<double,double> &dArrDisValue,
		const CArray<double,double> &dArrIntStart,
		const CArray<double,double> &dArrIntEnd,
		double &disValue);
	void getNewDisDataArr(
		CArray<double,double> &dArrNewDisValue,
		CArray<double,double> &dArrNewIntStart,
		CArray<double,double> &dArrNewIntEnd);
};

#endif // !defined(AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_)
