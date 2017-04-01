// SysVar.h: interface for the SysVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_)
#define AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include   <math.h> 
class CCalcModel;
class SysVar  
{
public:
	CString ID;
	CString Name;
	CString Desp;
	CString unit;
	CString var_expression;
	int type;//����ģ��
	double category_endpoint;//�˵�
	double fixed_interval;//�̶�����
	double relative_interval;//��Բ���
	CString cluster_source;//��ɢ����ʽ
public:
	SysVar();
	virtual ~SysVar();
	void SetValues(double category_endpoint,double fixed_interval,
		double relative_interval,CString cluster_source);
	int UpDateDB();

	int GetSelfDisInterval(int iLeftNum,//����ɢ������
						int iRightNum,//����ɢ������
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

	int GetSelfDisInterval(double dSValue,//��ɢ����Χ�Ŀ�ʼ
						double dEValue,//��ɢ����Χ�Ľ���
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

	int GetDisInterval(	CString strComFun, 
						double dPointValue,
						double dStepValue,	
						int iLeftNum,//����ɢ������
						int iRightNum,//����ɢ������
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);
	
	int GetDisInterval(	CString strComFun, 
						double dPointValue,
						double dStepValue,	
						double dSValue,//��ɢ����Χ�Ŀ�ʼ
						double dEValue,//��ɢ����Χ�Ľ���
						CArray<int,int> &disValueArr,
						CArray<double,double> &sValueArr,
						CArray<double,double> &eValueArr);

};

#endif // !defined(AFX_SYSVAR_H__87F52521_7A0A_4CAF_9AC1_268BCAE76036__INCLUDED_)
