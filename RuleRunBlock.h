// RuleRunBlock.h: interface for the CRuleRunBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_)
#define AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "RuleData.h"

class CRuleRunBlock : public CCalcBlock  
{
public:
	CRuleRunBlock();
	virtual ~CRuleRunBlock();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//添加必要的虚函数：
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //画特征形状以区别Add at 2011-10-30
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
public:
//定义自身特色的函数===============================================
	void SetDBConnectString(CString str);
	CString GetDBConnectString();
	void SetRuleDBName(CString str);
	CString GetRuleDBName();
	CString getABSDirRuleDB();
	BOOL EstablishMDBConnectPtr(CString strFileName);
	int SetRuleBlockInputOutputByDB(CString strDBConnect,CString strRuleDBName);
	int GetDisInterval(	CString strComFun, 
		double dPointValue,
		double dStepValue,	
		double dSValue,//离散化范围的开始
		double dEValue,//离散化范围的结束
		CArray<int,int> &disValueArr,
		CArray<double,double> &sValueArr,
		CArray<double,double> &eValueArr);
	int GetDisInterval(	CString strComFun, 
		double dPointValue,
		double dStepValue,	
		int iLeftNum,//左离散化个数
		int iRightNum,//右离散化个数
		CArray<int,int> &disValueArr,
		CArray<double,double> &sValueArr,
		CArray<double,double> &eValueArr);
	int getDisValueByName(CString strVarName,double dValue);
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void readRuleData();//读取规则信息
	void readParData();//读取变量信息
	void resetBeforeCalc();//在计算前进行复位
	void setBDValueByName(CString strVarName,double dValue);
	void setFDValueByName(CString strVarName,double dValue);
	CRuleData* getMinDiffRuleData();
	void calcDiffValue();
	double getDValueByDis(CString strVarName,int disValue);
	void setConToRuleData(CString *pStrConCode);
private:
 //定义自身特色的属性
	//连接的字符串，用于构造数据连接，读取相应的规则到内存中
	_ConnectionPtr m_pConRule; //连接导出规则库
	BOOL m_bCon;//是否连接规则库
	CString m_strDBConnect;
	CString m_strRuleDBName;
	CString m_strRuleDBDesp;
	int  m_iTagToFOrB;//前向推理和反向推理标记，0:前向(eg:从状态到性能)(默认),1:后向(eg:从状态到操作)
	int  m_iTagWeightOrTime;//按照权重最大还是按照时间最近来进行规则匹配。0:权重最大；1:时间最近

	CStringArray nameFArr;
	CStringArray despFArr;
	CArray<int,int> tagBeInFArr;//参与规则推导的前件变量标记,1:参与,0:不参与
	CArray<double,double> dValueFArr;//前件Value值
	CArray<int,int> iDisValueFArr;//前件的离散值

	CStringArray nameBArr;
	CStringArray despBArr;
	CArray<int,int> tagBeInBArr;//参与规则推导的后件变量标记,1:参与,0:不参与
	CArray<double,double> dValueBArr;//后件Value值
	CArray<int,int> iDisValueBArr;//后件的离散值

	CStringArray nameArrDB;
	CStringArray typeFBArrDB;//前件、后件标志
	CStringArray disMethodArrDB;
	CArray<double,double> disRefValueArrDB;
	CArray<double,double> disStepArrDB;

	CArray<CRuleData*,CRuleData*&> ruleDataArr;
};

#endif // !defined(AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_)
