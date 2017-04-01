// SineWaveBlock.cpp: implementation of the CSineWaveBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SineWaveBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include<math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSineWaveBlock::CSineWaveBlock()
{
	m_iComIndex=0;

	m_dFengzhi=1;
	m_dZhouqi=1;
	m_dLeftRight=0;
	m_dUpDown=0;
	m_dInitValue=0;
	m_dStep=3;

	m_iInputNum=0;
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "SineWave" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===

}

CSineWaveBlock::~CSineWaveBlock()
{

}
//�����ʼ�������������
void CSineWaveBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CSineWaveBlock::CreateFromString( const CString& strType )
{
	CSineWaveBlock* obj = new CSineWaveBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CSineWaveBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CSineWaveBlock::Calc() 
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
		m_Result=m_dFengzhi*sin(m_dZhouqi*dInput+m_dLeftRight)+m_dUpDown;          
	}
	else if(m_iInputNum==0)
	{
		m_Result=m_dFengzhi*sin(m_dZhouqi*m_dInitValue+m_dLeftRight)+m_dUpDown; 
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
void CSineWaveBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SineWave1")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFengzhi=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dZhouqi=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dLeftRight=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dUpDown=atof(strItem4);		
	}
	if(strPropName.Compare("SineWave2")==0)
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
void CSineWaveBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgSineWave  dlg;

	dlg.m_index =m_iComIndex;

	dlg.m_dFengzhi=m_dFengzhi;
	dlg.m_dZhouqi=	m_dZhouqi;
	dlg.m_dLeftRight=	m_dLeftRight;
	dlg.m_dUpDown=	m_dUpDown;
	dlg.m_dInitValue=	m_dInitValue;
	dlg.m_dStep=	m_dStep;
    ///
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_index==0)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dLeftRight =	dlg.m_dLeftRight;
			m_dUpDown    =	dlg.m_dUpDown;

		//	m_dInitValue =	dlg.m_dInitValue;
		//	m_dStep      =	dlg.m_dStep;

			m_iInputNum=1;

		}
		else if(dlg.m_index==1)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dLeftRight =	dlg.m_dLeftRight;
			m_dUpDown    =	dlg.m_dUpDown;
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
void CSineWaveBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("SineWave1"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFengzhi) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dZhouqi) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dLeftRight) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dUpDown) );

	//����5-6�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("SineWave2"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
    pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_iComIndex) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dInitValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dStep) );

	pRecord->Update();

}
//����ͼ��ʱ����Ҫ�������ԡ�
void CSineWaveBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iComIndex=((CSineWaveBlock*)pBlock)->m_iComIndex;
	m_dFengzhi=((CSineWaveBlock*)pBlock)->m_dFengzhi;
	m_dZhouqi=((CSineWaveBlock*)pBlock)->m_dZhouqi;
	m_dLeftRight=((CSineWaveBlock*)pBlock)->m_dLeftRight;
	m_dUpDown=((CSineWaveBlock*)pBlock)->m_dUpDown;
	m_dInitValue=((CSineWaveBlock*)pBlock)->m_dInitValue;
	m_dStep=((CSineWaveBlock*)pBlock)->m_dStep;
}
//�����Է��ô���������
void CSineWaveBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SineWave1");
	str.Format("%f",m_dFengzhi);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dZhouqi);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dLeftRight);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dUpDown);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SineWave2");
	str.Format("%d",m_iComIndex);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dInitValue);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dStep);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}