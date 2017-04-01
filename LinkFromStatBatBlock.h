// LinkFromStatBatBlock.h: interface for the CLinkFromStatBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_)
#define AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CLinkFromStatBatBlock : public CCalcBlock  
{
public:
	CLinkFromStatBatBlock();
	virtual ~CLinkFromStatBatBlock();

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
	
	CCalcPort* getRefFromWSByID(CString csID);
	
public:
	CStringArray strOutputArr;//�������
	CStringArray strTagDespArr;//��������
private:
	CString m_str;

};

#endif // !defined(AFX_LINKFROMSTATBATBLOCK_H__7152DBAF_B75F_4678_AF1B_ED484EA72E1E__INCLUDED_)
