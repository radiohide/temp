// TimeOffsetBlock.h: interface for the CTimeOffsetBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_)
#define AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CTimeOffsetBlock : public CCalcBlock  
{
public:
	CTimeOffsetBlock();
	virtual ~CTimeOffsetBlock();
//��ӱ�Ҫ���麯����
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
	int		m_iOffsetType;//0:��;1Сʱ;2:��;3:��;4:��;5:��;
	int		m_iSecond;//��

	int		m_iHour;//Сʱ
	CTime	m_HTime;//Сʱ-ʱ��

	int		m_iDay;//��
	CTime	m_DTime;//��-ʱ��

	int		m_iWeek;//��
	int		m_iWeekDay;//��-��
	CTime	m_WTime;//��-ʱ��

	
	int		m_iMonth;//��
	int		m_iMonthDay;//��-��
	CTime	m_MTime;//��-ʱ��
	
	int		m_iYear;//��
	int		m_iYearDay;//��-��
	int		m_iYearMonth;//��-��
	CTime	m_YTime;//��-ʱ��
private:
	int getDayOfTheMonth(int iYear,int iMonth,int iDayIn);
};

#endif // !defined(AFX_TIMEOFFSETBLOCK_H__EB151CCE_FC47_45DC_B4A6_39DC456865E5__INCLUDED_)
