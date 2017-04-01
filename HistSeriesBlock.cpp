// HistSeriesBlock.cpp: implementation of the CHistSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "HistSeriesBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistSeriesBlock::CHistSeriesBlock()
{
	m_strPointName ="";
	m_iDataSource =0;
	m_iReadModel =0;
	m_ISnapStep=5;
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "HistSeries" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===
}

CHistSeriesBlock::~CHistSeriesBlock()
{

}
//�����ʼ�������������
void CHistSeriesBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out1",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CHistSeriesBlock::CreateFromString( const CString& strType )
{
	CHistSeriesBlock* obj = new CHistSeriesBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CHistSeriesBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CHistSeriesBlock::Calc() 
{
	if(!this->m_pModel->m_bConRTDB)//���δ�����򲻽��м��㡣
		return;

	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	typedef int (*pRTDBFreePointer)(void *p);
	pRTDBFreePointer RTDBFreePointer=(pRTDBFreePointer)GetProcAddress(this->m_pModel->hDLL,"RTDBFreePointer");
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	
	if(m_iDataSource==0)//�����ڲ�ʱ��
	{
        if(m_iReadModel==0)//ԭʼֵ
		{
			ReadHiDataRequest req;
			std::vector<TagData> m_TagData;//ʢ�����ڻ��ֵ�ÿһ������
			m_TagData.clear();
			
			TagData tagTemp;
			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//��ȡ����������
            req.stTime=this->m_pModel->m_lDataSTime;
			req.enTime=this->m_pModel->m_lDataETime;
			strcpy(req.pointName,m_strPointName);
			req.tPeriod=0;
			req.reqType=0;
			int nRet=0;
			do
			{			
				nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
				if(nRet)
				{
					RTDBFreePointer(tagDatas);
					return;
				}
				else
				{
					for (int j=0; j<nCount; j++)
					{
						tagTemp.value=tagDatas[j].value;
						tagTemp.time=tagDatas[j].time;
						tagTemp.status=tagDatas[j].status;
						m_TagData.push_back(tagTemp);
					}
					if(nCount>0)
					{
						req.stTime=tagTemp.time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
					}
				}
				RTDBFreePointer(tagDatas);
			}while ((nCount==1024) && (req.stTime !=req.enTime));

			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			for(int m=0; m<m_TagData.size(); m++){UniVar.AddData(m_TagData[m].value,m_TagData[m].time,m_TagData[m].status);}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//����ֵ
		{
			ReadHiDataRequest req;			
			long nCount = 0;

			//��ȡ����������
			req.stTime=this->m_pModel->m_lDataSTime;
			req.enTime=this->m_pModel->m_lDataETime;
			strcpy(req.pointName,m_strPointName);
			req.tPeriod=m_ISnapStep;
			req.reqType=4;
			
			if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
			else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
			TagData *tagDatas = new TagData[nCount];
			memset(tagDatas,0,sizeof(TagData)*nCount);
			int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
            if(nRet)
			{
				if(tagDatas!=NULL)
				{
					delete[] tagDatas;
					tagDatas=NULL;
				}
				return;
			}

			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			for(int m=0; m<nCount; m++)
			{
				UniVar.AddData(tagDatas[m].value,tagDatas[m].time,tagDatas[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
		}
	}
	else if(m_iDataSource==1)//�ⲿ����ʱ��
	{		
		if(m_iReadModel==0)//ԭʼֵ
		{
			ReadHiDataRequest req;
			std::vector<TagData> m_TagData;//ʢ�����ڻ��ֵ�ÿһ������
			m_TagData.clear();
			
			TagData tagTemp;
			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//��ȡ��������
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);
			//
			if(dInput1>dInput2)
			{
				double dTemp=dInput1;
				dInput1=dInput2;
				dInput2=dTemp;
			}
			int nRet=0;
			req.stTime=dInput1;
			req.enTime=dInput2;
			strcpy(req.pointName,m_strPointName);
			req.tPeriod=0;
			req.reqType=0;
			do
			{			
				nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
				if(nRet)
				{
					RTDBFreePointer(tagDatas);
					return;
				}
				else
				{
					for (int j=0; j<nCount; j++)
					{
						tagTemp.value=tagDatas[j].value;
						tagTemp.time=tagDatas[j].time;
						tagTemp.status=tagDatas[j].status;
						m_TagData.push_back(tagTemp);
					}
					if(nCount>0)
					{
						req.stTime=tagTemp.time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
					}
				}
				RTDBFreePointer(tagDatas);
			}while ((nCount==1024) && (req.stTime !=req.enTime));

			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			for(int m=0; m<m_TagData.size(); m++)
			{
				UniVar.AddData(m_TagData[m].value,m_TagData[m].time,m_TagData[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//����ֵ
		{
			ReadHiDataRequest req;			
			long nCount = 0;

			//��ȡ��������
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);
			if(dInput1>dInput2)
			{
				double dTemp=dInput1;
				dInput1=dInput2;
				dInput2=dTemp;
			}
			req.stTime=dInput1;
			req.enTime=dInput2;
			strcpy(req.pointName,m_strPointName);
			req.tPeriod=m_ISnapStep;
			req.reqType=4;
			
			if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
			else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
			TagData *tagDatas = new TagData[nCount];
			memset(tagDatas,0,sizeof(TagData)*nCount);
			int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
            if(nRet)
			{
				if(tagDatas!=NULL)
				{
					delete[] tagDatas;
					tagDatas=NULL;
				}
				return;
			}

			CUniValue UniVar;//�������
			UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
			for(int m=0; m<nCount; m++)
			{
				UniVar.AddData(tagDatas[m].value,tagDatas[m].time,tagDatas[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//�������
			OutputResultToGlobalWS();
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
		}
	}	
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CHistSeriesBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("HistSeries")==0)
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
}
//��������ʾ��ͬ�����Զ���Ի���
void CHistSeriesBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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
	CPropertyDlgHistSeries  dlg;

	dlg.m_strPointName=m_strPointName;
	dlg.m_iDataSource=m_iDataSource ;
	dlg.m_iReadModel=m_iReadModel ;
	dlg.m_ISnapStep=m_ISnapStep;
	//����
	if(dlg.DoModal()==IDOK)
	{
		m_strPointName=dlg.m_strPointName;
		m_iDataSource=dlg.m_iDataSource;
		m_iReadModel=dlg.m_iReadModel;
		m_ISnapStep=dlg.m_ISnapStep;	
		
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
void CHistSeriesBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("HistSeries"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strPointName) );
	strFieldValue.Format("%d",m_iDataSource);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iReadModel);
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(strFieldValue));
	strFieldValue.Format("%d",m_ISnapStep);
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(strFieldValue));

	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CHistSeriesBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strPointName=((CHistSeriesBlock*)pBlock)->m_strPointName;
	m_iDataSource=((CHistSeriesBlock*)pBlock)->m_iDataSource;
	m_iReadModel=((CHistSeriesBlock*)pBlock)->m_iReadModel;
	m_ISnapStep=((CHistSeriesBlock*)pBlock)->m_ISnapStep;

}
//�����Է��ô���������
void CHistSeriesBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"HistSeries");
	str.Format("%s",m_strPointName);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iDataSource);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iReadModel);strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_ISnapStep);strcpy(pData->m_cValueItem4,str);

	dataArr.Add(pData);
}

void CHistSeriesBlock::Separate(long stablebegin, long stableend)
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

