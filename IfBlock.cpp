// IfBlock.cpp: implementation of the CIfBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "IfBlock.h"
#include "PropertyDlgIF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIfBlock::CIfBlock()
{
	SetBlockType(_T( "IF" ) );//��������
	m_iDataType=0;//Ĭ��
	ConstructBlockInputOutput();
}

CIfBlock::~CIfBlock()
{
	
}
void CIfBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("IF"), VALUE_DOUBLE);
	AddInputPort(_T("InValue1"), VALUE_DOUBLE);
	AddInputPort(_T("InValue2"), VALUE_DOUBLE);
	AddOutputPort("Out",VALUE_DOUBLE);
} 
//���������½�����ĺ���
CCalcBlock* CIfBlock::CreateFromString( const CString& strType )
{
	CIfBlock* obj = new CIfBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CIfBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CIfBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInputIfPortObj=GetInputPortObj(0);
	double ifDValue;
	pInputIfPortObj->GetPortUniValue().GetDoubleVal(&ifDValue);
	if(ifDValue>0)
	{
		pOutPortObj->SetPortUniValue(GetInputPortObj(1)->GetPortUniValue());//ȡ����1
	}
	else
	{
		pOutPortObj->SetPortUniValue(GetInputPortObj(2)->GetPortUniValue());//ȡ����2
	}
	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CIfBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("dateType")==0)
	{
		if(!strItem1.IsEmpty())  m_iDataType=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CIfBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgIF dlg;
	CString strTagName;
    //���Ի��������ֵ
    dlg.SetType(this->m_iDataType);
	int SetBefore=this->m_iDataType;
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		this->m_iDataType = dlg.GetType();
		int SetBehind=dlg.GetType();
		
		/*
		ClearInputPorts();
		ClearOutputPorts();
		AddInputPort(_T("If"), VALUE_DOUBLE, _T("If"), _T("If"));
		AddInputPort(_T("InValue1"), m_iDataType, _T("in1"), _T("in1"));
		AddInputPort(_T("InValue2"), m_iDataType, _T("in2"), _T("in2"));
		AddOutputPort("out",m_iDataType,"out");
		*/
		if(SetBefore==SetBehind){}
		else
		{
			this->ClearInputPortByIndex(2);
			this->ClearInputPortByIndex(1);
			ClearOutputPorts();

			AddInputPort(_T("InValue1"), m_iDataType, _T("in1"), _T("in1"));
			AddInputPort(_T("InValue2"), m_iDataType, _T("in2"), _T("in2"));
			AddOutputPort("out",m_iDataType,"out");
		}
		parent->RedrawWindow();
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
	}
	//////////////////////////////////////////////////
}
//�����Խ���ģ��д�뵽���ݿ���
void CIfBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="dateType";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iDataType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}
void CIfBlock::CopyPropertyData(CCalcBlock* pBlock)
{	
	m_iDataType=((CIfBlock*)pBlock)->m_iDataType;
}
	//�����Է��ô���������
void CIfBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"dateType");
	
	CString str;
	str.Format("%d",m_iDataType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}