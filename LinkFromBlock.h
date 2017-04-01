// LinkFromBlock.h: interface for the CLinkFromBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_)
#define AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>
class CLinkFromBlock : public CCalcBlock  
{
public:
	CLinkFromBlock();
	virtual ~CLinkFromBlock();
public:
	CString m_sInPut1,m_sInPut2,m_sInPut3,m_sInPut4,m_sInPut5;
	int m_iDataType1,m_iDataType2,m_iDataType3,m_iDataType4,m_iDataType5;
public:
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //画特征形状以区别Add at 2011-10-30
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
	void reviewPort(CWnd* parent,CStringArray &IDArrRightOut,std::vector<int> &ArrRightDatytypeOut);
	CCalcPort* getRefFromWSByTagIndex(int index);

};

#endif // !defined(AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_)
