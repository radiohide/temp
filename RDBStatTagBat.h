// RDBStatTagBat.h: interface for the CRDBStatTagBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDBSTATTAGBAT_H__0CFA74AF_B1F9_4A25_9A05_26F60B6CCBF2__INCLUDED_)
#define AFX_RDBSTATTAGBAT_H__0CFA74AF_B1F9_4A25_9A05_26F60B6CCBF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRDBStatTagBat : public CCalcBlock  
{
public:
	CRDBStatTagBat();
	virtual ~CRDBStatTagBat();
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

#endif // !defined(AFX_RDBSTATTAGBAT_H__0CFA74AF_B1F9_4A25_9A05_26F60B6CCBF2__INCLUDED_)
