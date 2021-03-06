// RTPointAdvBlock.h: interface for the CRTPointAdvBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPOINTADVBLOCK_H__4954494E_E617_4004_BFC0_799BB3DC0824__INCLUDED_)
#define AFX_RTPOINTADVBLOCK_H__4954494E_E617_4004_BFC0_799BB3DC0824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>

class CRTPointAdvBlock : public CCalcBlock  
{
public:
	CRTPointAdvBlock();
	virtual ~CRTPointAdvBlock();
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

private:
	void BubbleSort(double *pData,int iCount);
	std::vector<int> m_HaveValueIndex;//记录下那几个变量被配置了
public:
	//定义预处理的策略属性
	CString	n_strTagDesp;
	CString n_strTagName1;
	CString n_strTagName2;
	CString n_strTagName3;
	CString n_strTagName4;
	CString n_strTagName5;
	
	int		n_iOutMethod;	
	BOOL	n_bChkLimit;
	double	n_dHighLimit;
	double	n_dLowLimit;
	double	n_dConstWhenBad;
	BOOL	n_bForceEnable;	
	double	n_dForceValue;

	int m_iInOrOut;//1,内部；0，外部
	long m_lOffsetTime;
};

#endif // !defined(AFX_RTPOINTADVBLOCK_H__4954494E_E617_4004_BFC0_799BB3DC0824__INCLUDED_)
