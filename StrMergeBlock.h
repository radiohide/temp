// StrMergeBlock.h: interface for the CStrMergeBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRMERGEBLOCK_H__52DC9629_D36D_47A6_BA6B_E2B89409A494__INCLUDED_)
#define AFX_STRMERGEBLOCK_H__52DC9629_D36D_47A6_BA6B_E2B89409A494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgStrMerge.h"
class CStrMergeBlock : public CCalcBlock  
{
public:
	CStrMergeBlock();
	virtual ~CStrMergeBlock();

	int		m_strInputNum;
public:
	//必须重载的函数
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput(); 
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
};

#endif // !defined(AFX_STRMERGEBLOCK_H__52DC9629_D36D_47A6_BA6B_E2B89409A494__INCLUDED_)
