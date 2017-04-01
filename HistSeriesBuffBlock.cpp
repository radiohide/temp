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
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "HistSeriesBuff" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CHistSeriesBuffBlock::~CHistSeriesBuffBlock()
{

}
//构造初始的输入输出个数
void CHistSeriesBuffBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out1",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
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
//初始化计算的函数做特殊的初始化
void CHistSeriesBuffBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	m_listDataBuffer.RemoveAll();
	this->m_pModel->LoadRTDBDllAndCon();
}
//计算函数，实现本模块的计算
void CHistSeriesBuffBlock::Calc() 
{
	if(!this->m_pModel->m_bConRTDB)
		return;
	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	
	if(m_iDataSource==0)//采用内部时间
	{
        if(m_iReadModel==0)//原始值
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//获取两个输入
            if(bSureFull==FALSE)
			{
				//第一次向缓冲区填充填满
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
							req.stTime=pTDataTemp->time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
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
				//第一次之后按输入时间间隔填充数据
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
							req.stTime=pTDataTemp->time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
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
			
			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			
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
			//输出计算
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//快照值
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			
		    //第一次向缓冲区填充填满
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
		
					
			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型	
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
			//输出计算
			OutputResultToGlobalWS();		
		}
	}
	else if(m_iDataSource==1)//外部输入时间
	{		
		if(m_iReadModel==0)//原始值
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//获取两个输入
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);
			//第一次向缓冲区填充填满
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
							req.stTime=pTDataTemp->time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
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
							req.stTime=pTDataTemp->time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
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
			
			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			
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
			//输出计算
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//快照值
		{
			ReadHiDataRequest req;			
			TagData *tagDatas=NULL;
			long nCount = 0;

			//获取两个输入
			CCalcPort *pInPutPort1 = GetInputPortObj(0);		
			double dInput1;
			pInPutPort1->GetPortUniValue().GetDoubleVal(&dInput1);
			
			CCalcPort *pInPutPort2 = GetInputPortObj(1);		
			double dInput2;
			pInPutPort2->GetPortUniValue().GetDoubleVal(&dInput2);

			//第一次向缓冲区填充填满
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
					
			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型	
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
			//输出计算
			OutputResultToGlobalWS();			
		}
	}
}
//用于根据参数项和值进行属性的设置，当读取值要用
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
//重载以显示不同的属性对象对话框
void CHistSeriesBuffBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
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
	//保存
	if(dlg.DoModal()==IDOK)
	{
		m_strPointName=dlg.m_strPointName;
		m_iDataSource=dlg.m_iDataSource;
		m_iReadModel=dlg.m_iReadModel;
		m_ISnapStep=dlg.m_ISnapStep;
		m_iBuffTime=dlg.m_iBuffTime;
		m_iOutputTime=dlg.m_iOutputTime;

		//
		CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
		IDArrLeftIn.RemoveAll();

		if(m_iDataSource==1)
		{
			IDArrLeftIn.Add("In1");
			IDArrLeftIn.Add("In2");
		}		
		
		//删除原有而不用的Port,倒着删
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)
		{
			int  runs=0;//遍历次数
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
		//添加新的Port
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)
		{
			int  runs=0;//遍历次数
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
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
}
//重载以将本模块写入到数据库中
void CHistSeriesBuffBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("HistSeriesBuff1"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strPointName) );
	strFieldValue.Format("%d",m_iDataSource);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iReadModel);
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(strFieldValue));
	strFieldValue.Format("%d",m_ISnapStep);
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(strFieldValue));

	//属性5-6：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("HistSeriesBuff2"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	strFieldValue.Format("%d",m_iBuffTime);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iOutputTime);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CHistSeriesBuffBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strPointName=((CHistSeriesBuffBlock*)pBlock)->m_strPointName;
	m_iDataSource=((CHistSeriesBuffBlock*)pBlock)->m_iDataSource;
	m_iReadModel=((CHistSeriesBuffBlock*)pBlock)->m_iReadModel;
	m_ISnapStep=((CHistSeriesBuffBlock*)pBlock)->m_ISnapStep;
	m_iBuffTime=((CHistSeriesBuffBlock*)pBlock)->m_iBuffTime;
	m_iOutputTime=((CHistSeriesBuffBlock*)pBlock)->m_iOutputTime;
}
//将属性放置传输数据中
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
	//将稳态时间段截小 10分钟一段
    Temp_begintime.clear();
	Temp_endtime.clear();
	if(stableend<stablebegin+10*60)//将稳态时间段每10分钟算一次，因为如果时间段过长GetRawDataByTagName不能够把数据全部返回
	{
		Temp_begintime.push_back(stablebegin);
		Temp_endtime.push_back(stableend);
	}
	else if(stableend>=stablebegin+10*60)
	{
		int runs=(stableend-stablebegin)/(10*60);//取整
	    double result=(stableend-stablebegin)%(10*60);//取余
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
