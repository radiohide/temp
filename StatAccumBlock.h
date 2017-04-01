// StatAccumBlock.h: interface for the CStatAccumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATACCUMBLOCK_H__44B1BF14_3132_453A_9936_A4E95408F64A__INCLUDED_)
#define AFX_STATACCUMBLOCK_H__44B1BF14_3132_453A_9936_A4E95408F64A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>

class CStatAccumBlock : public CCalcBlock  
{
public:
	CStatAccumBlock();
	virtual ~CStatAccumBlock();
//���ӱ�Ҫ���麯����
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
	int m_iStatType;//ͳ�Ʒ�ʽ��0,���֣�1��β��ֵ

	double m_dGain;//�Ŵ�����Ĭ��Ϊ1��

	CStringArray tagNameArr;//����
	CStringArray unitArr;//��λ
	CStringArray dispArr;//����
	CArray<int,int> reqTypeArr;//��ѯ��ʽ
	CArray<int,int> snapshotTArr;//��������

	CStringArray strOutPutNameArr;//д���ϵ��ı����ƣ����������׺��
	int m_iTagTimeIntM;//ʱ������������m_iTimeLengthIntM��ȡ���ֱ�־��Ĭ��1ȡ����

	//��������ʽ
	int m_iConOn;//��������
	CString m_strConVar;//��������
	CString m_strConUnit;//��λ
	CString m_strConDesp;//����
	int m_iConReqType;//��ѯ��ʽ
	int m_iConSnapShotT;//��������
	int m_iCon1Fun;//��������1
	int m_iCon2Fun;//��������2
	double m_dCon1Value;//����ֵ1
	double m_dCon2Value;//����ֵ2
	//��������ʽ

private:
	double dLimitValue;
	int m_iTimeLengthIntM;//����������m_iTimeLengthIntM��ȡ���֣�Ĭ��Ϊ2��
private:
	void getInRangeValue(double &dValue);//����Ƿ��ڷ�Χ�ڣ�����ȡ��Χ�ڵ�ֵ
	void getTimeIntM(long &lSTime,long &lETime);//������ȡ��
	bool checkTableExist(CString strTable);
	void createTable(CString strTable);
	void writeDataToTable(
		CString strTable,
		long lSTime,
		long lETime,
		double dMaxValue,
		double dMinValue,
		double dSum,
		long iCount);
	void readDataFromRTDB(
		CString strVarName,
		long lSTime,
		long lETime,
		TagData &valueData1,
		TagData &valueData2);
	void readDataFromRTDB(
		CString strName,
		long lSTime,
		long lETime,
		int iReqType,//0:����;1:ԭʼ
		int isnapshotT,
		std::vector<TagData> &tagDataArr);
	void statData(std::vector<TagData> &tagDataArr,
							 double &dAvg,
							 double &dMin,
							 double &dMax);
	void getStatConGBTimeArr(std::vector<TagData> &ConTagDataArr,
		long lSTime,
		long lETime,
		std::vector<long> &lGSTime,//��ʱ��ο�ʼʱ��
		std::vector<long> &lGETime,//��ʱ��ν���ʱ��
		std::vector<long> &lBSTime,//��ʱ��ο�ʼʱ��
		std::vector<long> &lBETime//��ʱ��ν���ʱ��
		);
	int getConValueRegionNum(double dValue,const double &dRangMin,const double &dRangMax);
};

#endif // !defined(AFX_STATACCUMBLOCK_H__44B1BF14_3132_453A_9936_A4E95408F64A__INCLUDED_)