// ClockBlock.h: interface for the CClockBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_)
#define AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CClockBlock : public CCalcBlock  
{
public:
	CClockBlock();
	virtual ~CClockBlock();

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
	int m_iBeHaveTime;//0:不输出，1：输出
	int m_iBeHaveBeginEndTime;//0:不输出，1：输出
};

#endif // !defined(AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_)
