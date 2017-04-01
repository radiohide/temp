// SelectorBlock.h: interface for the CSelectorBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTORBLOCK_H__F9F5BC02_50E4_476B_B4B9_6090A2A759E7__INCLUDED_)
#define AFX_SELECTORBLOCK_H__F9F5BC02_50E4_476B_B4B9_6090A2A759E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CSelectorBlock : public CCalcBlock  
{
public:
	CSelectorBlock();
	virtual ~CSelectorBlock();
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
	CStringArray m_strArrFunName;//���з�������
	CStringArray m_strArrFunDisp;//���з�������	

	CString m_strFuncName;//��ǰ�ķ�����
	CString m_strBlockDisp;//���������
	CString m_strPropertityTableName;//���Դ�������
	UINT m_iInputCount;

};

#endif // !defined(AFX_SELECTORBLOCK_H__F9F5BC02_50E4_476B_B4B9_6090A2A759E7__INCLUDED_)
