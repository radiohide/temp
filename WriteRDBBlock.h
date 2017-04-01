// WriteRDBBlock.h: interface for the CWriteRDBBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_)
#define AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBBlock : public CCalcBlock  
{
public:
	CWriteRDBBlock();
	virtual ~CWriteRDBBlock();
public:
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
public:
	int m_TagTimeOffsetOrInput;//1:����ϵͳʱ��+ƫ�ơ�0:�����ⲿƫ�ơ�
	long m_lOffset;//ʱ��ƫ����ֵ
	CString m_strTagName;
	CString m_strTagDesp;
	CString	m_strTableName;
	CString m_strDBDesp;
	int m_iPointType;
	int m_iPointStatus;
private:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	void creatPointDataTableInMDB(CString tableName);
	BOOL gIsTableExist(_ConnectionPtr& pConnection, CString strTableName);
};

#endif // !defined(AFX_WRITERDBBLOCK_H__045960E6_9BC5_4748_9959_655B4A9A5EA4__INCLUDED_)
