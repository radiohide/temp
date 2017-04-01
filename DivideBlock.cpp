// DivideBlock.cpp: implementation of the CDivideBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DivideBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDivideBlock::CDivideBlock()
{
	SetBlockType(_T( "Divide" ) );
	ConstructBlockInputOutput();
}

CDivideBlock::~CDivideBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CDivideBlock::CreateFromString( const CString& strType )
{
	CDivideBlock* obj = new CDivideBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CDivideBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("D1"), VALUE_DOUBLE);
	AddInputPort(_T("D2"), VALUE_DOUBLE);
	//��ģ�������룬��������Ϻ����Ϊһ��ʱ�����е����ݣ�
	//�����ɶ�����������ı������ơ�
	AddOutputPort("Out",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CDivideBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CDivideBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue1,inValue2;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
	
	CUniValue UniVar;
	if(0!=inValue2)
	{
		UniVar.AddData(inValue1/(inValue2));
	}
	else
		UniVar.AddData(0);
	pOutPortObj->SetPortUniValue(UniVar);
	//����������ռ���
	OutputResultToGlobalWS();
}
