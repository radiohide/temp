// HistSeriesBuffBlock.cpp: implementation of the CHistSeriesBuffBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "HistSeriesBuffBlock.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistSeriesBuffBlock::CHistSeriesBuffBlock()
{
	m_strPointName ="";
	m_iDataSource =0;
	m_iReadModel =0;
	m_ISnapStep=5;
	m_iOutputTime=300;
	m_iBuffTime=600;

	bSureFull=FALSE;
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "HistSeriesBuff" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===

}

CHistSeriesBuffBlock::~CHistSeriesBuffBlock()
{

}
//�����ʼ�������������
void CHistSeriesBuffBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out1",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CHistSeriesBuffBlock::CreateFromString( const CString& strType )
{
	CHistSeriesBuffBlock* obj = new CHistSeriesBuffBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CHistSeriesBuffBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	m_listDataBuffer.RemoveAll();
	this->m_pModel->LoadRTDBDllAndCon();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CHistSeriesBuffBlock::Calc() 
{
	if(!this->m_pModel->m_bConRTDB)
		return;
	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	
	if(m_iDataSource==0)//�����ڲ�ʱ��
	{
        if(m_iReadModel==0)//ԭʼֵ
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//��ȡ��������
            if(bSureFull==FALSE)
			{
				//��һ���򻺳����������
				///*
				int nRet=0;
				TagData *pTDataTemp;
				req.stTime=this->m_pModel->m_lDataETime-m_iBuffTime;
				req.enTime=this->m_pModel->m_lDataETime;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=0;
				req.reqType=0;
				do
				{			
					nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					else
					{
						for (int j=0; j<nCount; j++)
						{
							pTDataTemp=NULL;
							pTDataTemp->value=tagDatas[j].value;
							pTDataTemp->time=tagDatas[j].time;
							pTDataTemp->status=tagDatas[j].status;
							m_listDataBuffer.AddTail(pTDataTemp);
						}
						if(nCount>0)
						{
							req.stTime=pTDataTemp->time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
						}
					}
				}while ((nCount==1024) && (req.stTime !=req.enTime));
				//*/
				/*
				Separate(this->m_pModel->m_lDataETime-m_iBuffTime,this->m_pModel->m_lDataETime);
				for(int j=0; j<Temp_begintime.size(); j++)
				{
				req.stTime=Temp_begintime[j];
				req.enTime=Temp_endtime[j];
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=0;
				req.reqType=0;
				int nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
				if(nRet){return;}
				
				  TagData *pTDataTemp;
				  for(int i=0; i<nCount; i++)
					{
						pTDataTemp=new TagData;
						pTDataTemp->value=tagDatas[i].value;
						pTDataTemp->time=tagDatas[i].time;
						pTDataTemp->status=tagDatas[i].status;
						m_listDataBuffer.AddTail(pTDataTemp);
					}	
				}
                */
				RemoveElementFromBuffer(this->m_pModel->m_lDataETime);
				bSureFull=TRUE;
			}
			else if(bSureFull==TRUE)
			{
				//��һ��֮������ʱ�����������
				int nRet=0;
				TagData *pTDataTemp;
				req.stTime=this->m_pModel->m_lDataSTime;
				req.enTime=this->m_pModel->m_lDataETime;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=0;
				req.reqType=0;
				do
				{			
					nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					else
					{
						for (int j=0; j<nCount; j++)
						{
							pTDataTemp=NULL;
							pTDataTemp->value=tagDatas[j].value;
							pTDataTemp->time=tagDatas[j].time;
							pTDataTemp->status=tagDatas[j].status;
							m_listDataBuffer.AddTail(pTDataTemp);
						}
						if(nCount>0)
						{
							req.stTime=pTDataTemp->time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
						}
					}
				}while ((nCount==1024) && (req.stTime !=req.enTime));
				/*
				Separate(this->m_pModel->m_lDataSTime,this->m_pModel->m_lDataETime);
				
				for(int j=0; j<Temp_begintime.size(); j++)
				{
					req.stTime=Temp_begintime[j];
					req.enTime=Temp_endtime[j];
					strcpy(req.pointName,m_strPointName);
					req.tPeriod=0;
					req.reqType=0;
					int nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					
					TagData *pTDataTemp;
					for(int i=0; i<nCount; i++)
					{
						pTDataTemp=new TagData;
						pTDataTemp->value=tagDatas[i].value;
						pTDataTemp->time=tagDatas[i].time;
						pTDataTemp->status=tagDatas[i].status;
						m_listDataBuffer.AddTail(pTDataTemp);
					}	
				}
				*/
				RemoveElementFromBuffer(this->m_pModel->m_lDataETime);
			}
			
			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			
			TagData *tempElement ;
			for (int i=0;i < m_listDataBuffer.GetCount();i++)
			{
				tempElement=m_listDataBuffer.GetAt(m_listDataBuffer.FindIndex(i));

				if(tempElement->time>=this->m_pModel->m_lDataETime-m_iOutputTime)
				{
					UniVar.AddData(tempElement->value);
				}
			}	
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//����ֵ
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			
		    //��һ���򻺳����������
			if(bSureFull==FALSE)
			{
				req.stTime=this->m_pModel->m_lDataETime-m_iBuffTime;
				req.enTime=this->m_pModel->m_lDataETime;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=m_ISnapStep;
				req.reqType=4;
				
				if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
				else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
				tagDatas = (TagData*)malloc(nCount*sizeof(TagData));
				int nRet = m_GetSnapshotDataByTagName(&req,tagDatas);
				if(nRet){return;}	
				
				TagData *pTDataTemp;
				
				for(int m=0; m<nCount; m++)	
				{
					pTDataTemp=new TagData;
					pTDataTemp->value=tagDatas[m].value;
					pTDataTemp->time=tagDatas[m].time;
					pTDataTemp->status=tagDatas[m].status;				
					m_listDataBuffer.AddTail(pTDataTemp);
				}
				RemoveElementFromBuffer(this->m_pModel->m_lDataETime);
				bSureFull=TRUE;
			}
			else if(bSureFull==TRUE)
			{
				req.stTime=this->m_pModel->m_lDataSTime;
				req.enTime=this->m_pModel->m_lDataETime;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=m_ISnapStep;
				req.reqType=4;
				
				if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
				else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
				tagDatas = (TagData*)malloc(nCount*sizeof(TagData));
				int nRet = m_GetSnapshotDataByTagName(&req,tagDatas);
				if(nRet){return;}	
				
				TagData *pTDataTemp;
				
				for(int m=0; m<nCount; m++)	
				{
					pTDataTemp=new TagData;
					pTDataTemp->value=tagDatas[m].value;
					pTDataTemp->time=tagDatas[m].time;
					pTDataTemp->status=tagDatas[m].status;				
					m_listDataBuffer.AddTail(pTDataTemp);
				}
				RemoveElementFromBuffer(this->m_pModel->m_lDataETime);
			}
		
					
			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����	
			TagData *tempElement ;
			for (int i=0;i < m_listDataBuffer.GetCount();i++)
			{
			    tempElement=m_listDataBuffer.GetAt(m_listDataBuffer.FindIndex(i));
				if(tempElement->time>=this->m_pModel->m_lDataETime-m_iOutputTime)
				{
					UniVar.AddData(tempElement->value);
				}
			}	
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();		
		}
	}
	else if(m_iDataSource==1)//�ⲿ����ʱ��
	{		
		if(m_iReadModel==0)//ԭʼֵ
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//��ȡ��������
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);
			//��һ���򻺳����������
			if(bSureFull==FALSE)
			{
				int nRet=0;
				TagData *pTDataTemp;
				req.stTime=dInput2-m_iBuffTime;
				req.enTime=dInput2;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=0;
				req.reqType=0;
				do
				{			
					nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					else
					{
						for (int j=0; j<nCount; j++)
						{
							pTDataTemp=NULL;
							pTDataTemp->value=tagDatas[j].value;
							pTDataTemp->time=tagDatas[j].time;
							pTDataTemp->status=tagDatas[j].status;
							m_listDataBuffer.AddTail(pTDataTemp);
						}
						if(nCount>0)
						{
							req.stTime=pTDataTemp->time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
						}
					}
				}while ((nCount==1024) && (req.stTime !=req.enTime));
				/*
				Separate(dInput2-m_iBuffTime,dInput2);
				
				for(int j=0; j<Temp_begintime.size(); j++)
				{
					req.stTime=Temp_begintime[j];
					req.enTime=Temp_endtime[j];
					strcpy(req.pointName,m_strPointName);
					req.tPeriod=0;
					req.reqType=0;
					int nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					TagData *pTDataTemp;
					for(int i=0; i<nCount; i++)
					{
						pTDataTemp=new TagData;
						pTDataTemp->value=tagDatas[i].value;
						pTDataTemp->time=tagDatas[i].time;
						pTDataTemp->status=tagDatas[i].status;
						m_listDataBuffer.AddTail(pTDataTemp);
					}	
				}
				*/
				RemoveElementFromBuffer(dInput2);
				bSureFull=TRUE;
			}
			else if(bSureFull==TRUE)
			{
				int nRet=0;
				TagData *pTDataTemp;
				req.stTime=dInput1;
				req.enTime=dInput2;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=0;
				req.reqType=0;
				do
				{			
					nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					else
					{
						for (int j=0; j<nCount; j++)
						{
							pTDataTemp=NULL;
							pTDataTemp->value=tagDatas[j].value;
							pTDataTemp->time=tagDatas[j].time;
							pTDataTemp->status=tagDatas[j].status;
							m_listDataBuffer.AddTail(pTDataTemp);
						}
						if(nCount>0)
						{
							req.stTime=pTDataTemp->time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
						}
					}
				}while ((nCount==1024) && (req.stTime !=req.enTime));
				/*
				Separate(dInput1,dInput2);
				
				for(int j=0; j<Temp_begintime.size(); j++)
				{
					req.stTime=Temp_begintime[j];
					req.enTime=Temp_endtime[j];
					strcpy(req.pointName,m_strPointName);
					req.tPeriod=0;
					req.reqType=0;
					int nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
					if(nRet){return;}
					TagData *pTDataTemp;
					for(int i=0; i<nCount; i++)
					{
						pTDataTemp=new TagData;
						pTDataTemp->value=tagDatas[i].value;
						pTDataTemp->time=tagDatas[i].time;
						pTDataTemp->status=tagDatas[i].status;
						m_listDataBuffer.AddTail(pTDataTemp);
					}	
				}
				*/
				RemoveElementFromBuffer(dInput2);
			}
			
			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			
			TagData *tempElement ;
			
			for (int i=0;i < m_listDataBuffer.GetCount();i++)
			{
				tempElement=m_listDataBuffer.GetAt(m_listDataBuffer.FindIndex(i));
				if(tempElement->time>=dInput2-m_iOutputTime)
				{
					UniVar.AddData(tempElement->value);
				}
			}	
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//����ֵ
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;

			//��ȡ��������
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);

			//��һ���򻺳����������
			if(bSureFull==FALSE)
			{
				req.stTime=dInput2-m_iBuffTime;
				req.enTime=dInput2;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=m_ISnapStep;
				req.reqType=4;
				
				if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
				else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
				tagDatas = (TagData*)malloc(nCount*sizeof(TagData));
				int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
				if(nRet){return;}
				
				TagData *pTDataTemp;
				
				for(int m=0; m<nCount; m++)	
				{
					pTDataTemp =new TagData;
					pTDataTemp->value=tagDatas[m].value;
					pTDataTemp->time=tagDatas[m].time;
					pTDataTemp->status=tagDatas[m].status;				
					m_listDataBuffer.AddTail(pTDataTemp);
				}
				RemoveElementFromBuffer(dInput2);
				bSureFull=TRUE;
			}
			else if(bSureFull==TRUE)
			{
				req.stTime=dInput1;
				req.enTime=dInput2;
				strcpy(req.pointName,m_strPointName);
				req.tPeriod=m_ISnapStep;
				req.reqType=4;
				
				if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
				else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
				tagDatas = (TagData*)malloc(nCount*sizeof(TagData));
				int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
				if(nRet){return;}
				
				TagData *pTDataTemp;
				
				for(int m=0; m<nCount; m++)	
				{
					pTDataTemp =new TagData;
					pTDataTemp->value=tagDatas[m].value;
					pTDataTemp->time=tagDatas[m].time;
					pTDataTemp->status=tagDatas[m].status;				
					m_listDataBuffer.AddTail(pTDataTemp);
				}
				RemoveElementFromBuffer(dInput2);
			}			
					
			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����	
			TagData *tempElement ;
			
			for (int i=0;i < m_listDataBuffer.GetCount();i++)
			{
				tempElement=m_listDataBuffer.GetAt(m_listDataBuffer.FindIndex(i));
				if(tempElement->time>=dInput2-m_iOutputTime)
				{
					UniVar.AddData(tempElement->value);
				}
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();			
		}
	}
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CHistSeriesBuffBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("HistSeriesBuff1")==0)
	{
		if(!strItem1.IsEmpty())
			m_strPointName=strItem1;	
		if(!strItem2.IsEmpty())
			m_iDataSource=atoi(strItem2);
		if(!strItem3.IsEmpty())
			m_iReadModel=atoi(strItem3);
		if(!strItem4.IsEmpty())
			m_ISnapStep=atoi(strItem4);
	}
	if(strPropName.Compare("HistSeriesBuff2")==0)
	{
		if(!strItem1.IsEmpty())
			m_iBuffTime=atoi(strItem1);	
		if(!strItem2.IsEmpty())
			m_iOutputTime=atoi(strItem2);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CHistSeriesBuffBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgHistSeriesBuff  dlg;

	dlg.m_strPointName=m_strPointName;
	dlg.m_iDataSource=m_iDataSource ;
	dlg.m_iReadModel=m_iReadModel ;
	dlg.m_ISnapStep=m_ISnapStep;
	dlg.m_iBuffTime=m_iBuffTime;
	dlg.m_iOutputTime=m_iOutputTime;
	//����
	if(dlg.DoModal()==IDOK)
	{
		m_strPointName=dlg.m_strPointName;
		m_iDataSource=dlg.m_iDataSource;
		m_iReadModel=dlg.m_iReadModel;
		m_ISnapStep=dlg.m_ISnapStep;
		m_iBuffTime=dlg.m_iBuffTime;
		m_iOutputTime=dlg.m_iOutputTime;

		//
		CStringArray  IDArrLeftIn;//������ڲ������Ըı䣬��ʹ�ⲿҪ���µ�����˿�����
		IDArrLeftIn.RemoveAll();

		if(m_iDataSource==1)
		{
			IDArrLeftIn.Add("In1");
			IDArrLeftIn.Add("In2");
		}		
		
		//ɾ��ԭ�ж����õ�Port,����ɾ
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)
		{
			int  runs=0;//��������
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if(IDArrLeftIn[k]!=IDArrLeftOut[j])	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}
		//����µ�Port
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)
		{
			int  runs=0;//��������
			for(int n=0; n<IDArrLeftOut.GetSize(); n++)
			{
				if(IDArrLeftIn[m]!=IDArrLeftOut[n])
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
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
}
//�����Խ���ģ��д�뵽���ݿ���
void CHistSeriesBuffBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("HistSeriesBuff1"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strPointName) );
	strFieldValue.Format("%d",m_iDataSource);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iReadModel);
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(strFieldValue));
	strFieldValue.Format("%d",m_ISnapStep);
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(strFieldValue));

	//����5-6�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("HistSeriesBuff2"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	strFieldValue.Format("%d",m_iBuffTime);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iOutputTime);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CHistSeriesBuffBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strPointName=((CHistSeriesBuffBlock*)pBlock)->m_strPointName;
	m_iDataSource=((CHistSeriesBuffBlock*)pBlock)->m_iDataSource;
	m_iReadModel=((CHistSeriesBuffBlock*)pBlock)->m_iReadModel;
	m_ISnapStep=((CHistSeriesBuffBlock*)pBlock)->m_ISnapStep;
	m_iBuffTime=((CHistSeriesBuffBlock*)pBlock)->m_iBuffTime;
	m_iOutputTime=((CHistSeriesBuffBlock*)pBlock)->m_iOutputTime;
}
//�����Է��ô���������
void CHistSeriesBuffBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"HistSeriesBuff1");
	str.Format("%s",m_strPointName);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iDataSource);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iReadModel);strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_ISnapStep);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"HistSeriesBuff2");
	str.Format("%d",m_iBuffTime);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iOutputTime);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}

