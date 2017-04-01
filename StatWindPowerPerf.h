// StatWindPowerPerf.h: interface for the CStatWindPowerPerf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATWINDPOWERPERF_H__16B7F97D_2411_49D8_BE67_1A875E1D838E__INCLUDED_)
#define AFX_STATWINDPOWERPERF_H__16B7F97D_2411_49D8_BE67_1A875E1D838E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "WindPowerTag.h"

class CStatWindPowerPerf : public CCalcBlock  
{
public:
	CStatWindPowerPerf();
	virtual ~CStatWindPowerPerf();
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
	CString m_strDBName;
	CString m_strTableName;
	CString m_strTurbine;
	int m_iPowerPorTagMin;//并网标识最小值
	int m_iPowerPorTagMax;//并网标识最大值	
	double m_dPowerLimit;//限电标识
	double m_dMinParWindSpeed;//最小切入风速
	double m_dMaxParWindSpeed;//风电机组85%额定功率输出时风速

private:
	void delTagWindPowerMinMaxArr(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CUniValue  &CUV,
		int iMin,
		int iMax											
		);
	void delTagWindPowerArr(//删除限电、并网、对风角度
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CUniValue  &CUV,
		double iDelTag,//删除值的标识
		int iMaxMinTag//0:删除等于，1删除小于；2删除大于
		);
	void delWindPowerArr(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//删除需要删除的数据

	void getAvgAndVar(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CArray<int,int> &iDiffGroupNums);//得到bin分组内的均值和方差
	
	void getAvgAndVar(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//得到均值和方差

	void delWindPowerArrByAvgVar(
								CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//根据均值和方差进行过滤
	void getBinGroup(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		double dMinBinWSpeed,
		double dMaxBinWSpeed,
		CArray<int,int> &iDiffGroupNums);//bin分组
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	void writeDataToTable(CString strTurbine,CString strTableName,WindPowerTag &oneTag);
};

#endif // !defined(AFX_STATWINDPOWERPERF_H__16B7F97D_2411_49D8_BE67_1A875E1D838E__INCLUDED_)
