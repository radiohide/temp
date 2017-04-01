// BlockPropData.h: interface for the BlockPropData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKPROPDATA_H__8EFCDFEB_92B7_45A9_8F38_BB9D91B1727C__INCLUDED_)
#define AFX_BLOCKPROPDATA_H__8EFCDFEB_92B7_45A9_8F38_BB9D91B1727C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BlockPropData  
{
public:
	CString block_name;
	CString prop_name;
	CString value_item1;
	CString value_item2;
	CString value_item3;
	CString value_item4;
	CString value_item5;
public:
	BlockPropData();
	virtual ~BlockPropData();

};

#endif // !defined(AFX_BLOCKPROPDATA_H__8EFCDFEB_92B7_45A9_8F38_BB9D91B1727C__INCLUDED_)
