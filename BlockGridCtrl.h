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
	
	//���汾�ؼ���Ҫ��ʾ�ļ��������б�
	CBlockList  m_blocks;
	//�������е���ʾ�����
	void  UpdateAllBlocks();
	//���һ����
	void AddBlock(CCalcBlock* pBlock);
	void RemoveBlock(CCalcBlock* pBlock);
	//�жϵ���������ĸ���
	



};

#endif // !defined(AFX_BLOCKGRIDCTRL_H__782E0353_C97E_4926_977D_CB58C0E367CB__INCLUDED_)
