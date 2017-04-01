// StatStateLostPowerBlock.h: interface for the CStatStateLostPowerBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATSTATELOSTPOWERBLOCK_H__59B4A740_42DA_4C2B_A56E_838FB8B63C84__INCLUDED_)
#define AFX_STATSTATELOSTPOWERBLOCK_H__59B4A740_42DA_4C2B_A56E_838FB8B63C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatStateLostPowerBlock : public CCalcBlock  
{
public:
	CStatStateLostPowerBlock();
	virtual ~CStatStateLostPowerBlock();
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
public:
	CStringArray strInputArr;//�������
	CArray<int,int> iValidMin;//����ʱ����ֵ��Сֵ
	CArray<int,int> iValidMax;//����ʱ����ֵ���ֵ
	CStringArray strTagDespArr;//��������
	
	CArray<double,double> dWindSpeedArr;//��������--����
	CArray<double,double> dPowerArr;//��������--����

	int iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����
private:
	int iInputDataStart;//�������ݵĿ�ʼ�˿ںţ���0��ʼ��
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	double getPowerFromWindSpeed(double dWindSpeed);
	bool isValidLostByTime(long lTime);
};

#endif // !defined(AFX_STATSTATELOSTPOWERBLOCK_H__59B4A740_42DA_4C2B_A56E_838FB8B63C84__INCLUDED_)
