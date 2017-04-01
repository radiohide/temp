// StatLimitLostPowerBlock.h: interface for the CStatLimitLostPowerBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATLIMITLOSTPOWERBLOCK_H__25A4F889_2EE4_411D_BFD6_6438CF0375CB__INCLUDED_)
#define AFX_STATLIMITLOSTPOWERBLOCK_H__25A4F889_2EE4_411D_BFD6_6438CF0375CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatLimitLostPowerBlock : public CCalcBlock  
{
public:
	CStatLimitLostPowerBlock();
	virtual ~CStatLimitLostPowerBlock();
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
	
	CArray<double,double> dWindSpeedArr;//��������--����
	CArray<double,double> dPowerArr;//��������--����

	int iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����
	double dMaxPower;//����й����ʸ���ֵ
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	double getPowerFromWindSpeed(double dWindSpeed);
	double getLimitPowerByTime(CUniValue &CUV,long lTime);
	//bool isValidLostByTime(CUniValue &CUV,long lTime);
};

#endif // !defined(AFX_STATLIMITLOSTPOWERBLOCK_H__25A4F889_2EE4_411D_BFD6_6438CF0375CB__INCLUDED_)
