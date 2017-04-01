// SawtoothBlock.h: interface for the CSawtoothBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAWTOOTHBLOCK_H__5D1B8ED0_7557_4F27_A3C7_CA5B966EBFB4__INCLUDED_)
#define AFX_SAWTOOTHBLOCK_H__5D1B8ED0_7557_4F27_A3C7_CA5B966EBFB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CSawtoothBlock : public CCalcBlock  
{
private:
	int m_iMin;
	int m_iMax;
	int m_iResult;
	BOOL m_BFirstCalc;
public:
	CSawtoothBlock();
	virtual ~CSawtoothBlock();
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

};

#endif // !defined(AFX_SAWTOOTHBLOCK_H__5D1B8ED0_7557_4F27_A3C7_CA5B966EBFB4__INCLUDED_)