// RDBHistSeriesBat.h: interface for the CRDBHistSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDBHISTSERIESBAT_H__16EABCE8_8E3A_45C5_8CB3_99064BB09CDB__INCLUDED_)
#define AFX_RDBHISTSERIESBAT_H__16EABCE8_8E3A_45C5_8CB3_99064BB09CDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRDBHistSeriesBat : public CCalcBlock  
{
public:
	CRDBHistSeriesBat();
	virtual ~CRDBHistSeriesBat();
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
	CString m_strFieldRealDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;

	CString m_strDBName;//���ݿ�����
private:
	CStringArray strTableArr;//����
	CStringArray strTagArr;//������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ

};

#endif // !defined(AFX_RDBHISTSERIESBAT_H__16EABCE8_8E3A_45C5_8CB3_99064BB09CDB__INCLUDED_)
