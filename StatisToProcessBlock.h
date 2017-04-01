// StatisToProcessBlock.h: interface for the CStatisToProcessBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTOPROCESSBLOCK_H__2B42BD39_82B4_449B_A908_424433A1FB6A__INCLUDED_)
#define AFX_STATISTOPROCESSBLOCK_H__2B42BD39_82B4_449B_A908_424433A1FB6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatisToProcessBlock : public CCalcBlock  
{
public:
	CStatisToProcessBlock();
	virtual ~CStatisToProcessBlock();

public:
	//�������صĺ���
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput(); 
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
	int	m_nProcType;//����ֵ���ͣ�0 - 2 Ĭ�� 0  (����)
	int iTimeSelect;//��עʱ�䣺0 ��ʼʱ�䣻1 ��ֹʱ�� 2 ����ʱ�� (����)
	CStringArray strInputArr;//�������
	CStringArray strOutputArr;//�������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ

};

#endif // !defined(AFX_STATISTOPROCESSBLOCK_H__2B42BD39_82B4_449B_A908_424433A1FB6A__INCLUDED_)