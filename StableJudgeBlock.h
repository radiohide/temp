// StableJudgeBlock.h: interface for the CStableJudgeBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_)
#define AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgStableJudge.h"

class CStableJudgeBlock : public CCalcBlock  
{
public: 

	CStableJudgeBlock();
	virtual ~CStableJudgeBlock();

	double	m_dDown2;
	double	m_dDown1;
	double	m_dFazhi1;
	double	m_dFazhi2;
	int		m_iRadio;
	int m_iCalcNoStable;
public:
	//�������صĺ���
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput(); 
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
private:
	BOOL getSmoothArr(const CArray<double,double> &dOrigValueArr,
		int iSmoothStep,
		int iSmoothMinSize,
		CArray<double,double> &dSmoothArr);
	void getDisValue(const CArray<double,double> &dSmoothArr,
		double dFBDataPart,
		double &dFDownPer,
		double &dFUpPer,
		double &dBDownPer,
		double &dBUpPer);
	int getUpOrDown(double dFBPartBeVauePer,
		double dFDownPer,
		double dFUpPer,
		double dBDownPer,
		double dBUpPer);
	double getVariance(const CArray<double,double> &dOrigValueArr);
	void getVariance(const CArray<double,double> &dOrigValueArr,
		int iPartNum,
		CArray<double,double> &dRealVarArr);
};

#endif // !defined(AFX_STABLEJUDGEBLOCK_H__7B40D6AD_508B_4159_8C7E_1276E3DF2137__INCLUDED_)
