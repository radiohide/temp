// StatEnumBlock.h: interface for the CStatEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_)
#define AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>

class CStatEnumBlock : public CCalcBlock  
{
public:
	CStatEnumBlock();
	virtual ~CStatEnumBlock();

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
public:
	CString m_strDBName;//数据库名称

	CString m_strVarName;//变量名称
	CString m_strVarUnit;//变量单位
	CString m_strVarDesp;//变量描述
	CString m_strOutName;//输出变量
	int m_iReqType;//查询方式：0快照，1原始
	long m_lSnapshotT;//查询周期
	int m_iStatType;//统计方式：0出现次数、1持续时长
	
	CArray<double,double> m_dSValueArr;//区间开始
	CArray<double,double> m_dEValueArr;//区间结束
	CArray<int,int> m_iSETag;//区间状态
	CArray<int,int> m_iUpOrDowmLimit;//高限或低限制，0低限；1高限
	int m_iTagTimeIntM;//时间在整分左右m_iTimeLengthIntM秒取整分标志，默认1取整分
private:
	double dLimitValue;
	int m_iTimeLengthIntM;//在整分左右m_iTimeLengthIntM秒取整分，默认为2秒
private:
	void getInRangeValue(double &dValue);//检查是否在范围内，并获取范围内的值
	void getTimeIntM(long &lSTime,long &lETime);//对整分取整
void readDataFromRTDB(CString strName,long lSTime,long lETime,int iReqType,int isnapshotT,std::vector<TagData> &tagDataArr);
	bool checkTableExist(CString strTable);
	void createTable(CString strTable);
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		int  iStatType,
		double dTotalValue,
		long iCount);
	void statData(std::vector<TagData> &tagDataArr,
							 double &dSum,
							 double &dMin,
							 double &dMax);
};

#endif // !defined(AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_)
