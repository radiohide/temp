// ConDataTag.h: interface for the ConDataTag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDATATAG_H__CD2C1CDB_817D_4031_B4DF_D09BF08F43FB__INCLUDED_)
#define AFX_CONDATATAG_H__CD2C1CDB_817D_4031_B4DF_D09BF08F43FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ConDataTag  
{
public:
	ConDataTag();
	virtual ~ConDataTag();
public:
	CString condition_code;
	int record_numble;
	CString load_desp;
	CString coal_desp;
	CString temp_desp;
	int RID_early;
	int RID_late;
	int BeHaveTag;//历史中是否存在标志，初始为-1，存在为1，不存在为0；
	int record_numble_hist;//历史中存在的稳态记录个数

};

#endif // !defined(AFX_CONDATATAG_H__CD2C1CDB_817D_4031_B4DF_D09BF08F43FB__INCLUDED_)
