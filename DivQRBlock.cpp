// DivQRBlock.cpp: implementation of the CDivQRBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DivQRBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDivQRBlock::CDivQRBlock()
{
	SetBlockType(_T( "DivQR" ) );
	ConstructBlockInputOutput();
}

CDivQRBlock::~CDivQRBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CDivQRBlock::CreateFromString( const CString& strType )
{
	CDivQRBlock* obj = new CDivQRBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CDivQRBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("D1"), VALUE_DOUBLE);
	AddInputPort(_T("D2"), VALUE_DOUBLE);
	//��ģ�������룬��������Ϻ����Ϊһ��ʱ�����е����ݣ�
	//�����ɶ�����������ı������ơ�
	AddOutputPort("Q",VALUE_DOUBLE);
	AddOutputPort("R",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CDivQRBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CDivQRBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutQPortObj = GetOutputPortObj(0);
	CCalcPort *pOutRPortObj = GetOutputPortObj(1);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue1,inValue2;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
	
	CUniValue UniVarQ,UniVarR;
	if(0!=inValue2)
	{
		UniVarQ.AddData((int)inValue1/(int)inValue2);
		UniVarR.AddData(fmod(inValue1,inValue2));
	}
	else
	{
		UniVarQ.AddData(0);
		UniVarR.AddData(0);
	}

	pOutQPortObj->SetPortUniValue(UniVarQ);
	pOutRPortObj->SetPortUniValue(UniVarR);
	//����������ռ���
	OutputResultToGlobalWS();
}
