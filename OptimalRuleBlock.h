// OptimalRuleBlock.h: interface for the COptimalRuleBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_)
#define AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
class RuleRecommendListArr;
class COptimalRuleBlock : public CCalcBlock  
{
public:
	COptimalRuleBlock();
	virtual ~COptimalRuleBlock();
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
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	BOOL getEValueBySGIDMarkVar(int iSubGID,CString SubGMark,CString varName,double &EValue);
public:
	CString	m_strDBName;
	int m_iUnit;
	int m_iSet;
	int m_iRuleID;	

	CArray<int,int> m_iCGArr;//���ʽ�и�����ID
	CStringArray m_strFunExpArr;//���ʽ
	CStringArray m_strFunDepArr;//���ʽ����

	CArray<int,int> m_iVarCGArr;//�����и�����ID
	CStringArray m_strVarNameArr;//��������
	CStringArray m_strVarDespArr;//��������
	CStringArray m_StrVarUnit;//������λ
private:
	RuleRecommendListArr *m_RuleRecommendListArr;//��������Ѱ�����Ź���
};

#endif // !defined(AFX_OPTIMALRULEBLOCK_H__7127CF62_29A6_45AC_A2EF_84E7359A1F52__INCLUDED_)
