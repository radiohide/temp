// RTDBHistSeriesBat.h: interface for the CRTDBHistSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_)
#define AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRTDBHistSeriesBat : public CCalcBlock  
{
public:
	CRTDBHistSeriesBat();
	virtual ~CRTDBHistSeriesBat();
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
private:
	double dLimitValue;//=10000000000000000000;//1E19;
private:
	CStringArray strVarArr;//表名 strTableArr
	CStringArray strPortArr;//变量名strTagArr
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位

	int iRawOrSnapshot;//0:原始；1:快照
	int iSnapshotSpan;//快照间隔

private:
	void readDataFromRTDB(CString strName,long lSTime,long lETime,int iReqType,int isnapshotT,std::vector<TagData> &tagDataArr);
};

#endif // !defined(AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_)
