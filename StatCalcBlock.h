// StatCalcBlock.h: interface for the CStatCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_)
#define AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatCalcBlock : public CCalcBlock  
{
public:
	CStatCalcBlock();
	virtual ~CStatCalcBlock();
//��ӱ�Ҫ���麯����
//-----------------------------------------------------------------------------------------------------------
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	virtual void Calc();//���м���ĺ���,��Ҫ����
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
	CString m_strBlockDesp;
	CString m_strTitle;
protected:
	BOOL IsTableExist(_ConnectionPtr &pConnection, CString strTableName);
public:
	CString m_strDBName;
	int		m_iSETimeType;//0:ϵͳʱ��Ρ�1:�ⲿ����ʱ���.2:����ʱ���
	int m_iVarType;

	CStringArray m_strVarNameArr;//��������
	CStringArray m_strVarDespArr;//��������
	CStringArray m_strVarUnitArr;//��������
	CStringArray m_strUnitSetArr;//��������

	CArray<long,long> m_sTimeArr;//��ʼʱ��
	CArray<long,long> m_eTimeArr;//����ʱ��
};

#endif // !defined(AFX_STATCALCBLOCK_H__EB889017_34F0_4562_9588_C10225A1A4B2__INCLUDED_)
