// StatEnumBlock.h: interface for the CStatEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_)
#define AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>

class CStatEnumBlock : public CCalcBlock  
{
public:
	CStatEnumBlock();
	virtual ~CStatEnumBlock();

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
	CString m_strDBName;//���ݿ�����

	CString m_strVarName;//��������
	CString m_strVarUnit;//������λ
	CString m_strVarDesp;//��������
	CString m_strOutName;//�������
	int m_iReqType;//��ѯ��ʽ��0���գ�1ԭʼ
	long m_lSnapshotT;//��ѯ����
	int m_iStatType;//ͳ�Ʒ�ʽ��0���ִ�����1����ʱ��
	
	CArray<double,double> m_dSValueArr;//���俪ʼ
	CArray<double,double> m_dEValueArr;//�������
	CArray<int,int> m_iSETag;//����״̬
	CArray<int,int> m_iUpOrDowmLimit;//���޻�����ƣ�0���ޣ�1����
	int m_iTagTimeIntM;//ʱ������������m_iTimeLengthIntM��ȡ���ֱ�־��Ĭ��1ȡ����
private:
	double dLimitValue;
	int m_iTimeLengthIntM;//����������m_iTimeLengthIntM��ȡ���֣�Ĭ��Ϊ2��
private:
	void getInRangeValue(double &dValue);//����Ƿ��ڷ�Χ�ڣ�����ȡ��Χ�ڵ�ֵ
	void getTimeIntM(long &lSTime,long &lETime);//������ȡ��
void readDataFromRTDB(CString strName,long lSTime,long lETime,int iReqType,int isnapshotT,std::vector<TagData> &tagDataArr);
	bool checkTableExist(CString strTable);
	void createTable(CString strTable);
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		int  iStatType,
		double dTotalValue,
		long iCount);
	void statData(std::vector<TagData> &tagDataArr,
							 double &dSum,
							 double &dMin,
							 double &dMax);
};

#endif // !defined(AFX_STATENUMBLOCK_H__4FE824D5_CA17_4845_9BEB_44EF6A0AF049__INCLUDED_)
