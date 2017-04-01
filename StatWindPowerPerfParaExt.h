// StatWindPowerPerfParaExt.h: interface for the CStatWindPowerPerfParaExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATWINDPOWERPERFPARAEXT_H__412D3D05_D4AF_4EE2_A414_8D13D177C56C__INCLUDED_)
#define AFX_STATWINDPOWERPERFPARAEXT_H__412D3D05_D4AF_4EE2_A414_8D13D177C56C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "WindPowerTag.h"

class CStatWindPowerPerfParaExt : public CCalcBlock  
{
public:
	CStatWindPowerPerfParaExt();
	virtual ~CStatWindPowerPerfParaExt();
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
private:
	CString m_strDBName;
	CString m_strTableName;
	CString m_strTurbine;
	int m_iPowerPorTagMin;//������ʶ��Сֵ
	int m_iPowerPorTagMax;//������ʶ���ֵ	
	double m_dPowerLimit;//�޵��ʶ
	double m_dMinParWindSpeed;//��С�������
	double m_dMaxParWindSpeed;//������85%��������ʱ����
	double m_dGbOilTempLimitPower;//���ز���޹���
	long lTimeOffset;
	
private:
	void delTagWindPowerMinMaxArr(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CUniValue  &CUV,
		int iMin,
		int iMax											
		);
	void delTagWindPowerArr(//ɾ���޵硢�������Է�Ƕ�
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CUniValue  &CUV,
		double iDelTag,//ɾ��ֵ�ı�ʶ
		int iMaxMinTag//0:ɾ�����ڣ�1ɾ��С�ڣ�2ɾ������
		);
	void delWindPowerArr(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//ɾ����Ҫɾ��������

	void delTimeOffsetOutWindPowerArr(CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);
	void delGbOilTempLimitWindPowerArr(CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);
	
	void getAvgAndVar(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		CArray<int,int> &iDiffGroupNums);//�õ�bin�����ڵľ�ֵ�ͷ���
	
	void getAvgAndVar(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//�õ���ֵ�ͷ���
	
	void delWindPowerArrByAvgVar(
								CArray<WindPowerTag,WindPowerTag&> &WindPowerArr);//���ݾ�ֵ�ͷ�����й���
	void getBinGroup(
		CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
		double dMinBinWSpeed,
		double dMaxBinWSpeed,
		CArray<int,int> &iDiffGroupNums);//bin����
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	void writeDataToTable(CString strTurbine,CString strTableName,WindPowerTag &oneTag);
};

#endif // !defined(AFX_STATWINDPOWERPERFPARAEXT_H__412D3D05_D4AF_4EE2_A414_8D13D177C56C__INCLUDED_)