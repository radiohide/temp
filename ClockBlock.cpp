// ClockBlock.cpp: implementation of the CClockBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ClockBlock.h"
#include "PropertyDlgClock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClockBlock::CClockBlock()
{
	SetBlockType(_T("Clock") );
	m_iBeHaveTime=1;//0:�������1�����
	m_iBeHaveBeginEndTime=0;//0:�������1�����
	ConstructBlockInputOutput();
}

CClockBlock::~CClockBlock()
{
	
}
void CClockBlock::ConstructBlockInputOutput ()
{
	//�����ɶ�����������ı������ơ�
	ClearOutputPorts();
	if(1==m_iBeHaveTime)
	{
		AddOutputPort("Time", VALUE_DOUBLE);
	}
	if(1==m_iBeHaveBeginEndTime)
	{
		AddOutputPort("StartT", VALUE_DOUBLE);
		AddOutputPort("EndT", VALUE_DOUBLE);
	}
}
	//���������½�����ĺ���
CCalcBlock* CClockBlock::CreateFromString( const CString& strType )
{
	CClockBlock* obj = new CClockBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CClockBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iBeHaveTime=((CClockBlock*)pBlock)->m_iBeHaveTime;
	m_iBeHaveBeginEndTime=((CClockBlock*)pBlock)->m_iBeHaveBeginEndTime;
}
	//��ʼ������ĺ���������ĳ�ʼ��
void CClockBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//���㺯����ʵ�ֱ�ģ��ļ���
void CClockBlock::Calc() //���м���ĺ���,��Ҫ����
{
	if((1==m_iBeHaveTime)&&(1==m_iBeHaveBeginEndTime))//ȫ�����
	{
		CUniValue UniVar1;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar1.AddData(this->m_pModel->m_lDataTime);
		pOutPortObj->SetPortUniValue(UniVar1);

		CUniValue UniVar2;
		pOutPortObj=GetOutputPortObj(1);
		UniVar2.AddData(this->m_pModel->m_lDataSTime);
		pOutPortObj->SetPortUniValue(UniVar2);

		CUniValue UniVar3;
		pOutPortObj=GetOutputPortObj(2);
		UniVar3.AddData(this->m_pModel->m_lDataETime);
		pOutPortObj->SetPortUniValue(UniVar3);
	}
	if((1==m_iBeHaveTime)&&(1!=m_iBeHaveBeginEndTime))//ֻ���ʱ��
	{
		CUniValue UniVar1;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar1.AddData(this->m_pModel->m_lDataTime);
		pOutPortObj->SetPortUniValue(UniVar1);
	}
	if((1!=m_iBeHaveTime)&&(1==m_iBeHaveBeginEndTime))//ֻ���ʱ��
	{
		CUniValue UniVar2;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar2.AddData(this->m_pModel->m_lDataSTime);
		pOutPortObj->SetPortUniValue(UniVar2);

		CUniValue UniVar3;
		pOutPortObj=GetOutputPortObj(1);
		UniVar3.AddData(this->m_pModel->m_lDataETime);
		pOutPortObj->SetPortUniValue(UniVar3);
	}
	
	////////////////////////////////////////////////////////////////////////
   //����������ռ���
	OutputResultToGlobalWS();
}
void CClockBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//�����Է��ô���������
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeHaveTime");
	
	CString str;
	str.Format("%d",m_iBeHaveTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeHaveBeginEndTime");
	
	str.Format("%d",m_iBeHaveBeginEndTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CClockBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("BeHaveTime")==0)
	{
		if(!strItem1.IsEmpty())
			m_iBeHaveTime=atoi(strItem1);
	}
	else if(strPropName.Compare("BeHaveBeginEndTime")==0)
	{
		if(!strItem1.IsEmpty())
			m_iBeHaveBeginEndTime=atoi(strItem1);
	}
}
	//��������ʾ��ͬ�����Զ���Ի���
void CClockBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//������ڲ�������˿�����
    IDArrRightIn.RemoveAll();
	

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}
	//
	CPropertyDlgClock dlg;
    //���Ի��������ֵ
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_iBeHaveTime=m_iBeHaveTime;
	dlg.m_iBeHaveBeginEndTime=m_iBeHaveBeginEndTime;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_iBeHaveTime = dlg.m_iBeHaveTime;
		m_iBeHaveBeginEndTime=dlg.m_iBeHaveBeginEndTime;
		
		//������˿ڵ��޸�
		if(1==m_iBeHaveTime)
		{
			//AddOutputPort("Time", VALUE_DOUBLE);
			IDArrRightIn.Add("Time");
		}
		if(1==m_iBeHaveBeginEndTime)
		{
			//AddOutputPort("StartT", VALUE_DOUBLE);
			//AddOutputPort("EndT", VALUE_DOUBLE);
			IDArrRightIn.Add("StartT");
			IDArrRightIn.Add("EndT");
		}
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if(IDArrRightIn[kk]!=IDArrRightOut[jj])	
				{runs++;}
			}
			if(runs==IDArrRightIn.GetSize())
			{
				this->ClearOutputPortByIndex(jj,parent);
			}
		}		
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//����µ�Port
		{
			int  runs=0;//��������
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if(IDArrRightIn[mm]!=IDArrRightOut[nn])
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
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
void CClockBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("BeHaveTime"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	strField.Format("%d",m_iBeHaveTime); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("BeHaveBeginEndTime"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	strField.Format("%d",m_iBeHaveBeginEndTime); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );

	pRecord->Update();
}