void CHistSeriesBuffBlock::Separate(long stablebegin, long stableend)
{
	//����̬ʱ��ν�С 10����һ��
    Temp_begintime.clear();
	Temp_endtime.clear();
	if(stableend<stablebegin+10*60)//����̬ʱ���ÿ10������һ�Σ���Ϊ���ʱ��ι���GetRawDataByTagName���ܹ�������ȫ������
	{
		Temp_begintime.push_back(stablebegin);
		Temp_endtime.push_back(stableend);
	}
	else if(stableend>=stablebegin+10*60)
	{
		int runs=(stableend-stablebegin)/(10*60);//ȡ��
	    double result=(stableend-stablebegin)%(10*60);//ȡ��
		for(int i=0; i<runs; i++)
		{
			Temp_begintime.push_back(stablebegin+10*60*i);
		    Temp_endtime.push_back(stablebegin+10*60*(i+1));
		}
		if(result!=0)
		{
			Temp_begintime.push_back(stablebegin+10*60*(runs));
		    Temp_endtime.push_back(stableend);
		}
	}
}

void CHistSeriesBuffBlock::RemoveElementFromBuffer(long lEndOfInputTime)
{
	do
	{
		TagData *tempElement ;
		tempElement=m_listDataBuffer.GetHead();
		if(tempElement->time<lEndOfInputTime-m_iBuffTime)
		{
			m_listDataBuffer.RemoveHead();
		}
		else
			break;
	}while(true);
}
