// LinkFromProSerBatBlock.h: interface for the CLinkFromProSerBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKFROMPROSERBATBLOCK_H__F0FD03BF_4C66_468A_B2BF_740CDA0DD66E__INCLUDED_)
#define AFX_LINKFROMPROSERBATBLOCK_H__F0FD03BF_4C66_468A_B2BF_740CDA0DD66E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CLinkFromProSerBatBlock : public CCalcBlock  
{
public:
	CLinkFromProSerBatBlock();
	virtual ~CLinkFromProSerBatBlock();

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

#endif // !defined(AFX_LINKFROMPROSERBATBLOCK_H__F0FD03BF_4C66_468A_B2BF_740CDA0DD66E__INCLUDED_)
