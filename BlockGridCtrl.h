// BlockGridCtrl.h: interface for the CBlockGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKGRIDCTRL_H__782E0353_C97E_4926_977D_CB58C0E367CB__INCLUDED_)
#define AFX_BLOCKGRIDCTRL_H__782E0353_C97E_4926_977D_CB58C0E367CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GRIDCTRL_SRC\GridCtrl.h"

#include "CalcBlock.h"

class CBlockGridCtrl : public CGridCtrl  
{
public:
	CBlockGridCtrl();
	virtual ~CBlockGridCtrl();

	virtual void  OnEndEditCell(int nRow, int nCol, CString str);
public:
	void GetCellInfo(int nRow,int nCol,BLOCK_NODEINFO* pNodeInfo);
	void ShowBlock(CCalcBlock* pBlock);
	void SetBlock(CCalcBlock* pBlock);
	CCalcBlock* m_pBlock;
///////////////////////////////////////////////////////////////////////////////////////////////
	
	//保存本控件需要显示的计算块对象列表
	CBlockList  m_blocks;
	//更新所有的显示计算块
	void  UpdateAllBlocks();
	//添加一个块
	void AddBlock(CCalcBlock* pBlock);
	void RemoveBlock(CCalcBlock* pBlock);
	//判断点击到的是哪个块
	



};

#endif // !defined(AFX_BLOCKGRIDCTRL_H__782E0353_C97E_4926_977D_CB58C0E367CB__INCLUDED_)
