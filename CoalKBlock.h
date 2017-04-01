// CoalKBlock.h: interface for the CCoalKBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_)
#define AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCoalKBlock : public CCalcBlock  
{
public:
	CCoalKBlock();
	virtual ~CCoalKBlock();
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
private:
	BOOL getSETimeNeedToCalc(_ConnectionPtr &m_pConRDB,
		CString strDBType,
		long time,
		long &lRealRecordTime,
		CArray<long,long> &sTimeArr,
		CArray<long,long> &eTimeArr);

	BOOL getCoalK(CStringArray &TagFlowArr,long sTime,long eTime,double &dCoalK);
	void writeCoalK(_ConnectionPtr &m_pConRDB,long time,double dCoalK);
private:
	int initDbConnect(HINSTANCE hDLL);
	BOOL loadDll();
public:
	int m_iFDay;
	int m_iSHour;
	int m_iSMinute;
	int m_iSSecond;

	int m_iBDay;
	int m_iEHour;
	int m_iEMinute;
	int m_iESecond;

	int m_iStepLength;
	int m_iStepUnit;//1:秒，2分，3小时

	CString m_strPowerTagName;
	CString m_StrPowerUnit;

	CStringArray m_strFlowTagNameArr;
	CStringArray m_strFlowUnitArr;
	CStringArray m_strFlowDespArr;

	CString m_strTableName;
	CString m_strDBName;

	long	m_lOffset;
	int		m_TagTimeOffsetOrInput;
private:
	HINSTANCE  hDLL;
	bool m_bConDB;
};

#endif // !defined(AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_)
