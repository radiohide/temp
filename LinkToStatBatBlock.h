// LinkToStatBatBlock.h: interface for the CLinkToStatBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKTOSTATBATBLOCK_H__85426ACA_3657_4AF1_8266_1822300004FE__INCLUDED_)
#define AFX_LINKTOSTATBATBLOCK_H__85426ACA_3657_4AF1_8266_1822300004FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CLinkToStatBatBlock : public CCalcBlock  
{
public:
	CLinkToStatBatBlock();
	virtual ~CLinkToStatBatBlock();

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
	
	CString getRefToWSTagName(int index);
	
public:
	CStringArray strInputArr;//�������
	CStringArray strTagDespArr;//��������
	
	
private:
	CString m_str;

};

#endif // !defined(AFX_LINKTOSTATBATBLOCK_H__85426ACA_3657_4AF1_8266_1822300004FE__INCLUDED_)