// BlockListData.h: interface for the BlockListData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKLISTDATA_H__802E85D4_DC7D_4F93_B978_C51BE869BDBF__INCLUDED_)
#define AFX_BLOCKLISTDATA_H__802E85D4_DC7D_4F93_B978_C51BE869BDBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BlockListData  
{
public:
	CString name;
	CString desp;
	CString type;
	int orderid;
	double left;
	double top;
	double right;
	double bottom;
	int InNum;
	int OutNum;
	CString InNames;
	CString InValueTypes;
	CString OutNames;
	CString OutValueTypes;
	CString InputLinkStrs;
public:
	BlockListData();
	virtual ~BlockListData();

};

#endif // !defined(AFX_BLOCKLISTDATA_H__802E85D4_DC7D_4F93_B978_C51BE869BDBF__INCLUDED_)
