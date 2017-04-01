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
	int BeHaveTag;//��ʷ���Ƿ���ڱ�־����ʼΪ-1������Ϊ1��������Ϊ0��
	int record_numble_hist;//��ʷ�д��ڵ���̬��¼����

};

#endif // !defined(AFX_CONDATATAG_H__CD2C1CDB_817D_4031_B4DF_D09BF08F43FB__INCLUDED_)
