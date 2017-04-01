// WriteRDBStatBlock.h: interface for the CWriteRDBStatBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_)
#define AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBStatBlock : public CCalcBlock  
{
public:
	CWriteRDBStatBlock();
	virtual ~CWriteRDBStatBlock();
public:
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
public:
	CString m_strDBName;//���ݿ�����
	
	BOOL m_BStat;
	BOOL m_BMaxValue;
	BOOL m_BMinValue;
	BOOL m_BCount;//�Ƿ�ѡ���������
	
	CString m_strVarDesp;
	CString m_strVarName;
private:
	int m_iTagTimeIntM;//Ĭ��ȡ����
	int m_iTimeLengthIntM;//Ĭ��������������ȡ����
private:
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		double dMax,
		double dMin,
		int  iStatType,
		double dTotalValue,
		long iCount);
	void getTimeIntM(long &lSTime,long &lETime);
	void createTable(CString strTable);
	bool checkTableExist(CString strTable);
	void getInRangeValue(double &dValue);
};

#endif // !defined(AFX_WRITERDBSTATBLOCK_H__AB842207_C173_4121_A396_1EB971FC4486__INCLUDED_)
