// WriteRDBStatBat.h: interface for the CWriteRDBStatBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBSTATBAT_H__C70D1246_7DE2_4557_AB31_F4064A6A5CE6__INCLUDED_)
#define AFX_WRITERDBSTATBAT_H__C70D1246_7DE2_4557_AB31_F4064A6A5CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBStatBat : public CCalcBlock  
{
public:
	CWriteRDBStatBat();
	virtual ~CWriteRDBStatBat();
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
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		double dMaxValue,
		double dMinValue,
		short iType,
		double dSum,
		long iCount);
	void getInRangeValue(double &dValue);
private:
	CString m_strDBName;//���ݿ�����

	CStringArray tagNameArr;//����
	CStringArray unitArr;//��λ
	CStringArray dispArr;//����
	CStringArray strOutPutNameArr;//д���ϵ��ı����ƣ����������׺��

};

#endif // !defined(AFX_WRITERDBSTATBAT_H__C70D1246_7DE2_4557_AB31_F4064A6A5CE6__INCLUDED_)
