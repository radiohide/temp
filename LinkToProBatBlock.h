// LinkToProBatBlock.h: interface for the CLinkToProBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKTOPROBATBLOCK_H__D59454AF_7B93_47A7_8BC1_AB39048AFFE4__INCLUDED_)
#define AFX_LINKTOPROBATBLOCK_H__D59454AF_7B93_47A7_8BC1_AB39048AFFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgProcToStatis.h"

class CLinkToProBatBlock : public CCalcBlock  
{
public:
	CLinkToProBatBlock();
	virtual ~CLinkToProBatBlock();

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

#endif // !defined(AFX_LINKTOPROBATBLOCK_H__D59454AF_7B93_47A7_8BC1_AB39048AFFE4__INCLUDED_)
