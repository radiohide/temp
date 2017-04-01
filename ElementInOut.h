// ElementInOut.h: interface for the CElementInOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_)
#define AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkSpaceElement.h"
#include "BTree.h"
#include "PoteDll.h"
#include "CalcBlock.h"
class CBTree;
//该对象是
class CElementInOut :public CWorkSpaceElement
{
public:
	void SetBeCalc();
	void ReSet();//复位b_Calced
	CElementInOut(CString tLongID,int dType,CCalcBlock* pBlock);//初始化来源点
	CElementInOut(CString tLongID,int dType,CString Formula,CCalcBlock* pBlock);//初始化计算点
	~CElementInOut();

	int ParseFormula();//解析公式
	int CalcFormula();//计算点值(计算点)
	double ReportValue (CBTree* pnod);

	void SetParentBlock(CCalcBlock* pBlock);
	void SetPtrMap(CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> *m_pWorkSpaceList);
	CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> *m_pWorkSpaceList;
	CCalcBlock *m_pParentBlock;
	CString LongID;		//点号
	TAGINFO *pDataAll;//存储原始数据
	int NumData;//记录到的原始数据的个数

	RTFUNC rtFunc;//外部函数指针
	TAGINFO* PSourceValue;//针对统计点的来源点数据
	TAGINFO* pG;

	CBTree* m_BTree;	//公式表达式的二叉树
	int	PType;			//点类型，0为采集点，1为计算点，2为系数点
	int DType;			//点类型，0为单值点，1为序列点
	CString PFormula;	//点的计算公式
	double PValue;		//点的结果（包括采集数据、和根据公式计算数据）
	int PState;			//标识是否已经得到正确点值
	time_t PTime;		//标识是否已经得到正确点值


	double GetValue();
	
	BOOL b_Calced;	//
	
private:
	int	m_Position;		//表达式字符分析指针
	bool IsFormula;		//标识公式是否正确
	//	RTFUNC rtFunc;
	//	TAGINFO* pG;
	bool IsDigit(char byte);
	CString Substring(CString str,int pos1,int count);
	bool IsLetter(char byte);
	
	void SkipSpaces();
	CBTree* pmOP();
	CBTree* mdOP();
	CBTree* involOP();
	CBTree* logicOP();
	CBTree* logsecOP();
	CBTree* factorOP();
	CBTree* identiOP();
};

#endif // !defined(AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_)
//typedef CTypedPtrList<CObList,CElementInOut*> CElementObjList;