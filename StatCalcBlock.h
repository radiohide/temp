// StatCalcBlock.h: interface for the CStatCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_)
#define AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatCalcBlock : public CCalcBlock  
{
public:
	CStatCalcBlock();
	virtual ~CStatCalcBlock();
//添加必要的虚函数：
//-----------------------------------------------------------------------------------------------------------
	//初始化计算的函数做特殊的初始化
	virtual void InitCalc();
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput (); 
	virtual void Calc();//进行计算的函数,需要重载
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
protected:
	CString m_strBlockDesp;
	CString m_strTitle;
protected:
	BOOL IsTableExist(_ConnectionPtr &pConnection, CString strTableName);
public:
	CString m_strDBName;
	int		m_iSETimeType;//0:系统时间段。1:外部输入时间段.2:数据时间段
	int m_iVarType;

	CStringArray m_strVarNameArr;//变量名称
	CStringArray m_strVarDespArr;//变量描述
	CStringArray m_strVarUnitArr;//机组类型
	CStringArray m_strUnitSetArr;//机组名称

	CArray<long,long> m_sTimeArr;//开始时间
	CArray<long,long> m_eTimeArr;//结束时间
};

#endif // !defined(AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_)
