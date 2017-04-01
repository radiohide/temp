// CycleBlock.h: interface for the CCycleBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_)
#define AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCycleBlock : public CCalcBlock  
{
public:
	CCycleBlock();
	virtual ~CCycleBlock();
public:
	int maxInPut;//最大输入个数
	int maxOutPut;//最大输出个数
	int maxFormula;//最大公式条数（最大中间变量个数也是这个）
	int tagConFOrB;//条件在迭代前计算还是迭代后计算，0在迭代前计算；1在迭代后计算

	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;//0,数值；1,序列
	CArray<int,int> inPutCanBeEdit;//0不可被编辑；1可被编辑；

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;//0,数值；1,序列
	CArray<int,int> outPutCanBeEdit;//0不可被编辑；1可被编辑；

	CStringArray IterFVarArr;//迭代前变量
	CStringArray IterFFormulaArr;
	CArray<int,int> IterFVarTypeArr;//0,数值；1,序列
	CArray<int,int> IterFCanBeEdit;//0不可被编辑；1可被编辑；
	CArray<int,int> IterFCanBeShow;//0不可显示；1可显示；

	CStringArray conVarArr;//条件变量
	CStringArray conFormulaArr;
	CArray<int,int> conVarTypeArr;//0,数值；1,序列
	CArray<int,int> conVarCanBeEdit;//0不可被编辑；1可被编辑；
	CArray<int,int> conVarCanBeShow;//0不可显示；1可显示；

	CStringArray IterVarArr;//迭代变量
	CStringArray IterFormulaArr;
	CArray<int,int> IterVarTypeArr;//0,数值；1,序列
	CArray<int,int> IterCanBeEdit;//0不可被编辑；1可被编辑；
	CArray<int,int> IterCanBeShow;//0不可显示；1可显示；

	CStringArray IterBVarArr;//迭代后变量
	CStringArray IterBFormulaArr;
	CArray<int,int> IterBVarTypeArr;//0,数值；1,序列
	CArray<int,int> IterBCanBeEdit;//0不可被编辑；1可被编辑；
	CArray<int,int> IterBCanBeShow;//0不可显示；1可显示；

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
	//进行循环计算
	virtual void CalcIter(); 
public:
	void BulidInPutEle();
	void BulidIterFEle();
	void BulidConVarEle();
	void BulidIterEle();
	void BulidIterBEle();

	void ElementParseFormula();
	void DelInnerWorkSpace();//清空Map
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type);//type:0,单值点；1,序列值
	void setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type);//type:0,单值点；1,序列值
	bool calcCon(); //进行条件
	void setBlokcElemBeCalc(); //设置所有元素被计算过
	void setAllElemReSet(); //设置所有元素未被计算过

};

#endif // !defined(AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_)
