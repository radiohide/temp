// SumSeriesBlock.cpp: implementation of the CSumSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SumSeriesBlock.h"
#include "PropertyDlgSumSeries.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSumSeriesBlock::CSumSeriesBlock()
{
	SetBlockType(_T( "SumSeries" ) );
	ConstructBlockInputOutput();
}

CSumSeriesBlock::~CSumSeriesBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CSumSeriesBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Value"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//���������½�����ĺ���
CCalcBlock* CSumSeriesBlock::CreateFromString( const CString& strType )
{
	CSumSeriesBlock* obj = new CSumSeriesBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CSumSeriesBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CSumSeriesBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������

	CCalcPort *pPortObjIn3 = GetInputPortObj(0);	
	CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
	double dOutput=0;//������
	if(CUV.GetDataSize()>0)//�����ݸ������������ʱ��ʼ���������������Ϊ0
	{
		//�õ���ֵ
		double sum=0;
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
			sum+=value;
		}
		dOutput=sum;
	}
	CUniValue UniVar1;
	CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
	UniVar1.AddData(dOutput);
	pPortObjOut1->SetPortUniValue(UniVar1);
	//�������
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CSumSeriesBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}
//��������ʾ��ͬ�����Զ���Ի���
void CSumSeriesBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgSumSeries dlg;
	dlg.DoModal();
}
//�����Խ���ģ��д�뵽���ݿ���
void CSumSeriesBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}
//����ͼ��ʱ����Ҫ�������ԡ�
void CSumSeriesBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//�����Է��ô���������
void CSumSeriesBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
