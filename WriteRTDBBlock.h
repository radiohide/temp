// WriteRTDBBlock.h: interface for the CWriteRTDBBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERTDBBLOCK_H__557D72CA_FA6E_402A_9789_ADDBC9B6EF6C__INCLUDED_)
#define AFX_WRITERTDBBLOCK_H__557D72CA_FA6E_402A_9789_ADDBC9B6EF6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRTDBBlock : public CCalcBlock  
{
public:
	CWriteRTDBBlock();
	virtual ~CWriteRTDBBlock();
public:
	//��ӱ�Ҫ���麯����
//-----------------------------------------------------------------------------------------------------------
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
public:
	int m_TagTimeOffsetOrInput;//1:����ϵͳʱ��+ƫ�ơ�2:�����ⲿʱ�䡣
	long m_lOffset;//ʱ��ƫ����ֵ
	CString m_strTagName;
	CString m_strTagDesp;
	int m_iPointType;
	int m_iPointStatus;
	int m_iCheckPort;
};

#endif // !defined(AFX_WRITERTDBBLOCK_H__557D72CA_FA6E_402A_9789_ADDBC9B6EF6C__INCLUDED_)
