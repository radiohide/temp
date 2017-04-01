// ClockBlock.h: interface for the CClockBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_)
#define AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CClockBlock : public CCalcBlock  
{
public:
	CClockBlock();
	virtual ~CClockBlock();

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
	int m_iBeHaveTime;//0:�������1�����
	int m_iBeHaveBeginEndTime;//0:�������1�����
};

#endif // !defined(AFX_CLOCKBLOCK_H__B18AAD38_F025_4356_911B_407050D59355__INCLUDED_)
