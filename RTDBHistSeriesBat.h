// RTDBHistSeriesBat.h: interface for the CRTDBHistSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_)
#define AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRTDBHistSeriesBat : public CCalcBlock  
{
public:
	CRTDBHistSeriesBat();
	virtual ~CRTDBHistSeriesBat();
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
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	double dLimitValue;//=10000000000000000000;//1E19;
private:
	CStringArray strVarArr;//���� strTableArr
	CStringArray strPortArr;//������strTagArr
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ

	int iRawOrSnapshot;//0:ԭʼ��1:����
	int iSnapshotSpan;//���ռ��

private:
	void readDataFromRTDB(CString strName,long lSTime,long lETime,int iReqType,int isnapshotT,std::vector<TagData> &tagDataArr);
};

#endif // !defined(AFX_RTDBHISTSERIESBAT_H__BD149C16_CF13_475B_97C2_D69C4DE2F11F__INCLUDED_)
