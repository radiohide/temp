// PLossRateBlock.cpp: implementation of the CPLossRateBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PLossRateBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLossRateBlock::CPLossRateBlock()
{
	SetBlockType(_T( "PLossRate" ) );//��������
	ConstructBlockInputOutput ();
}

CPLossRateBlock::~CPLossRateBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CPLossRateBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("Extr"), VALUE_DOUBLE);
	AddInputPort(_T("InStm"), VALUE_DOUBLE);
	AddOutputPort("PLossR",VALUE_DOUBLE);
}
//���������½�����ĺ���
CCalcBlock* CPLossRateBlock::CreateFromString( const CString& strType )
{
	CPLossRateBlock* obj = new CPLossRateBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CPLossRateBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CPLossRateBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double extValue,inValue;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&extValue);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue);
	
	CUniValue UniVar;
	UniVar.AddData((extValue-inValue)/extValue);
	pOutPortObj->SetPortUniValue(UniVar);
	//����������ռ���
	OutputResultToGlobalWS();
}
