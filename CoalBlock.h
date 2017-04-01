// CoalBlock.h: interface for the CCoalBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COALBLOCK_H__09941A51_BC49_4A4B_8DD1_3C0A358E6BF7__INCLUDED_)
#define AFX_COALBLOCK_H__09941A51_BC49_4A4B_8DD1_3C0A358E6BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCoalBlock : public CCalcBlock  
{
public:
	CCoalBlock();
	virtual ~CCoalBlock();
public:
	double m_dMf;
	int m_bFixMf;
	int m_iH;
	int m_iLwCoalAr;
	int m_iMs;
	int m_iS;
	int m_iAshAr;
public:
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
};

#endif // !defined(AFX_COALBLOCK_H__09941A51_BC49_4A4B_8DD1_3C0A358E6BF7__INCLUDED_)
