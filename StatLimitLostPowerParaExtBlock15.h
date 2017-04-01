// StatLimitLostPowerParaExtBlock15.h: interface for the CStatLimitLostPowerParaExtBlock15 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATLIMITLOSTPOWERPARAEXTBLOCK15_H__30A6D48E_80E9_4B34_B3DC_125641103F9F__INCLUDED_)
#define AFX_STATLIMITLOSTPOWERPARAEXTBLOCK15_H__30A6D48E_80E9_4B34_B3DC_125641103F9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatLimitLostPowerParaExtBlock15 : public CCalcBlock  
{
public:
	CStatLimitLostPowerParaExtBlock15();
	virtual ~CStatLimitLostPowerParaExtBlock15();

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
	char* GetParaExtName();
	void GetStateLostPara(const char* FileName);

};

#endif // !defined(AFX_STATLIMITLOSTPOWERPARAEXTBLOCK15_H__30A6D48E_80E9_4B34_B3DC_125641103F9F__INCLUDED_)
