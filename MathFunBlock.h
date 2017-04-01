// MathFunBlock.h: interface for the CMathFunBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHFUNBLOCK_H__EA45456F_069E_4BAB_AA82_1D4988D9FD47__INCLUDED_)
#define AFX_MATHFUNBLOCK_H__EA45456F_069E_4BAB_AA82_1D4988D9FD47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CMathFunBlock : public CCalcBlock  
{
public:
	CMathFunBlock();
	virtual ~CMathFunBlock();

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
public:
	CStringArray m_strArrFunName;//���з�������
	CStringArray m_strArrFunDisp;//���з�������	

	CStringArray m_strArrInPortName;//����Ķ˿�����
	CArray<int,int> m_iArrInFunIndex;//����˿�����Ӧ�ķ�����������m_strArrInPortName�����ʹ��

	CStringArray m_strArrOutPortName;//����Ķ˿�����
	CArray<int,int> m_iArrOutFunIndex;//����˿�����Ӧ�ķ�����������m_strArrOutPortName�����ʹ��

	CString m_strFuncName;//��ǰ�ķ�����
	CString m_strBlockDisp;//���������
	CString m_strPropertityTableName;//���Դ�������
};

#endif // !defined(AFX_MATHFUNBLOCK_H__EA45456F_069E_4BAB_AA82_1D4988D9FD47__INCLUDED_)
