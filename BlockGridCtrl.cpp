// BlockGridCtrl.cpp: implementation of the CBlockGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "BlockGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int nStartRow = 2;
const int BlockHeadRows = 2;

CBlockGridCtrl::CBlockGridCtrl()
{
	m_pBlock = NULL;
}

CBlockGridCtrl::~CBlockGridCtrl()
{
		m_blocks.RemoveAll();
}
void  CBlockGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
	//CString str1;
	//str1.Format("row:%d,col:%d,%s",nRow,nCol,str);
	//AfxMessageBox(str1);
	//当编辑结束后，需要修改相应的对象属性，首先需要给出当前的
	//GetCellInfo(nRow,nCol); //获得是否是输入端口，
	//BLOCK_NODEINFO* pNodeInfo;

}

void CBlockGridCtrl::SetBlock(CCalcBlock *pBlock)
{
	m_pBlock = pBlock;
}

void CBlockGridCtrl::ShowBlock(CCalcBlock *pBlock)
{
	m_pBlock = pBlock;

	int inNums = pBlock->GetInputNum();
	int OutNums = pBlock->GetOutputNum();
	int NodeNums =  inNums <= OutNums ? OutNums:inNums;

	//int nStartRow = 2;
	//int BlockHeadRows = 2;
	int RowCount = NodeNums*4 +(nStartRow + BlockHeadRows);
	int ColCount = 6;
	/////////////////////////////////////
	DeleteAllItems();
	SetRowCount(RowCount);
	SetColumnCount(ColCount);
	SetFixedRowCount(0);
	SetFixedColumnCount(0);
	SetColumnWidth(0,100);
	SetColumnWidth(1,20);
	SetColumnWidth(2,100);
	SetColumnWidth(3,30);
	SetColumnWidth(4,100);
	SetColumnWidth(5,20);

	//for header
	int row=0;
	int col=2;
	for(row=0;row<RowCount;row++)
	{
		COLORREF bkColor;
		COLORREF ftColor;
		if(row<nStartRow)
		{
			continue;
		}
		else if(row<nStartRow+BlockHeadRows)
		{
			bkColor = BLOCK_TITLE_BKCOLOR;
			ftColor = BLOCK_TITLE_FTCOLOR;
		}
		else
		{
			bkColor = BLOCK_BODY_BKCOLOR;
			ftColor = BLOCK_BODY_FTCOLOR;
		}
		for(col=2;col<5;col++)
		{
			SetItemBkColour(row,col,bkColor);
			SetItemFgColour(row,col,ftColor);
		}
	}
	SetItemText(nStartRow+0,2,pBlock->GetBlockName());
	SetItemText(nStartRow+1,2,pBlock->GetBlockDesp());
	///////////////////////////////////////////////////
	//for input
	int i=0;
	int j=0;
	for(i=0;i<inNums;i++)
	{
		CCalcPort* pPort = pBlock->GetInputPortObj(i);
        row = i*4+(nStartRow + BlockHeadRows); //该端口的行位置开始
		SetItemText(row,0,pPort->GetPortLinkID());//from block

		SetItemText(row,  2,pPort->GetPortDesp());
		SetItemText(row+1,2,pPort->GetPortID());
		CString strLink = pPort->GetPortLinkID();
		if(strLink.IsEmpty())
		{
		    SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[0]);//设置输入的端口颜色
		}
		else
		{
			SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[1]);//设置输入的端口颜色
		}
		SetItemFgColour(row+1,1,BLOCK_INPUT_FTCOLOR);
	}
	//for output 
	for(i=0;i<OutNums;i++)
	{
		CCalcPort* pPort = pBlock->GetOutputPortObj(i);
        row = i*4+(nStartRow + BlockHeadRows); //该端口的行位置开始
		SetItemText(row,  4,pPort->GetPortDesp()); //desp
		SetItemText(row+1,4,pPort->GetPortID());
		SetItemBkColour(row+1,5,BLOCK_OUPUT_BKCOLOR[1]);//设置输出的端口颜色
		SetItemFgColour(row+1,5,BLOCK_INPUT_FTCOLOR);
	}
}
//通过对行列的情况，给出是什么样的块属性，是输入/输出，是输入的名称、描述
//还是block的名称和描述，还是无效的地方
void CBlockGridCtrl::GetCellInfo(int nRow, int nCol, BLOCK_NODEINFO *pCellInfo)
{
	if(m_pBlock==NULL) {pCellInfo->bValid =FALSE;return;}
	//判断是否为块的名称、描述

	//判断是否为输入输出端子

	//判断是否为输入描述，名称

	//判断是否为输入端的连接模块信息
}

//根据对象列表，画出所有的块，画的时候：
//1、计算需要多少列，多少行，计算每个块的高度，然后给每个块的坐标赋值，然后画出每个
void  CBlockGridCtrl::UpdateAllBlocks()
{
	//开始画出各个块
}
//添加一个块
void CBlockGridCtrl::AddBlock(CCalcBlock* pBlock)
{
	 m_blocks.AddHead(pBlock);
	 UpdateAllBlocks();
}
void CBlockGridCtrl::RemoveBlock(CCalcBlock* pBlock)
{
	 POSITION pos = m_blocks.Find(pBlock);
	 if(pos!=NULL)
	 {
		 m_blocks.RemoveAt(pos);
		 UpdateAllBlocks();
	 }
	 
}