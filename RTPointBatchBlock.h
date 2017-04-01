// RTPointBatchBlock.h: interface for the CRTPointBatchBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_)
#define AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRTPointBatchBlock : public CCalcBlock  
{
public:
	CRTPointBatchBlock();
	virtual ~CRTPointBatchBlock();
public:
	//virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //画特征形状以区别Add at 2011-10-30
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
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	char *pTagNames[1000];
	TagData *pTagData;
private:
	CStringArray strRTDBTagArr;//实时库点名
	CStringArray strPortTagArr;//端口点名
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位

	int m_iSysOrInput;

	
};

#endif // !defined(AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_)
