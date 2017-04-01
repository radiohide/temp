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
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "HistSeries" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===
}

CHistSeriesBlock::~CHistSeriesBlock()
{

}
//构造初始的输入输出个数
void CHistSeriesBlock::ConstructBlockInputOutput()
{
	AddOutputPort("Out1",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
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
//初始化计算的函数做特殊的初始化
void CHistSeriesBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//计算函数，实现本模块的计算
void CHistSeriesBlock::Calc() 
{
	if(!this->m_pModel->m_bConRTDB)//如果未连接则不进行计算。
		return;

	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	typedef int (*pRTDBFreePointer)(void *p);
	pRTDBFreePointer RTDBFreePointer=(pRTDBFreePointer)GetProcAddress(this->m_pModel->hDLL,"RTDBFreePointer");
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	
	if(m_iDataSource==0)//采用内部时间
	{
        if(m_iReadModel==0)//原始值
		{
			ReadHiDataRequest req;
			std::vector<TagData> m_TagData;//盛放用于积分的每一个变量
			m_TagData.clear();
			
			TagData tagTemp;
			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//获取两个个输入
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
						req.stTime=tagTemp.time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
					}
				}
				RTDBFreePointer(tagDatas);
			}while ((nCount==1024) && (req.stTime !=req.enTime));

			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			for(int m=0; m<m_TagData.size(); m++){UniVar.AddData(m_TagData[m].value,m_TagData[m].time,m_TagData[m].status);}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//输出计算
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//快照值
		{
			ReadHiDataRequest req;			
			long nCount = 0;

			//获取两个个输入
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

			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			for(int m=0; m<nCount; m++)
			{
				UniVar.AddData(tagDatas[m].value,tagDatas[m].time,tagDatas[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//输出计算
			OutputResultToGlobalWS();
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
		}
	}
	else if(m_iDataSource==1)//外部输入时间
	{		
		if(m_iReadModel==0)//原始值
		{
			ReadHiDataRequest req;
			std::vector<TagData> m_TagData;//盛放用于积分的每一个变量
			m_TagData.clear();
			
			TagData tagTemp;
			
			TagData *tagDatas=NULL;
			long nCount = 0;
			//获取连个输入
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
						req.stTime=tagTemp.time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
					}
				}
				RTDBFreePointer(tagDatas);
			}while ((nCount==1024) && (req.stTime !=req.enTime));

			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			for(int m=0; m<m_TagData.size(); m++)
			{
				UniVar.AddData(m_TagData[m].value,m_TagData[m].time,m_TagData[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//输出计算
			OutputResultToGlobalWS();
		}
		else if(m_iReadModel==1)//快照值
		{
			ReadHiDataRequest req;			
			long nCount = 0;

			//获取连个输入
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

			CUniValue UniVar;//用于输出
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			for(int m=0; m<nCount; m++)
			{
				UniVar.AddData(tagDatas[m].value,tagDatas[m].time,tagDatas[m].status);
			}
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			pPortObjOut->SetPortUniValue(UniVar);
			//输出计算
			OutputResultToGlobalWS();
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
		}
	}	
}
//用于根据参数项和值进行属性的设置，当读取值要用
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
//重载以显示不同的属性对象对话框
void CHistSeriesBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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
	CPropertyDlgHistSeries  dlg;

	dlg.m_strPointName=m_strPointName;
	dlg.m_iDataSource=m_iDataSource ;
	dlg.m_iReadModel=m_iReadModel ;
	dlg.m_ISnapStep=m_ISnapStep;
	//保存
	if(dlg.DoModal()==IDOK)
	{
		m_strPointName=dlg.m_strPointName;
		m_iDataSource=dlg.m_iDataSource;
		m_iReadModel=dlg.m_iReadModel;
		m_ISnapStep=dlg.m_ISnapStep;	
		
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
void CHistSeriesBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("HistSeries"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strPointName) );
	strFieldValue.Format("%d",m_iDataSource);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iReadModel);
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(strFieldValue));
	strFieldValue.Format("%d",m_ISnapStep);
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(strFieldValue));

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CHistSeriesBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strPointName=((CHistSeriesBlock*)pBlock)->m_strPointName;
	m_iDataSource=((CHistSeriesBlock*)pBlock)->m_iDataSource;
	m_iReadModel=((CHistSeriesBlock*)pBlock)->m_iReadModel;
	m_ISnapStep=((CHistSeriesBlock*)pBlock)->m_ISnapStep;

}
//将属性放置传输数据中
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

