// StatAvgBlock.h: interface for the CStatAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATAVGBLOCK_H__5704ECD0_FE7B_4F42_96C0_E8BFF71A8485__INCLUDED_)
#define AFX_STATAVGBLOCK_H__5704ECD0_FE7B_4F42_96C0_E8BFF71A8485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>

class CStatAvgBlock : public CCalcBlock  
{
public:
	CStatAvgBlock();
	virtual ~CStatAvgBlock();
//添加必要的虚函数：
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

	CStringArray tagNameArr;//点名
	CStringArray unitArr;//单位
	CStringArray dispArr;//描述
	CArray<int,int> reqTypeArr;//查询方式
	CArray<int,int> snapshotTArr;//快照周期

	CStringArray strOutPutNameArr;//写入关系库的表名称（不包含年后缀）
	int m_iTagTimeIntM;//时间在整分左右m_iTimeLengthIntM秒取整分标志，默认1取整分

	//条件表达式
	int m_iConOn;//条件开关
	CString m_strConVar;//条件变量
	CString m_strConUnit;//单位
	CString m_strConDesp;//描述
	int m_iConReqType;//查询方式
	int m_iConSnapShotT;//快照周期
	int m_iCon1Fun;//条件方法1
	int m_iCon2Fun;//条件方法2
	double m_dCon1Value;//条件值1
	double m_dCon2Value;//条件值2
	//条件表达式
private:
	double dLimitValue;
	int m_iTimeLengthIntM;//在整分左右m_iTimeLengthIntM秒取整分，默认为2秒
private:
	void getInRangeValue(double &dValue);//检查是否在范围内，并获取范围内的值
	void getTimeIntM(long &lSTime,long &lETime);//对整分取整
	int readDataFromRTDB(CString strName,long lSTime,long lETime,int iReqType,int isnapshotT,std::vector<TagData> &tagDataArr);
	bool checkTableExist(CString strTable);
	void createTable(CString strTable);
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		double dMaxValue,
		double dMinValue,
		double dSum,
		long iCount);
	void getStatConGBTimeArr(std::vector<TagData> &ConTagDataArr,
		long lSTime,
		long lETime,
		std::vector<long> &lGSTime,//好时间段开始时间
		std::vector<long> &lGETime,//好时间段结束时间
		std::vector<long> &lBSTime,//坏时间段开始时间
		std::vector<long> &lBETime//坏时间段结束时间
		);
	int getConValueRegionNum(double dValue,const double &dRangMin,const double &dRangMax);
	void statData(std::vector<TagData> &tagDataArr,
							 double &dSum,
							 double &dMin,
							 double &dMax);
};

#endif // !defined(AFX_STATAVGBLOCK_H__5704ECD0_FE7B_4F42_96C0_E8BFF71A8485__INCLUDED_)
