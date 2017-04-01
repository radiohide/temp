// StatProEnumBlock.h: interface for the CStatProEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATPROENUMBLOCK_H__456981E1_F594_4913_AF86_D6E54C71C440__INCLUDED_)
#define AFX_STATPROENUMBLOCK_H__456981E1_F594_4913_AF86_D6E54C71C440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatProEnumBlock : public CCalcBlock  
{
public:
	CStatProEnumBlock(int iTimeSelfOrInput);
	virtual ~CStatProEnumBlock();
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
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入

	double	m_dLeft;//区间左（闭区间）
	double	m_dRight;//区间右（闭区间）
	short	m_iStatType;//状态值
	int		m_iFunType;//统计方法
	double m_dGain;//统计结果方法倍数
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	void statDataCount(CUniValue  &CUV,//统计次数
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
	void statDataTimeLength(CUniValue  &CUV,//统计时长
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
	bool isInRange(const double &dValue,const double &dLeft,const double &dRight);

};

#endif // !defined(AFX_STATPROENUMBLOCK_H__456981E1_F594_4913_AF86_D6E54C71C440__INCLUDED_)
