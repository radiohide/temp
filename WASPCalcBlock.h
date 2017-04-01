// WASPCalcBlock.h: interface for the CWASPCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_)
#define AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
//其calc是针对两输入的。
class CWASPCalcBlock : public CCalcBlock  
{
public:
	CWASPCalcBlock();
	virtual ~CWASPCalcBlock();
public:
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
protected:
	RTFUNC rtFunc;//外部函数指针
	CString m_strFuncName;//配置的函数名
	TAGINFO* PSourceValue;//公式计算时候准备的数据
	TAGINFO* pG;//公式计算所得值
	CStringArray m_strArrFunDisp;//所有方法描述
	CStringArray m_strArrFunName;//所有方法名称
	CStringArray m_strArrOutPortName;//输出的端口名称
	CString m_strBlockDisp;//计算块描述
};

#endif // !defined(AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_)
