// AvgSeriesBlock.h: interface for the CAvgSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVGSERIESBLOCK_H__9E8E389E_536D_41D1_BE1D_42BF39D2BF65__INCLUDED_)
#define AFX_AVGSERIESBLOCK_H__9E8E389E_536D_41D1_BE1D_42BF39D2BF65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CAvgSeriesBlock : public CCalcBlock  
{
public:
	CAvgSeriesBlock();
	virtual ~CAvgSeriesBlock();
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

#endif // !defined(AFX_AVGSERIESBLOCK_H__9E8E389E_536D_41D1_BE1D_42BF39D2BF65__INCLUDED_)
