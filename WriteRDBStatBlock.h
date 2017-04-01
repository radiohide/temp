// WriteRDBStatBlock.h: interface for the CWriteRDBStatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_)
#define AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBStatBlock : public CCalcBlock  
{
public:
	CWriteRDBStatBlock();
	virtual ~CWriteRDBStatBlock();
public:
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
	//返回属性的逗点字符串
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//设置计算块的字符串属性
public:
	CString m_strDBName;//数据库名称
	
	BOOL m_BStat;
	BOOL m_BMaxValue;
	BOOL m_BMinValue;
	BOOL m_BCount;//是否选择输出个数
	
	CString m_strVarDesp;
	CString m_strVarName;
private:
	int m_iTagTimeIntM;//默认取整分
	int m_iTimeLengthIntM;//默认整分左右两秒取整分
private:
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		double dMax,
		double dMin,
		int  iStatType,
		double dTotalValue,
		long iCount);
	void getTimeIntM(long &lSTime,long &lETime);
	void createTable(CString strTable);
	bool checkTableExist(CString strTable);
	void getInRangeValue(double &dValue);
};

#endif // !defined(AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_)
