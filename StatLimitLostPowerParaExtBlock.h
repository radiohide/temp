// StatLimitLostPowerParaExtBlock.h: interface for the CStatLimitLostPowerParaExtBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATLIMITLOSTPOWERPARAEXTBLOCK_H__F4484322_4BA7_410C_AC49_43F3CDD932BC__INCLUDED_)
#define AFX_STATLIMITLOSTPOWERPARAEXTBLOCK_H__F4484322_4BA7_410C_AC49_43F3CDD932BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatLimitLostPowerParaExtBlock : public CCalcBlock  
{
public:
	CStatLimitLostPowerParaExtBlock();
	virtual ~CStatLimitLostPowerParaExtBlock();
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
public:
	
	CArray<double,double> dWindSpeedArr;//功率曲线--风速
	CArray<double,double> dPowerArr;//功率曲线--功率
	
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
	double dMaxPower;//最大有功功率给定值
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	double getPowerFromWindSpeed(double dWindSpeed);
	double getLimitPowerByTime(CUniValue &CUV,long lTime);
	char* GetParaExtName();
	void GetStateLostPara(const char* FileName);
};

#endif // !defined(AFX_STATLIMITLOSTPOWERPARAEXTBLOCK_H__F4484322_4BA7_410C_AC49_43F3CDD932BC__INCLUDED_)
