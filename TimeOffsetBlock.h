// TimeOffsetBlock.h: interface for the CTimeOffsetBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_)
#define AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CTimeOffsetBlock : public CCalcBlock  
{
public:
	CTimeOffsetBlock();
	virtual ~CTimeOffsetBlock();
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
	int		m_iOffsetType;//0:秒;1小时;2:日;3:周;4:月;5:年;
	int		m_iSecond;//秒

	int		m_iHour;//小时
	CTime	m_HTime;//小时-时间

	int		m_iDay;//日
	CTime	m_DTime;//日-时间

	int		m_iWeek;//周
	int		m_iWeekDay;//周-日
	CTime	m_WTime;//周-时间

	
	int		m_iMonth;//月
	int		m_iMonthDay;//月-日
	CTime	m_MTime;//月-时间
	
	int		m_iYear;//年
	int		m_iYearDay;//年-日
	int		m_iYearMonth;//年-月
	CTime	m_YTime;//年-时间
private:
	int getDayOfTheMonth(int iYear,int iMonth,int iDayIn);
};

#endif // !defined(AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_)
