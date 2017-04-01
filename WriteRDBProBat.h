// WriteRDBProBat.h: interface for the CWriteRDBProBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_)
#define AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBProBat : public CCalcBlock  
{
public:
	CWriteRDBProBat();
	virtual ~CWriteRDBProBat();
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
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
private:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldRealDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	int m_iOnlySave;//只保存标志
	int m_iSaveSeconds;//只保存最近n秒内的数据。
	CString m_strDBName;//数据库名称

	CStringArray tagNameArr;//点名
	CStringArray unitArr;//单位
	CStringArray dispArr;//描述
	CStringArray strOutPutNameArr;//写入关系库的表名称（不包含年后缀）

};

#endif // !defined(AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_)
