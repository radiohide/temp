// StableJudgeBlock.h: interface for the CStableJudgeBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_)
#define AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgStableJudge.h"

class CStableJudgeBlock : public CCalcBlock  
{
public: 

	CStableJudgeBlock();
	virtual ~CStableJudgeBlock();

	double	m_dDown2;
	double	m_dDown1;
	double	m_dFazhi1;
	double	m_dFazhi2;
	int		m_iRadio;
	int m_iCalcNoStable;
public:
	//必须重载的函数
	//根据不同的对象进行构造固定的输入输出,默认的输入输出
	virtual void ConstructBlockInputOutput(); 
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
	BOOL getSmoothArr(const CArray<double,double> &dOrigValueArr,
		int iSmoothStep,
		int iSmoothMinSize,
		CArray<double,double> &dSmoothArr);
	void getDisValue(const CArray<double,double> &dSmoothArr,
		double dFBDataPart,
		double &dFDownPer,
		double &dFUpPer,
		double &dBDownPer,
		double &dBUpPer);
	int getUpOrDown(double dFBPartBeVauePer,
		double dFDownPer,
		double dFUpPer,
		double dBDownPer,
		double dBUpPer);
	double getVariance(const CArray<double,double> &dOrigValueArr);
	void getVariance(const CArray<double,double> &dOrigValueArr,
		int iPartNum,
		CArray<double,double> &dRealVarArr);
};

#endif // !defined(AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_)
