// RuleRunBlock.h: interface for the CRuleRunBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_)
#define AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "RuleData.h"

class CRuleRunBlock : public CCalcBlock  
{
public:
	CRuleRunBlock();
	virtual ~CRuleRunBlock();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��ӱ�Ҫ���麯����
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //��������״������Add at 2011-10-30
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
//����������ɫ�ĺ���===============================================
	void SetDBConnectString(CString str);
	CString GetDBConnectString();
	void SetRuleDBName(CString str);
	CString GetRuleDBName();
	CString getABSDirRuleDB();
	BOOL EstablishMDBConnectPtr(CString strFileName);
	int SetRuleBlockInputOutputByDB(CString strDBConnect,CString strRuleDBName);
	int GetDisInterval(	CString strComFun, 
		double dPointValue,
		double dStepValue,	
		double dSValue,//��ɢ����Χ�Ŀ�ʼ
		double dEValue,//��ɢ����Χ�Ľ���
		CArray<int,int> &disValueArr,
		CArray<double,double> &sValueArr,
		CArray<double,double> &eValueArr);
	int GetDisInterval(	CString strComFun, 
		double dPointValue,
		double dStepValue,	
		int iLeftNum,//����ɢ������
		int iRightNum,//����ɢ������
		CArray<int,int> &disValueArr,
		CArray<double,double> &sValueArr,
		CArray<double,double> &eValueArr);
	int getDisValueByName(CString strVarName,double dValue);
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void readRuleData();//��ȡ������Ϣ
	void readParData();//��ȡ������Ϣ
	void resetBeforeCalc();//�ڼ���ǰ���и�λ
	void setBDValueByName(CString strVarName,double dValue);
	void setFDValueByName(CString strVarName,double dValue);
	CRuleData* getMinDiffRuleData();
	void calcDiffValue();
	double getDValueByDis(CString strVarName,int disValue);
	void setConToRuleData(CString *pStrConCode);
private:
 //����������ɫ������
	//���ӵ��ַ��������ڹ����������ӣ���ȡ��Ӧ�Ĺ����ڴ���
	_ConnectionPtr m_pConRule; //���ӵ��������
	BOOL m_bCon;//�Ƿ����ӹ����
	CString m_strDBConnect;
	CString m_strRuleDBName;
	CString m_strRuleDBDesp;
	int  m_iTagToFOrB;//ǰ������ͷ��������ǣ�0:ǰ��(eg:��״̬������)(Ĭ��),1:����(eg:��״̬������)
	int  m_iTagWeightOrTime;//����Ȩ������ǰ���ʱ����������й���ƥ�䡣0:Ȩ�����1:ʱ�����

	CStringArray nameFArr;
	CStringArray despFArr;
	CArray<int,int> tagBeInFArr;//��������Ƶ���ǰ���������,1:����,0:������
	CArray<double,double> dValueFArr;//ǰ��Valueֵ
	CArray<int,int> iDisValueFArr;//ǰ������ɢֵ

	CStringArray nameBArr;
	CStringArray despBArr;
	CArray<int,int> tagBeInBArr;//��������Ƶ��ĺ���������,1:����,0:������
	CArray<double,double> dValueBArr;//���Valueֵ
	CArray<int,int> iDisValueBArr;//�������ɢֵ

	CStringArray nameArrDB;
	CStringArray typeFBArrDB;//ǰ���������־
	CStringArray disMethodArrDB;
	CArray<double,double> disRefValueArrDB;
	CArray<double,double> disStepArrDB;

	CArray<CRuleData*,CRuleData*&> ruleDataArr;
};

#endif // !defined(AFX_RULERUNBLOCK_H__9F86DF51_7F69_4A7B_84A6_2EF524078CE2__INCLUDED_)
