// CoalBlock.cpp: implementation of the CCoalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "CoalBlock.h"
#include "PropertyDlgCoal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoalBlock::CCoalBlock()
{
	m_bFixMf=1;
	m_dMf=10;
	m_iMs=0;
	m_iS=0;
	m_iAshAr=0;
	m_iLwCoalAr=0;
	m_iH=0;

	SetBlockType(_T( "Coal" ) );//��������
	ConstructBlockInputOutput ();
}

CCoalBlock::~CCoalBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CCoalBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("iMs"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("iAshAr"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddInputPort(_T("iLwCoalAr"), VALUE_DOUBLE, _T("in3"), _T("in3"));
	
	AddOutputPort("Ms",VALUE_DOUBLE,"out1");
	AddOutputPort("AshAr",VALUE_DOUBLE,"out2");
	AddOutputPort("LwCoalAr",VALUE_DOUBLE,"out3");
	AddOutputPort("H",VALUE_DOUBLE,"out4");
}
//���������½�����ĺ���
CCalcBlock* CCoalBlock::CreateFromString( const CString& strType )
{
	CCoalBlock* obj = new CCoalBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CCoalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CCoalBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPort_Ms = GetOutputPortObj(0);
	CCalcPort *pOutPort_AshAr = GetOutputPortObj(1);
	CCalcPort *pOutPort_LwCoalAr = GetOutputPortObj(2);
	CCalcPort *pOutPort_H = GetOutputPortObj(3);

	CCalcPort *pInput_Ms=GetInputPortObj(0);
	CCalcPort *pInput_AshAr=GetInputPortObj(1);
	CCalcPort *pInput_LwCoalAr=GetInputPortObj(2);
	CCalcPort *pInput_Mf=NULL;
	if(!m_bFixMf)
		pInput_Mf=GetInputPortObj(3);
	
	//ˮ��0.ȫˮ�֣��յ���ˮ�֣�1.������ˮ��
	double dMs,dMf,dMad,dResultMs;
	pInput_Ms->GetPortUniValue().GetDoubleVal(&dMs);
	if(!m_bFixMf)
		pInput_Mf->GetPortUniValue().GetDoubleVal(&dMf);
	else
		dMf=m_dMf;
	CUniValue UniVarMs;
	if(this->m_iMs==0)
	{
		dResultMs=dMs;
		UniVarMs.AddData(dResultMs);
		dMad=(dMs - dMf)*100/(100 - dMf);
	}
	else if(this->m_iMs==1)
	{
		dResultMs=dMf + dMs*(100-dMf)/100;
		UniVarMs.AddData(dResultMs);
		dMad=dMs;
	}
	pOutPort_Ms->SetPortUniValue(UniVarMs);
	
	//�ҷּ���  0.Ӧ�û��ҷ� 1.�������ҷ� 2.������ҷ� 3.�����޻һ��ҷ�
	double dAshAr,dResultAshAr;
	pInput_AshAr->GetPortUniValue().GetDoubleVal(&dAshAr);
	CUniValue UniVarAshAr;
	if(this->m_iAshAr==0)
	{
		UniVarAshAr.AddData(dAshAr);
	}
	else if(this->m_iAshAr==1)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/(100-dMad);
		UniVarAshAr.AddData(dResultAshAr);
	}
	else if(this->m_iAshAr==2)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/100;
		UniVarAshAr.AddData(dResultAshAr);
	}
	else if(this->m_iAshAr==3)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/(100+dAshAr);
		UniVarAshAr.AddData(dResultAshAr);
	}
	pInput_AshAr->SetPortUniValue(UniVarAshAr);

	//��λ���������� 0.�յ�����λ������ 1.��������λ������ 2.�������λ������ 3.�����޻һ���λ������
	double dLwCoalAr,dResultLwCoalAr;
	pInput_LwCoalAr->GetPortUniValue().GetDoubleVal(&dLwCoalAr);
	CUniValue UniVarLwCoalAr;
	if(this->m_iLwCoalAr==0)
	{
		dResultLwCoalAr=dLwCoalAr;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==1)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs)/(100-dMad);
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==2)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs)/100;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==3)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs-dResultAshAr)/100;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	pInput_LwCoalAr->SetPortUniValue(UniVarLwCoalAr);
	
	//�⺬������ 0.����ú 1.ƶú 2.��ú(20~30) 3.��ú(30~40) 4.����ú 5.��ú
	double dResultH;
	CUniValue UniVarH;
	if(this->m_iH==0)
	{
		dResultH=2;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.8;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.5;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.5;
		UniVarH.AddData(dResultH);
	}
	pOutPort_H->SetPortUniValue(UniVarH);

	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CCoalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Mf")==0)
	{
		if(!strItem1.IsEmpty())  m_bFixMf=atoi(strItem1);
		if(!strItem2.IsEmpty())  m_dMf=atof(strItem2);
	}
	if(strPropName.Compare("Ms")==0)
	{
		if(!strItem1.IsEmpty())  m_iMs=atoi(strItem1);
	}
	if(strPropName.Compare("S")==0)
	{
		if(!strItem1.IsEmpty())  m_iS=atoi(strItem1);
	}
	if(strPropName.Compare("AshAr")==0)
	{
		if(!strItem1.IsEmpty())  m_iAshAr=atoi(strItem1);
	}
	if(strPropName.Compare("LwCoalAr")==0)
	{
		if(!strItem1.IsEmpty())  m_iLwCoalAr=atoi(strItem1);
	}
	if(strPropName.Compare("H")==0)
	{
		if(!strItem1.IsEmpty())  m_iH=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CCoalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgCoal dlg;
    //���Ի��������ֵ
	dlg.m_bFixMf = this->m_bFixMf;
	dlg.m_dMf=this->m_dMf;
	dlg.m_iAshAr=this->m_iAshAr;
	dlg.m_iH=this->m_iH;
	dlg.m_iLwCoalAr=this->m_iLwCoalAr;
	dlg.m_iMs=this->m_iMs;
	dlg.m_iS=this->m_iS;
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_bFixMf = dlg.m_bFixMf;
		m_dMf=dlg.m_dMf;
		m_iAshAr=dlg.m_iAshAr;
		m_iH=dlg.m_iH;
		m_iLwCoalAr=dlg.m_iLwCoalAr;
		m_iMs=dlg.m_iMs;
		m_iS=dlg.m_iS;

		if((this->GetInputNum()==3)&&(!m_bFixMf))
		{
			AddInputPort(_T("iMf"), VALUE_DOUBLE, _T("in4"), _T("in4"));
		}
		else if((this->GetInputNum()==4)&&(m_bFixMf))
		{
			this->ClearInputPortByIndex(3);
		}
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
void CCoalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="Mf";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_bFixMf); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%f",m_dMf); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="Ms";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iMs); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="S";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iS); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="AshAr";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iAshAr); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="LwCoalAr";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iLwCoalAr); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="H";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iH); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->Update();
}
void CCoalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dMf=((CCoalBlock*)pBlock)->m_dMf;
	m_bFixMf=((CCoalBlock*)pBlock)->m_bFixMf;
	m_iH=((CCoalBlock*)pBlock)->m_iH;
	m_iLwCoalAr=((CCoalBlock*)pBlock)->m_iLwCoalAr;
	m_iMs=((CCoalBlock*)pBlock)->m_iMs;
	m_iS=((CCoalBlock*)pBlock)->m_iS;
	m_iAshAr=((CCoalBlock*)pBlock)->m_iAshAr;
}
	//�����Է��ô���������
void CCoalBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Mf");
	CString str;
	str.Format("%d",m_bFixMf);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dMf);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Ms");
	str.Format("%d",m_iMs);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"S");
	str.Format("%d",m_iS);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"AshAr");
	str.Format("%d",m_iAshAr);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"LwCoalAr");
	str.Format("%d",m_iLwCoalAr);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"H");
	str.Format("%d",m_iH);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
