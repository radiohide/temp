// H8CalcBlock.cpp: implementation of the CH8CalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "H8CalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CH8CalcBlock::CH8CalcBlock()
{
	SetBlockType(_T("H8Calc"));
	ConstructBlockInputOutput();
}

CH8CalcBlock::~CH8CalcBlock()
{

}


//�����ʼ�������������
void CH8CalcBlock::ConstructBlockInputOutput()
{
	//����3�����
	AddOutputPort("PerX_h8thExtr",VALUE_DOUBLE,"");//8����ֵ
	AddOutputPort("PerX_x8thExtrStm",VALUE_DOUBLE,"");//�˶γ����ɶ�
	AddOutputPort("PerX_effExtr78",VALUE_DOUBLE,"");//78�鼶��Ч��

	//����5������ 
	AddInputPort(_T("PudX_x8Extr"), VALUE_DOUBLE, _T("�˶θɶ����ֵ"), _T(""));	//�˶θɶ����ֵ
	AddInputPort(_T("PudX_eff78Extr"), VALUE_DOUBLE, _T("78�μ���Ч�����ֵ"), _T(""));	//78�μ���Ч�����ֵ
	AddInputPort(_T("PerX_hi8thExtr"), VALUE_DOUBLE, _T("�˶γ���������"), _T(""));	//�˶γ���������
	AddInputPort(_T("PerX_h7thExtr"), VALUE_DOUBLE, _T("�߶γ�����ֵ"), _T(""));	//�߶γ�����ֵ
	AddInputPort(_T("PubX_p8thExtr"), VALUE_DOUBLE, _T("#8����ѹ��"), _T(""));	//#8����ѹ��

}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CH8CalcBlock::CreateFromString( const CString& strType )
{
	CH8CalcBlock* obj = new CH8CalcBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CH8CalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

double bph_x(double para1,double para2)
{
	typedef void (_stdcall *PH2X97)(double P,double H,double & X,int & Range);
	PH2X97 proc;
	HINSTANCE hst = ::LoadLibrary("WASP.dll");
	if(!hst)
		return 0;
	proc = (PH2X97)::GetProcAddress(hst, "PH2X97");
	if(!proc)
		return 0;
	//��1�����ֵ��ʾѹ��
	double P=para1;
	//��2�����ֵ��ʾ����
	double T=para2;
	double H=0.0;
	int R=0;
	
	//ֱ�ӵ���97��׼�ĺ���
	proc(P,T,H,R);
	//printf("ֱ�ӵ���97��׼�ĺ�����h=%10.5f\n",H);
	return H;
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CH8CalcBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
	/////////////////////////////////////
	//�������

	CCalcPort *pPortObj1 = GetOutputPortObj("PerX_h8thExtr"); //8����ֵ
	CCalcPort *pPortObj2 = GetOutputPortObj("PerX_x8thExtrStm"); //�˶γ����ɶ�
	CCalcPort *pPortObj3 = GetOutputPortObj("PerX_effExtr78"); //78�鼶��Ч��
	if(pPortObj1 && pPortObj2 && pPortObj3)
	{
		double result=0;
		CUniValue a,b,c,d,e;
		BOOL ret1 = GetInputValue("PudX_x8Extr",&a);//�˶θɶ����ֵ
		BOOL ret2 = GetInputValue("PudX_eff78Extr",&b);//78�μ���Ч�����ֵ
		BOOL ret3 = GetInputValue("PerX_hi8thExtr",&c);//�˶γ���������
		BOOL ret4 = GetInputValue("PubX_p8thExtr",&d);//#8����ѹ��
		BOOL ret5 = GetInputValue("PerX_h7thExtr",&e);//7����ֵ

		if(ret1 && ret2 && ret3 && ret4 && ret5)
		{
			double x8_0,Eta78_0,hi8,p8,h7;
			x8_0 = a.GetValueDouble()/100;	//�˶θɶ����ֵ
			Eta78_0 = b.GetValueDouble()/100;//78�μ���Ч�����ֵ
			hi8 = c.GetValueDouble();//�˶γ���������
		    p8 = d.GetValueDouble(); //#8����ѹ��
			h7 = e.GetValueDouble();//7����ֵ

			//��8����ֵ
			double Eta78 = Eta78_0/((1+x8_0)/2);
			double h80 = h7-Eta78*(h7-hi8);	//h80��ʼֵ
			double x8,h8;

			int numCycle = 0;
			do
			{
				numCycle++;
				x8 = bph_x(p8,h80);

				h8 = h80+(1-x8)*(h7-hi8);

				if(abs(h8-h80) < 0.001)
					break;
				h80 = h8;
				if(numCycle > 10)	//ѭ������
					break;
			}
			while(true);//10�ĸ�3�η�
			//1)	��78Ϊ��ǰ��78�鼶��Ч�ʣ�
			//2)	x8Ϊ�˶γ����ɶȣ�
			//3)	h8Ϊ�˶γ�����ֵ��

			CUniValue UniVar1;
			UniVar1.AddData(h8);
			pPortObj1->SetPortUniValue(UniVar1);

			CUniValue UniVar2;
			UniVar1.AddData(x8);
			pPortObj2->SetPortUniValue(UniVar2);

			CUniValue UniVar3;
			UniVar1.AddData(Eta78);
			pPortObj3->SetPortUniValue(UniVar3);
		}
	}

	///////////////////////////////////
	//�������
	OutputResultToGlobalWS();
}