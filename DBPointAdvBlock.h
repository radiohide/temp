// DBPointAdvBlock.h: interface for the CDBPointAdvBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPOINTADVBLOCK_H__36BEC20F_8F22_48A9_BDCB_4E343E17AA7C__INCLUDED_)
#define AFX_DBPOINTADVBLOCK_H__36BEC20F_8F22_48A9_BDCB_4E343E17AA7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>
class CDBPointAdvBlock : public CCalcBlock  
{
public:
	CDBPointAdvBlock();
	virtual ~CDBPointAdvBlock();
public:
	//��ӱ�Ҫ���麯����
//-----------------------------------------------------------------------------------------------------------
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //��������״������Add at 2011-10-30
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
	//����Ԥ����Ĳ�������
	CString	n_strTagDesp;
	CString n_strTagName1;
	CString n_strTagName2;
	CString n_strTagName3;
	CString n_strTagName4;
	CString n_strTagName5;
	
	int		n_iOutMethod;	
	BOOL	n_bChkLimit;
	double	n_dHighLimit;
	double	n_dLowLimit;
	double	n_dConstWhenBad;
	BOOL	n_bForceEnable;	
	double	n_dForceValue;

	int  m_iInOrOut;//1,�ڲ���0���ⲿ
	long m_lOffsetTime;
	int	 m_iForwardOrBack;

	CString	m_strTableName;
	CString m_strDBDesp;
private:
	BOOL getValueFromDB(_ConnectionPtr &m_pConRDB,
		CString strDBType,
		CString strTableName,
		CString strTagName,
		long timeflag,
		int iForwardOrBack,
		double &dRetValue,
		long &lRetTime,
		int &iRetState);
};

#endif // !defined(AFX_DBPOINTADVBLOCK_H__36BEC20F_8F22_48A9_BDCB_4E343E17AA7C__INCLUDED_)
