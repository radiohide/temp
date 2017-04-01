// StatStateLostPowerParaExtBlock.h: interface for the CStatStateLostPowerParaExtBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATSTATELOSTPOWERPARAEXTBLOCK_H__0BA871E0_6EBA_4D71_B1A4_57CAC0586AB2__INCLUDED_)
#define AFX_STATSTATELOSTPOWERPARAEXTBLOCK_H__0BA871E0_6EBA_4D71_B1A4_57CAC0586AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatStateLostPowerParaExtBlock : public CCalcBlock  
{
public:
	CStatStateLostPowerParaExtBlock();
	virtual ~CStatStateLostPowerParaExtBlock();

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
	char* GetParaExtName();
	void GetStateLostPara(const char* FileName);
};

#endif // !defined(AFX_STATSTATELOSTPOWERPARAEXTBLOCK_H__0BA871E0_6EBA_4D71_B1A4_57CAC0586AB2__INCLUDED_)
