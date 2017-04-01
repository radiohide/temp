// LinkToBlock.h: interface for the CLinkToBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_)
#define AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>
class CLinkToBlock : public CCalcBlock  
{
public:
	CLinkToBlock();
	virtual ~CLinkToBlock();
public:
	CString m_sOutPut1,m_sOutPut2,m_sOutPut3,m_sOutPut4,m_sOutPut5;
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
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//将属性放置传输数据中
	virtual void SetBlockPropertyToTransferData(
		CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
	//返回属性的逗点字符串
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//设置计算块的字符串属性
private:
	void reviewPort(CWnd* parent,CStringArray &IDArrLeftOut,std::vector<int> &ArrLeftDatytypeOut);
	void removeElemFromWSByKey(CString strKey);
	CString getRefToWSTagName(int index);

};

#endif // !defined(AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_)
