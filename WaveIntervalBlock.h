// WaveIntervalBlock.h: interface for the CWaveIntervalBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_)
#define AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWaveIntervalBlock : public CCalcBlock  
{
public:
	CWaveIntervalBlock();
	virtual ~CWaveIntervalBlock();
public:
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
	BOOL getSmoothData(
		const CArray<double,double> &dOrigValueArr,
		const CArray<long,long> &dOrigTimeArr,
		const CArray<short,short> &dOrigStatusArr,
		int iSmoothWidth,
		int iSmoothStep,
		CArray<double,double> &dSmoothValueArr,
		CArray<long,long> &dSmoothTimeArr,
		CArray<short,short> &dSmoothStatusArr
		);
private:
	bool m_bBeFirstCalc;//�Ƿ��һ����
	double m_dDisValue;//��¼���һ�����õ�����ɢֵ��
	
	//***********����*************
	int m_iSmoothWidth;//�������(��)
	int m_iSmoothStep;//��������(��)
	double m_dSpanIntPercent;//�����ٽ����䳬���߽�İٷֱ�
	int m_iSpanKeepTime;//�����趨���䲢������ʱ�䳤��
	CArray<double,double> m_dArrDisValue;//��ɢֵ
	CArray<double,double> m_dArrIntStart;//���俪ʼֵ
	CArray<double,double> m_dArrIntEnd;//�������ֵ
	//***********����*************
private:
	CArray<double,double> m_dSmoothValueArr;
	CArray<long,long> m_dSmoothTimeArr;
	CArray<short,short> m_dSmoothStatusArr;
private:
	double getFirstCalcDisValue();
	int getDisValue(double &disValue);
	int getDisValueByValueArr(
		const CArray<double,double> &tempValueArr,
		const CArray<double,double> &dArrDisValue,
		const CArray<double,double> &dArrIntStart,
		const CArray<double,double> &dArrIntEnd,
		double &disValue);
	void getDisValueByValue(
		double dAvg,
		const CArray<double,double> &dArrDisValue,
		const CArray<double,double> &dArrIntStart,
		const CArray<double,double> &dArrIntEnd,
		double &disValue);
	void getNewDisDataArr(
		CArray<double,double> &dArrNewDisValue,
		CArray<double,double> &dArrNewIntStart,
		CArray<double,double> &dArrNewIntEnd);
};

#endif // !defined(AFX_WAVEINTERVALBLOCK_H__E5CF6CFA_FF87_4D21_B66A_7E2DB559203E__INCLUDED_)
