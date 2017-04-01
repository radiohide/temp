// FormulaBlock.h: interface for the CFormulaBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_)
#define AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CFormulaBlock : public CCalcBlock  
{
public:
	CFormulaBlock();
	virtual ~CFormulaBlock();
public:
	int maxInPut;//最大输入个数
	int maxOutPut;//最大输出个数
	int maxFormula;//最大公式条数（最大中间变量个数也是这个）

	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;//0,数值；1,序列

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;//0,数值；1,序列

	CStringArray varArr;
	CStringArray formulaArr;
	CArray<int,int> varTypeArr;//0,数值；1,序列
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
private:
	void setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type);//type:0,单值点；1,序列值
	void setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type);//type:0,单值点；1,序列值
	void delInnerWorkSpace();//清空内部Map
	void establishInnerWorkSpace();//初始化变量到内部Map并解析公式
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void resetAllElement();//对所有元素进行复位，以便于进行下次计算。
};

#endif // !defined(AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_)
