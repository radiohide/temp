// NumConverToStrBlock.cpp: implementation of the CNumConverToStrBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "NumConverToStrBlock.h"
#include "PropertyDlgNumToStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumConverToStrBlock::CNumConverToStrBlock()
{
	SetBlockType(_T( "NumConverToStr" ) );
	ConstructBlockInputOutput();
}

CNumConverToStrBlock::~CNumConverToStrBlock()
{

}
//�����ʼ�������������
void CNumConverToStrBlock::ConstructBlockInputOutput()
{
	AddInputPort("In",VALUE_DOUBLE, _T("out1"), _T("out1"));	
	AddOutputPort("Out",VALUE_RTVALUE_SERIES,"out1");	
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CNumConverToStrBlock::CreateFromString( const CString& strType )
{
	CNumConverToStrBlock* obj = new CNumConverToStrBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//��ʼ������ĺ���������ĳ�ʼ��
void CNumConverToStrBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CNumConverToStrBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
	CCalcPort *pPortObjIn = GetInputPortObj(0);
	double dInValue;
	pPortObjIn->GetPortUniValue().GetDoubleVal(&dInValue);
	int iInValue=(int)dInValue;// ȡ����������
	CString strIn;
	strIn.Format("%d",iInValue);

	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	for(int i=0;i<strIn.GetLength();i++)
	{
		int iChar=strIn.GetAt(i);
		UniVar.AddData((double)iChar);
	}
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//�������
	OutputResultToGlobalWS();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CNumConverToStrBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
}

//��������ʾ��ͬ�����Զ���Ի���
void CNumConverToStrBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgNumToStr dlg;
	dlg.DoModal();
}
//�����Խ���ģ��д�뵽���ݿ���
void CNumConverToStrBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CNumConverToStrBlock::CopyPropertyData(CCalcBlock* pBlock)
{

}
//�����Է��ô���������
void CNumConverToStrBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}