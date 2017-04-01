// CalcModule.h: interface for the CCalcModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_)
#define AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//这是一个计算模块，可以容纳多个计算块，模块具有顺序属性
#include "CalcBlock.h"

class CCalcModel;
class CCalcModule:public CObject
{
public:
	CCalcModule();
	virtual ~CCalcModule();

	CCalcBlock* GetBlockByIndex(int k);
	CCalcBlock* FindBlockByName(CString str);
	void AddBlock(CCalcBlock* pBlock);

	void CheckBlockCalcIntegrity(CStringArray& LogStrings);
	void AdjustBlockCalcOrder(); //根据给予的号码，调整计算的顺序
	void InitforCalc(CCalcModel* pModel);
	void Calc();

	void ClearAllBlocks(); //清除所有的计算块

	void SetModuleName(CString str){strcpy(m_Name,str);}
	CString GetModuleName() {CString str =m_Name; return str;}
	void SetModuleDesp(CString str){strcpy(m_Desp,str);}
	CString GetModuleDesp() {CString str =m_Desp; return str;}
	void SetParentName(CString *pstr) {m_pStrParentModelName=pstr;}
	CString GetParentName() {return *m_pStrParentModelName;}
	void SetCalcOrderID(int orderID);
	int GetCalcOrderID();
	int GetCounterByBType(CString strBType);//得到某类型计算块的个数
public:
	CString FindCanUseBlockName(CString strBlockType=_T(""));
	int GetBlockCount();
	void GetBlockInputValueFromGlobalWS();
	void SetWriteLogOnOrOff(int iOnOrOff);//设置日志开关
	void SetTraceOnOrOff(int iOnOrOff);//设置追踪开关
	void SetDebugOnOrOff(int iOnOrOff);//设置调试开关
	void RunCompile();
	void InitCompile(int *pModelWarningCount,int *pModelErrorCoutn,CWnd *pWnd,int iMsg,CLog *pLog);
	void ResetStopCalc();//停止标志复位//for stop
	void StopCalc();//for stop
	BOOL CheckTableExist(_ConnectionPtr &pCon);
	BOOL ReadModule(_ConnectionPtr &pCon);
	
	CCalcBlock* CreateBlock(CString strType,CString strName,CString strDesp);

	char m_Name[128]; //模块名字,将作为模块列表的名称为英文
	char m_Desp[128]; //模块描述

    CString* m_pStrParentModelName;

	CBlockList m_BlockList;

	BOOL m_bCompared; //说明排序处理过的临时变量
protected:
	//计算的顺序
	int m_iCalcOrderID;
private:
	 CStringArray s_BlockTypeArray;
	 CStringArray s_BlockIDArray;
	int m_iStopTag;//0:运行；1:停止//for stop

	int m_iModuleWarningCount;
	int	m_iModuleErrorCount;
	int *m_pModelWarningCount;
	int *m_pModelErrorCoutn;
	CWnd *m_pWnd;
	int m_iMsg;
	CLog *pLog;
	int m_iLogOnOrOff;//日志开或关1:开，0：关
	int m_iTraceOn;//追踪开关1:开，0:关
	int m_iDebugOn;//调试开关1:开，0:关
};

#endif // !defined(AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_)
typedef CTypedPtrList<CObList,CCalcModule*> CModuleList;