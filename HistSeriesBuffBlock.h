// HistSeriesBuffBlock.h: interface for the CHistSeriesBuffBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_)
#define AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgHistSeriesBuff.h"
#include <vector>
class CHistSeriesBuffBlock : public CCalcBlock  
{
public:
	CHistSeriesBuffBlock();
	virtual ~CHistSeriesBuffBlock();

	CList<TagData*,TagData*>  m_listDataBuffer;
	CString	m_strPointName;
	int		m_iDataSource;
	int		m_iReadModel;
	int		m_ISnapStep;
	int		m_iOutputTime;
	int		m_iBuffTime;

	HINSTANCE  hDLL;

	//1、缓冲区时间应大于等于输出时间，缓冲区才有存在的意义
	//2、如果缓冲区满了，向缓冲区尾部插入的同时，需要删除头部多余数据
	//3、第一次填充缓冲区时，应填满，否则第一次输出时，从缓冲区中取的数据可能不完整
	//此变量用来解决3而设计的
	BOOL  bSureFull;
	//将稳态时间段截小 10分钟一段
	std::vector<long> Temp_begintime;
	std::vector<long> Temp_endtime;
public:
	void RemoveElementFromBuffer(long lEndOfInputTime);
	void Separate(long stablebegin, long stableend);
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

};

#endif // !defined(AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_)
