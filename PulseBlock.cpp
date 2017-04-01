// PulseBlock.cpp: implementation of the CPulseBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PulseBlock.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPulseBlock::CPulseBlock()
{
	m_iComIndex=0;

	m_dFengzhi = 3.0;
	m_dFengzhiNegative = 0.0;
	m_dZhouqi = 2.0;
	m_dYanchi = 0.0;
	m_dMaichongKD = 0.5;

	m_dInitValue = 0.0;
	m_dStep = 3.0;

	m_iInputNum=0;
		//SYSTME=====��������======
	//��������
	SetBlockType(_T( "Pulse" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===

}

CPulseBlock::~CPulseBlock()
{

}

//�����ʼ�������������
void CPulseBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CPulseBlock::CreateFromString( const CString& strType )
{
	CPulseBlock* obj = new CPulseBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CPulseBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CPulseBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//���м���
	int inNum = GetInputNum();
	double m_Result=0;
    if(m_iInputNum==1)//�ⲿ����
	{
		CCalcPort *pInPutPort = GetInputPortObj(0);		
		double dInput;
		pInPutPort->GetPortUniValue().GetDoubleVal(&dInput);
		
		if(dInput<m_dYanchi)
		{
			m_Result=m_dFengzhiNegative;
		}
		else
		{
			double dYushu=0;
			dYushu=dInput-m_dYanchi;
			double dInside=dYushu-(int)(dYushu/m_dZhouqi)*m_dZhouqi;
			if(dInside<=m_dZhouqi*m_dMaichongKD)
			{
				m_Result=m_dFengzhi;
			}
			else
			{
				m_Result=m_dFengzhiNegative;
			}	
		}
		m_dInitValue+=m_dStep;         
	}
	else if(m_iInputNum==0)
	{
		if(m_dInitValue<m_dYanchi)
		{
			m_Result=m_dFengzhiNegative;
		}
		else
		{
			double dYushu=0;
			dYushu=m_dInitValue-m_dYanchi;
			double dInside=dYushu-(int)(dYushu/m_dZhouqi)*m_dZhouqi;
			if(dInside<=m_dZhouqi*m_dMaichongKD)
			{
				m_Result=m_dFengzhi;
			}
			else
			{
				m_Result=m_dFengzhiNegative;
			}	
		}
		m_dInitValue+=m_dStep;
	}
	///////////////////////////////////
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	UniVar.AddData(m_Result);
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//�������
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CPulseBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Pulse1")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFengzhi=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dFengzhiNegative=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dZhouqi=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dYanchi=atof(strItem4);
		if(!strItem5.IsEmpty())
			m_dMaichongKD=atof(strItem5);		
	}
	if(strPropName.Compare("Pulse2")==0)
	{
		if(!strItem1.IsEmpty())
			m_iComIndex=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_dInitValue=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dStep=atof(strItem3);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CPulseBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgPulse  dlg;

	dlg.m_index =m_iComIndex;

	dlg.m_dFengzhi=m_dFengzhi;
	dlg.m_dFengzhiNegative= m_dFengzhiNegative;
	dlg.m_dZhouqi=	m_dZhouqi;
	dlg.m_dYanchi=	m_dYanchi;
	dlg.m_dMaichongKD=	m_dMaichongKD;
	dlg.m_dInitValue=	m_dInitValue;
	dlg.m_dStep=	m_dStep;
    ///
	
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_index==0)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dFengzhiNegative =dlg.m_dFengzhiNegative;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dYanchi    =	dlg.m_dYanchi;
			m_dMaichongKD    =	dlg.m_dMaichongKD;

			m_dInitValue =	dlg.m_dInitValue;
			m_dStep      =	dlg.m_dStep;

			m_iInputNum=1;
		}
		else if(dlg.m_index==1)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dFengzhiNegative = dlg.m_dFengzhiNegative;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dYanchi    =	dlg.m_dYanchi;
			m_dMaichongKD    =	dlg.m_dMaichongKD;
			m_dInitValue =	dlg.m_dInitValue;
			m_dStep      =	dlg.m_dStep;

			m_iInputNum=0;
		}
		if(IDArrLeftOut.GetSize()==m_iInputNum){}
		else
		{
			if(m_iInputNum==1)
			{
			ClearInputPorts();
			AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
			}
			else
			{
				ClearInputPorts();
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CPulseBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("Pulse1"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFengzhi) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dFengzhiNegative) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dZhouqi) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dYanchi) );
	pRecord->PutCollect(_variant_t("value_item5"),_variant_t(m_dMaichongKD) );

	//����5-6�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("Pulse2"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
    pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_iComIndex) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dInitValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dStep) );

	pRecord->Update();

}
void CPulseBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dFengzhi=((CPulseBlock*)pBlock)->m_dFengzhi;
	m_dFengzhiNegative=((CPulseBlock*)pBlock)->m_dFengzhiNegative;
	m_dZhouqi=((CPulseBlock*)pBlock)->m_dZhouqi;
	m_dYanchi=((CPulseBlock*)pBlock)->m_dYanchi;
	m_dMaichongKD=((CPulseBlock*)pBlock)->m_dMaichongKD;
	m_iComIndex=((CPulseBlock*)pBlock)->m_iComIndex;
	m_dInitValue=((CPulseBlock*)pBlock)->m_dInitValue;
	m_dStep=((CPulseBlock*)pBlock)->m_dStep;
}
void CPulseBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//�����Է��ô���������
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Pulse1");
	str.Format("%f",m_dFengzhi);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dFengzhiNegative);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dZhouqi);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dYanchi);strcpy(pData->m_cValueItem4,str);
	str.Format("%f",m_dMaichongKD);strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Pulse2");
	str.Format("%d",m_iComIndex);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dInitValue);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dStep);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}
