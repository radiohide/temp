// IntegrateBlock.cpp: implementation of the CIntegrateBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "IntegrateBlock.h"
#include "PropertyDlgIntegrate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntegrateBlock::CIntegrateBlock()
{
	SetBlockType(_T( "Integrate" ) );
	ConstructBlockInputOutput();
}

CIntegrateBlock::~CIntegrateBlock()
{

}

//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CIntegrateBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("STime"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("ETime"), VALUE_DOUBLE, _T("in1"), _T("in1"));	
	AddInputPort(_T("Value"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//���������½�����ĺ���
CCalcBlock* CIntegrateBlock::CreateFromString( const CString& strType )
{
	CIntegrateBlock* obj = new CIntegrateBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CIntegrateBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CIntegrateBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	double inValue1,inValue2;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
	long lSTime=(long)inValue1;
	long lETime=(long)inValue2;

	CCalcPort *pPortObjIn3 = GetInputPortObj(2);	
	CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
	double dOutput=0;//���
	if(CUV.GetDataSize()>=3)//�����ݸ������ڵ���������ʱ��ʼ���������������Ϊ0
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
		dOutput=avg*(lETime-lSTime);
	}
	CUniValue UniVar1;
	CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
	UniVar1.AddData(dOutput);
	pPortObjOut1->SetPortUniValue(UniVar1);
	//�������
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CIntegrateBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}
//��������ʾ��ͬ�����Զ���Ի���
void CIntegrateBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgIntegrate dlg;
	dlg.DoModal();
}
//�����Խ���ģ��д�뵽���ݿ���
void CIntegrateBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}
//����ͼ��ʱ����Ҫ�������ԡ�
void CIntegrateBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//�����Է��ô���������
void CIntegrateBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
