// H7CalcBlock.cpp: implementation of the CH7CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "H7CalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CH7CalcBlock::CH7CalcBlock()
{
	SetBlockType(_T("H7Calc"));
	ConstructBlockInputOutput();
}

CH7CalcBlock::~CH7CalcBlock()
{

}


//�����ʼ�������������
void CH7CalcBlock::ConstructBlockInputOutput()
{
	//����1�����
	AddOutputPort("PerX_h7thExtr",VALUE_DOUBLE,"");
	//����3������ P,T 
	AddInputPort(_T("PerX_h6thExtr"), VALUE_DOUBLE, _T("���γ�����ֵ"), _T(""));	//���γ�����ֵ
	AddInputPort(_T("PerX_hi7thExtr"), VALUE_DOUBLE, _T("�߶γ���������"), _T(""));	//�߶γ���������
	AddInputPort(_T("PudX_eff67Extr"), VALUE_DOUBLE, _T("���߶μ���Ч�����ֵ"), _T(""));	//���߶μ���Ч�����ֵ
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CH7CalcBlock::CreateFromString( const CString& strType )
{
	CH7CalcBlock* obj = new CH7CalcBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CH7CalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CH7CalcBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
	/////////////////////////////////////
	//�������

	//��7����ֵ	h7 = h6-(h6-(hi7)*Eta67_0)

	CCalcPort *pPortObj = GetOutputPortObj("PerX_h7thExtr"); 
	if(pPortObj)
	{
		double result=0;
		CUniValue a;
		CUniValue b;
		CUniValue c;

		BOOL ret1 = GetInputValue("PerX_h6thExtr",&a);	//���γ�����ֵ
		BOOL ret2 = GetInputValue("PerX_hi7thExtr",&b);	//�߶γ���������
		BOOL ret3 = GetInputValue("PudX_eff67Extr",&c);	//���߶μ���Ч�����ֵ
		if(ret1 && ret2 && ret3)
		{
			double h6 =	a.GetValueDouble();	//���γ�����ֵ
			double hi7 = b.GetValueDouble() ;	//�߶γ���������
			double Eta67_0 = c.GetValueDouble() ;//���߶μ���Ч�����ֵ

			double h7 = h6-(h6-(hi7)*Eta67_0);
			CUniValue UniVar;
			UniVar.AddData(h7);
			pPortObj->SetPortUniValue(UniVar);
		}
	}
	///////////////////////////////////
	//�������
	OutputResultToGlobalWS();
}