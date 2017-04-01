// LinkFromStatBatBlock.h: interface for the CLinkFromStatBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_)
#define AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CLinkFromStatBatBlock : public CCalcBlock  
{
public:
	CLinkFromStatBatBlock();
	virtual ~CLinkFromStatBatBlock();

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
	
	CCalcPort* getRefFromWSByID(CString csID);
	
public:
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
private:
	CString m_str;

};

#endif // !defined(AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_)
