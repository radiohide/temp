// TimeBlock.h: interface for the CTimeBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEBLOCK_H__8BF19283_B987_4B38_9ED4_1C2C497A9371__INCLUDED_)
#define AFX_TIMEBLOCK_H__8BF19283_B987_4B38_9ED4_1C2C497A9371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CTimeBlock : public CCalcBlock  
{
public:
	CTimeBlock();
	virtual ~CTimeBlock();
public:
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
	int m_iTheTimeOrCurrent;//0:��ǰʱ�䣻1:ָ��ʱ��
	CString m_strDateTime;//��ǰʱ��
};

#endif // !defined(AFX_TIMEBLOCK_H__8BF19283_B987_4B38_9ED4_1C2C497A9371__INCLUDED_)
