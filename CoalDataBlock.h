// CoalDataBlock.h: interface for the CCoalDataBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COALDATABLOCK_H__AD019950_2F14_4974_B270_13D106AB3BA8__INCLUDED_)
#define AFX_COALDATABLOCK_H__AD019950_2F14_4974_B270_13D106AB3BA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCoalDataBlock : public CCalcBlock  
{
public:
	CCoalDataBlock();
	virtual ~CCoalDataBlock();
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
	void getAllNearCoalData(_ConnectionPtr &m_pConRDB,//�õ�Ԥ��ʱ����ڵ�����ú�����ݡ�
		CString strDBType,
		long timeflag,
		CStringArray &strTagNameArr,
		CArray<long,long> &lTimeArr,
		CArray<double,double> &dCoalKArr,
		CArray<double,double> &dCoalAllValue);
	void calcMatchCoalData(double dCoalK,CStringArray &strTagNameArr,//�õ�������ֵ��Χ��ú������
		CArray<long,long> &lTimeArr,
		CArray<double,double> &dCoalKArr);
private:
	HINSTANCE  hDLL;
	bool m_bConDB;
public:
	CString m_strTableName;
	CString m_strDBName;

	long	m_lOffset;
	int		m_TagTimeOffsetOrInput;
	
	int m_iDays;
	double m_dThreshold;
	int m_iNearTimeOrValue;//0:ʱ�����ȣ�1ֵ��ӽ�����

	CStringArray m_strCoalTagName;
	CStringArray m_strCoalTagDesp;
	CStringArray m_strCoalTagUnit;
};

#endif // !defined(AFX_COALDATABLOCK_H__AD019950_2F14_4974_B270_13D106AB3BA8__INCLUDED_)
