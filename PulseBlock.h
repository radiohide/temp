// PulseBlock.h: interface for the CPulseBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PULSEBLOCK_H__EDA91F79_02C0_4C87_A514_13DE2789178F__INCLUDED_)
#define AFX_PULSEBLOCK_H__EDA91F79_02C0_4C87_A514_13DE2789178F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgPulse.h"

class CPulseBlock : public CCalcBlock  
{
public:
	int m_iComIndex;
	double	m_dFengzhi;
	double	m_dFengzhiNegative;

	double	m_dZhouqi;
	double	m_dYanchi;
	double	m_dMaichongKD;

	double	m_dInitValue;
	double	m_dStep;

	int m_iInputNum;
public:
	CPulseBlock();
	virtual ~CPulseBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
	virtual void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="");
	//��������ʾ��ͬ�����Զ���Ի���
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
	//�����Խ���ģ��д�뵽���ݿ���
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord);
	//����ͼ��ʱ����Ҫ�������ԡ�
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//�����Է��ô���������
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);

};

#endif // !defined(AFX_PULSEBLOCK_H__EDA91F79_02C0_4C87_A514_13DE2789178F__INCLUDED_)
