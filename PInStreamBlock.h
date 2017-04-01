// PInStreamBlock.h: interface for the CPInStreamBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINSTREAMBLOCK_H__0FA124F7_3A7E_4C25_A7D5_EDF614F6529C__INCLUDED_)
#define AFX_PINSTREAMBLOCK_H__0FA124F7_3A7E_4C25_A7D5_EDF614F6529C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CPInStreamBlock : public CCalcBlock  
{
public:
	int m_bBeIgnore;//1:���ԣ�0��������
public:
	CPInStreamBlock();
	virtual ~CPInStreamBlock();
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
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//�����Է��ô���������
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
};

#endif // !defined(AFX_PINSTREAMBLOCK_H__0FA124F7_3A7E_4C25_A7D5_EDF614F6529C__INCLUDED_)
