// StatProEnumParaExtBlock.h: interface for the CStatProEnumParaExtBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATPROENUMPARAEXTBLOCK_H__A7FA7EEA_88CE_4959_9446_BA3072B4C2FA__INCLUDED_)
#define AFX_STATPROENUMPARAEXTBLOCK_H__A7FA7EEA_88CE_4959_9446_BA3072B4C2FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatProEnumParaExtBlock : public CCalcBlock  
{
public:
	CStatProEnumParaExtBlock(int iTimeSelfOrInput);
	virtual ~CStatProEnumParaExtBlock();

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
	CStringArray strOutputArr;//�������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ
	int iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����
	
	double	m_dLeft;//�����󣨱����䣩
	double	m_dRight;//�����ң������䣩
	short	m_iStatType;//״ֵ̬
	int		m_iFunType;//ͳ�Ʒ���
	double m_dGain;//ͳ�ƽ����������
protected:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	void statDataCount(CUniValue  &CUV,//ͳ�ƴ���
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
	void statDataTimeLength(CUniValue  &CUV,//ͳ��ʱ��
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount);
	bool isInRange(const double &dValue,const double &dLeft,const double &dRight);

};

#endif // !defined(AFX_STATPROENUMPARAEXTBLOCK_H__A7FA7EEA_88CE_4959_9446_BA3072B4C2FA__INCLUDED_)
