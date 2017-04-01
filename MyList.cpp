// MyList.cpp: implementation of the CMyList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COLORREF CMyListCtrl::OnGetCellTextColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrText != -1)
			{
				return pItem->m_clrText;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellTextColor (nRow, nColumn);
}

COLORREF CMyListCtrl::OnGetCellBkColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrBckg != -1)
			{
				return pItem->m_clrBckg;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellBkColor (nRow, nColumn);
}

void CMyListCtrl::SetCellColor(int row, int col, COLORREF bk, COLORREF frt)
{
    CMyListItem* pItem = NULL;

	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pListItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pListItem);

		if (pListItem->m_x == col && pListItem->m_y == row)
		{
			pItem = pListItem;
			break;
		}
	}

	if (pItem == NULL)
	{
		pItem = new CMyListItem (col, row, frt, bk);
		m_ItemColors.AddTail (pItem);
	}
	else
	{
		pItem->m_clrText = frt;
		pItem->m_clrBckg = bk;
	}

	RedrawWindow ();
}
//根据块来显示行列
void CMyListCtrl::DisplayBlock(CCalcBlock *pBlock)
{
    DeleteAllItems();
}
