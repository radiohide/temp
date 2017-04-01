// WASPCalcBlock.h: interface for the CWASPCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_)
#define AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
//��calc�����������ġ�
class CWASPCalcBlock : public CCalcBlock  
{
public:
	CWASPCalcBlock();
	virtual ~CWASPCalcBlock();
public:
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
protected:
	RTFUNC rtFunc;//�ⲿ����ָ��
	CString m_strFuncName;//���õĺ�����
	TAGINFO* PSourceValue;//��ʽ����ʱ��׼��������
	TAGINFO* pG;//��ʽ��������ֵ
	CStringArray m_strArrFunDisp;//���з�������
	CStringArray m_strArrFunName;//���з�������
	CStringArray m_strArrOutPortName;//����Ķ˿�����
	CString m_strBlockDisp;//���������
};

#endif // !defined(AFX_WASPCALCBLOCK_H__956487A0_D1CC_4D31_A3AB_90D5BA304D61__INCLUDED_)
