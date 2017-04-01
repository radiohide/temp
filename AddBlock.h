// AddBlock.h: interface for the CAddBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDBLOCK_H__0976B8DE_DBAA_4902_8061_0C3E7D69E329__INCLUDED_)
#define AFX_ADDBLOCK_H__0976B8DE_DBAA_4902_8061_0C3E7D69E329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CAddBlock : public CCalcBlock  
{
public:
	CAddBlock();
	virtual ~CAddBlock();
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

public:
	CString m_strInputEdit;

private:
	void buildInputPort(CStringArray &IDArrLeftOut);
};

#endif // !defined(AFX_ADDBLOCK_H__0976B8DE_DBAA_4902_8061_0C3E7D69E329__INCLUDED_)
