// StatUtilizationRate.h: interface for the CStatUtilizationRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_)
#define AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatUtilizationRate : public CCalcBlock  
{
public:
	CStatUtilizationRate();
	virtual ~CStatUtilizationRate();

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

private:
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	void writeDataToTable(CString strTurbine,CString strTableName,TagData stateTable[],double dUtiliRate);
	void DataRevise(TagData stateTable[],double &dSum);
	void DataReviseR(TagData stateTable[],double dSum);

private:
	CString m_strDBName;
	CString m_strTableName;
	CString m_strTurbine;

};

#endif // !defined(AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_)
