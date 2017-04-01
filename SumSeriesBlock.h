// SumSeriesBlock.h: interface for the CSumSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUMSERIESBLOCK_H__A209D603_D685_4D57_ACB0_2547DDD6501C__INCLUDED_)
#define AFX_SUMSERIESBLOCK_H__A209D603_D685_4D57_ACB0_2547DDD6501C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CSumSeriesBlock : public CCalcBlock  
{
public:
	CSumSeriesBlock();
	virtual ~CSumSeriesBlock();
public:
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
};

#endif // !defined(AFX_SUMSERIESBLOCK_H__A209D603_D685_4D57_ACB0_2547DDD6501C__INCLUDED_)
