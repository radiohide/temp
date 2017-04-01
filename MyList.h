// MyList.h: interface for the CMyListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLIST_H__AB1185EA_C737_46D3_9019_A58476A16BD0__INCLUDED_)
#define AFX_MYLIST_H__AB1185EA_C737_46D3_9019_A58476A16BD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CMyListItem : public CObject
{
public:
	CMyListItem(int x, int y, COLORREF clrText, COLORREF clrBckg) :
		m_x (x),
		m_y (y),
		m_clrText (clrText),
		m_clrBckg (clrBckg)
	{
	}

	const int	m_x;
	const int	m_y;
	COLORREF	m_clrText;
	COLORREF	m_clrBckg;
};

class CMyListCtrl : public CBCGPListCtrl  
{
public:
	void DisplayBlock(CCalcBlock* pBlock=NULL);
	void SetCellColor(int row, int col, COLORREF bk, COLORREF frt);
	//CMyListCtrl();
	virtual ~CMyListCtrl ()
	{
		while (!m_ItemColors.IsEmpty ())
		{
			delete m_ItemColors.RemoveHead ();
		}
	}

	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);

	CList<CMyListItem*,CMyListItem*>	m_ItemColors;
};

#endif // !defined(AFX_MYLIST_H__AB1185EA_C737_46D3_9019_A58476A16BD0__INCLUDED_)
