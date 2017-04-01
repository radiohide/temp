// SleepBlock.h: interface for the CSleepBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLEEPBLOCK_H__03E7519B_89AC_4F82_866D_E0A5C3D81F06__INCLUDED_)
#define AFX_SLEEPBLOCK_H__03E7519B_89AC_4F82_866D_E0A5C3D81F06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CSleepBlock : public CCalcBlock  
{
public:
	CSleepBlock();
	virtual ~CSleepBlock();
	//添加必要的虚函数：
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //画特征形状以区别Add at 2011-10-30
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
    long m_iSleepMS;//睡眠的毫秒数
};

#endif // !defined(AFX_SLEEPBLOCK_H__03E7519B_89AC_4F82_866D_E0A5C3D81F06__INCLUDED_)
