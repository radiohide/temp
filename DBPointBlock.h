// DBPointBlock.h: interface for the CDBPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_)
#define AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDBPointBlock : public CCalcBlock  
{
public:
	CDBPointBlock();
	virtual ~CDBPointBlock();
public:
//添加必要的虚函数：
//-----------------------------------------------------------------------------------------------------------
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
public:
	int m_TagTimeOffsetOrInput;//1:采用系统时间+偏移。0:采用外部偏移。
	long m_lOffset;//时间偏移量值
	CString m_strTagName;
	CString m_strTagDesp;
	int		m_iForwardOrBack;
	CString	m_strTableName;
	CString m_strDBDesp;

	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	HINSTANCE  hDLL;
	bool m_bConDB;
private:
	BOOL getValueFromDB(_ConnectionPtr &m_pConRDB,
		CString strDBType,
		CString strTableName,
		CString strTagName,
		long timeflag,
		int iForwardOrBack,
		double &dRetValue,
		long &lRetTime,
		int &iRetState);

};

#endif // !defined(AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_)
