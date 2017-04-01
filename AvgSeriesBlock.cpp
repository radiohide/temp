// AvgSeriesBlock.cpp: implementation of the CAvgSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "AvgSeriesBlock.h"
#include "PropertyDlgAvgSeries.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAvgSeriesBlock::CAvgSeriesBlock()
{
	SetBlockType(_T( "AvgSeries" ) );
	ConstructBlockInputOutput();
}

CAvgSeriesBlock::~CAvgSeriesBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CAvgSeriesBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Value"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//���������½�����ĺ���
CCalcBlock* CAvgSeriesBlock::CreateFromString( const CString& strType )
{
	CAvgSeriesBlock* obj = new CAvgSeriesBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CAvgSeriesBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CAvgSeriesBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������

	CCalcPort *pPortObjIn3 = GetInputPortObj(0);	
	CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
	double dOutput=0;//���
	if(CUV.GetDataSize()>0)//�����ݸ������������ʱ��ʼ���������������Ϊ0
	{
		//�õ���ֵ
		double sum=0;
		double avg=0;
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
			sum+=value;
		}
		avg=sum/CUV.GetDataSize();
		dOutput=avg;
	}
	CUniValue UniVar1;
	CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
	UniVar1.AddData(dOutput);
	pPortObjOut1->SetPortUniValue(UniVar1);
	//�������
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CAvgSeriesBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}
//��������ʾ��ͬ�����Զ���Ի���
void CAvgSeriesBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgAvgSeries dlg;
	dlg.DoModal();
}
//�����Խ���ģ��д�뵽���ݿ���
void CAvgSeriesBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}
//����ͼ��ʱ����Ҫ�������ԡ�
void CAvgSeriesBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//�����Է��ô���������
void CAvgSeriesBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}