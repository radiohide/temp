// DBPointBlock.h: interface for the CDBPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_)
#define AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CDBPointBlock : public CCalcBlock  
{
public:
	CDBPointBlock();
	virtual ~CDBPointBlock();
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
public:
	int m_TagTimeOffsetOrInput;//1:����ϵͳʱ��+ƫ�ơ�0:�����ⲿƫ�ơ�
	long m_lOffset;//ʱ��ƫ����ֵ
	CString m_strTagName;
	CString m_strTagDesp;
	int		m_iForwardOrBack;
	CString	m_strTableName;
	CString m_strDBDesp;

	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	HINSTANCE  hDLL;
	bool m_bConDB;
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

#endif // !defined(AFX_DBPOINTBLOCK_H__2613E5C5_F591_4941_96D1_EB5F0A855887__INCLUDED_)
