// StatProAvgBlock.h: interface for the CStatProAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATPROAVGBLOCK_H__5CF76553_3CFD_46E2_BC18_7A4F62BD22E7__INCLUDED_)
#define AFX_STATPROAVGBLOCK_H__5CF76553_3CFD_46E2_BC18_7A4F62BD22E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatProAvgBlock : public CCalcBlock  
{
public:
	CStatProAvgBlock(int iTimeSelfOrInput);
	virtual ~CStatProAvgBlock();
public:
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
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	void statData(CUniValue  &CUV,
							 double &dAvg,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
};

#endif // !defined(AFX_STATPROAVGBLOCK_H__5CF76553_3CFD_46E2_BC18_7A4F62BD22E7__INCLUDED_)
