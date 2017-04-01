// RDBHistTagBat.h: interface for the CRDBHistTagBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDBHISTTAGBAT_H__15F8EE64_55D6_4CB6_B446_A025050647F3__INCLUDED_)
#define AFX_RDBHISTTAGBAT_H__15F8EE64_55D6_4CB6_B446_A025050647F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRDBHistTagBat : public CCalcBlock  
{
public:
	CRDBHistTagBat();
	virtual ~CRDBHistTagBat();
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;

	CString m_strDBName;//���ݿ�����
	int m_iNearSec;//ȡ�ڽ������������
private:
	CStringArray strTableArr;//����
	CStringArray strTagArr;//������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ
};

#endif // !defined(AFX_RDBHISTTAGBAT_H__15F8EE64_55D6_4CB6_B446_A025050647F3__INCLUDED_)
