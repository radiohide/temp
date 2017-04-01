// RTDBHistSeriesBatWP.h: interface for the CRTDBHistSeriesBatWP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTDBHISTSERIESBATWP_H__FF7FC685_F3DA_4D33_9D2C_B08B7C05068E__INCLUDED_)
#define AFX_RTDBHISTSERIESBATWP_H__FF7FC685_F3DA_4D33_9D2C_B08B7C05068E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
//#include "g_define.h"

class CRTDBHistSeriesBatWP : public CCalcBlock  
{
public:
	CRTDBHistSeriesBatWP();
	virtual ~CRTDBHistSeriesBatWP();
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
	int findTagIndex(long lcurTime,TagData *tagDatas,int& ncurIndex,long nCount);
};

#endif // !defined(AFX_RTDBHISTSERIESBATWP_H__FF7FC685_F3DA_4D33_9D2C_B08B7C05068E__INCLUDED_)
