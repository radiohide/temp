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
	//���༭��������Ҫ�޸���Ӧ�Ķ������ԣ�������Ҫ������ǰ��
	//GetCellInfo(nRow,nCol); //����Ƿ�������˿ڣ�
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
        row = i*4+(nStartRow + BlockHeadRows); //�ö˿ڵ���λ�ÿ�ʼ
		SetItemText(row,0,pPort->GetPortLinkID());//from block

		SetItemText(row,  2,pPort->GetPortDesp());
		SetItemText(row+1,2,pPort->GetPortID());
		CString strLink = pPort->GetPortLinkID();
		if(strLink.IsEmpty())
		{
		    SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[0]);//��������Ķ˿���ɫ
		}
		else
		{
			SetItemBkColour(row+1,1,BLOCK_INPUT_BKCOLOR[1]);//��������Ķ˿���ɫ
		}
		SetItemFgColour(row+1,1,BLOCK_INPUT_FTCOLOR);
	}
	//for output 
	for(i=0;i<OutNums;i++)
	{
		CCalcPort* pPort = pBlock->GetOutputPortObj(i);
        row = i*4+(nStartRow + BlockHeadRows); //�ö˿ڵ���λ�ÿ�ʼ
		SetItemText(row,  4,pPort->GetPortDesp()); //desp
		SetItemText(row+1,4,pPort->GetPortID());
		SetItemBkColour(row+1,5,BLOCK_OUPUT_BKCOLOR[1]);//��������Ķ˿���ɫ
		SetItemFgColour(row+1,5,BLOCK_INPUT_FTCOLOR);
	}
}
//ͨ�������е������������ʲô���Ŀ����ԣ�������/���������������ơ�����
//����block�����ƺ�������������Ч�ĵط�
void CBlockGridCtrl::GetCellInfo(int nRow, int nCol, BLOCK_NODEINFO *pCellInfo)
{
	if(m_pBlock==NULL) {pCellInfo->bValid =FALSE;return;}
	//�ж��Ƿ�Ϊ������ơ�����

	//�ж��Ƿ�Ϊ�����������

	//�ж��Ƿ�Ϊ��������������

	//�ж��Ƿ�Ϊ����˵�����ģ����Ϣ
}

//���ݶ����б��������еĿ飬����ʱ��
//1��������Ҫ�����У������У�����ÿ����ĸ߶ȣ�Ȼ���ÿ��������긳ֵ��Ȼ�󻭳�ÿ��
void  CBlockGridCtrl::UpdateAllBlocks()
{
	//��ʼ����������
}
//���һ����
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