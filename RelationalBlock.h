// RelationalBlock.h: interface for the CRelationalBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_)
#define AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRelationalBlock : public CCalcBlock  
{
public:
	CRelationalBlock();
	virtual ~CRelationalBlock();
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

	CString m_strFuncName;//��ǰ�ķ�����
	CString m_strBlockDisp;//���������
	CString m_strPropertityTableName;//���Դ�������
};

#endif // !defined(AFX_RELATIONALBLOCK_H__57D12BCA_0D85_4217_808D_F78847F13E74__INCLUDED_)
