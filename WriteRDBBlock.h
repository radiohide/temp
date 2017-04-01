// WriteRDBBlock.h: interface for the CWriteRDBBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_)
#define AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBBlock : public CCalcBlock  
{
public:
	CWriteRDBBlock();
	virtual ~CWriteRDBBlock();
public:
	//添加必要的虚函数：
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
	//返回属性的逗点字符串
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//设置计算块的字符串属性
public:
	int m_TagTimeOffsetOrInput;//1:采用系统时间+偏移。0:采用外部偏移。
	long m_lOffset;//时间偏移量值
	CString m_strTagName;
	CString m_strTagDesp;
	CString	m_strTableName;
	CString m_strDBDesp;
	int m_iPointType;
	int m_iPointStatus;
private:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	void creatPointDataTableInMDB(CString tableName);
	BOOL gIsTableExist(_ConnectionPtr& pConnection, CString strTableName);
};

#endif // !defined(AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_)
