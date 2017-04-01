// DiscreteTableBlock.h: interface for the CDiscreteTableBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISCRETETABLEBLOCK_H__D4333FEB_D68C_4821_B146_45C74ED59009__INCLUDED_)
#define AFX_DISCRETETABLEBLOCK_H__D4333FEB_D68C_4821_B146_45C74ED59009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgDiscreteTable.h"

class DiscreteRecord
{
public:
	CString str_division_code;
	CString str_code_desp;
	CString str_low_value;
	CString str_high_value;
};
class CDiscreteTableBlock : public CCalcBlock  
{
public:
	CDiscreteTableBlock();
	virtual ~CDiscreteTableBlock();

	int iComIndex;

	CString m_strDBDesp;
	CString	m_strDBType;
	CString	m_strTableName;

	CArray<DiscreteRecord*,DiscreteRecord*&>  m_DiscreteContent ;
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

#endif // !defined(AFX_DISCRETETABLEBLOCK_H__D4333FEB_D68C_4821_B146_45C74ED59009__INCLUDED_)
