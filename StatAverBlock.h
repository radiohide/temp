// StatAverBlock.h: interface for the CStatAverBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_)
#define AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatCalcBlock.h"

class CStatAverBlock : public CStatCalcBlock  
{
public:
	CStatAverBlock();
	virtual ~CStatAverBlock();
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
public:
	CArray<double,double> m_dAvgArr;
	CArray<double,double> m_dMaxArr;
	CArray<double,double> m_dMinArr;
private:
	void writeDataToDB(CString strVarName,CString strUnitSet,
						long lSTime,long lETime,
						double dAvgValue,double dMaxValue,double dMinValue);
};

#endif // !defined(AFX_STATAVERBLOCK_H__2CB04828_C296_43A8_B292_44706DA23259__INCLUDED_)
