// CoalKBlock.h: interface for the CCoalKBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_)
#define AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCoalKBlock : public CCalcBlock  
{
public:
	CCoalKBlock();
	virtual ~CCoalKBlock();
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
private:
	BOOL getSETimeNeedToCalc(_ConnectionPtr &m_pConRDB,
		CString strDBType,
		long time,
		long &lRealRecordTime,
		CArray<long,long> &sTimeArr,
		CArray<long,long> &eTimeArr);

	BOOL getCoalK(CStringArray &TagFlowArr,long sTime,long eTime,double &dCoalK);
	void writeCoalK(_ConnectionPtr &m_pConRDB,long time,double dCoalK);
private:
	int initDbConnect(HINSTANCE hDLL);
	BOOL loadDll();
public:
	int m_iFDay;
	int m_iSHour;
	int m_iSMinute;
	int m_iSSecond;

	int m_iBDay;
	int m_iEHour;
	int m_iEMinute;
	int m_iESecond;

	int m_iStepLength;
	int m_iStepUnit;//1:�룬2�֣�3Сʱ

	CString m_strPowerTagName;
	CString m_StrPowerUnit;

	CStringArray m_strFlowTagNameArr;
	CStringArray m_strFlowUnitArr;
	CStringArray m_strFlowDespArr;

	CString m_strTableName;
	CString m_strDBName;

	long	m_lOffset;
	int		m_TagTimeOffsetOrInput;
private:
	HINSTANCE  hDLL;
	bool m_bConDB;
};

#endif // !defined(AFX_COALKBLOCK_H__4AFE69F7_5A75_4FEB_A6FE_EFEDAD0155F2__INCLUDED_)
