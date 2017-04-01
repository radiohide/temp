// IfBlock.h: interface for the CIfBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFBLOCK_H__DFB6AE51_6A5F_4FB3_A7CC_70A64293D088__INCLUDED_)
#define AFX_IFBLOCK_H__DFB6AE51_6A5F_4FB3_A7CC_70A64293D088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CIfBlock : public CCalcBlock  
{
public:
	CIfBlock();
	virtual ~CIfBlock();
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
public:
	int m_iDataType;
};

#endif // !defined(AFX_IFBLOCK_H__DFB6AE51_6A5F_4FB3_A7CC_70A64293D088__INCLUDED_)
