// RelationalBlock.h: interface for the CRelationalBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_)
#define AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRelationalBlock : public CCalcBlock  
{
public:
	CRelationalBlock();
	virtual ~CRelationalBlock();
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
	CStringArray m_strArrFunName;//所有方法名称
	CStringArray m_strArrFunDisp;//所有方法描述	

	CString m_strFuncName;//当前的方法名
	CString m_strBlockDisp;//计算块描述
	CString m_strPropertityTableName;//属性窗口名称
};

#endif // !defined(AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_)
