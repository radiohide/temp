// RTDBHistSeriesBat.cpp: implementation of the CRTDBHistSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTDBHistSeriesBat.h"
#include "PropertyDlgRTDBHistSeriesBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTDBHistSeriesBat::CRTDBHistSeriesBat()
{
	dLimitValue=10000000000000000000;
	iRawOrSnapshot=1;
	iSnapshotSpan=60;
	SetBlockType(_T( "RTDBHistSeriesBat" ) );
	ConstructBlockInputOutput();
}

CRTDBHistSeriesBat::~CRTDBHistSeriesBat()
{

}
//根据类型新建对象的函数
CCalcBlock* CRTDBHistSeriesBat::CreateFromString( const CString& strType )
{
	CRTDBHistSeriesBat* obj = new CRTDBHistSeriesBat;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRTDBHistSeriesBat::ConstructBlockInputOutput ()
{
	AddInputPort("Enable",VALUE_DOUBLE);
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CRTDBHistSeriesBat::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//返回属性的逗点字符串
CString CRTDBHistSeriesBat::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTDBHistSeriesBat::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRTDBHistSeriesBat::Calc()//进行计算的函数,需要重载
{
	if(!this->m_pModel->m_bConRTDB)//如果未连接则不进行计算。
		return;
	GetInputValueFromGlobalWS();
	
	double dInValue=0;
	long lSTime=0;
	long lETime=0;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
	lSTime=(long)dInValue;
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
	lETime=(long)dInValue;
	if(lSTime>lETime)
	{
		long lTemp=lSTime;
		lSTime=lETime;
		lETime=lTemp;
	}
	//CTime st(lSTime);
	//CTime et(lETime);
	//CString strSTime = st.Format("%Y-%m-%d %H:%M:%S");
	//CString strETime = et.Format("%Y-%m-%d %H:%M:%S");

	for(int i=0;i<strVarArr.GetSize();i++)
	{
		CString strVarName=strVarArr[i];
		std::vector<TagData> tagDataArr;
		readDataFromRTDB(strVarName,lSTime,lETime,iRawOrSnapshot,iSnapshotSpan,tagDataArr);
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		CUniValue UniVar;//用于输出
		UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
		for(int j=0;j<tagDataArr.size();j++)
		{
			UniVar.AddData(tagDataArr[j].value,tagDataArr[j].time,tagDataArr[j].status);
		}
		pPortObjOut->SetPortUniValue(UniVar);
	}
	OutputResultToGlobalWS();
}
void CRTDBHistSeriesBat::readDataFromRTDB(
									 CString strName,
									 long lSTime,
									 long lETime,
									 int iReqType,//0:原始;1:快照;
									 int isnapshotT,
									 std::vector<TagData> &tagDataArr)
{
	tagDataArr.clear();
	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	typedef int (*pRTDBFreePointer)(void *p);
	pRTDBFreePointer RTDBFreePointer=(pRTDBFreePointer)GetProcAddress(this->m_pModel->hDLL,"RTDBFreePointer");
	
	ReadHiDataRequest req;
	req.stTime=lSTime;
	req.enTime=lETime;
	strcpy(req.pointName,strName);
	TagData tagTemp;
	long nCount = 0;
	if(iReqType==1)//快照
	{
		req.tPeriod=isnapshotT;
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
		for(int i=0;i<nCount;i++)
		{
			if((tagDatas[i].value<dLimitValue)&&(tagDatas[i].value>-1*dLimitValue)&&((tagDatas[i].status==0)||(tagDatas[i].status==2)))//状态为正确
			{
				tagTemp.value=tagDatas[i].value;
				tagTemp.time=tagDatas[i].time;
				tagTemp.status=tagDatas[i].status;
				tagDataArr.push_back(tagTemp);
			}
		}
		if(tagDatas!=NULL)
		{
			delete[] tagDatas;
			tagDatas=NULL;
		}
	}
	else if(iReqType==0)//原始
	{
		req.tPeriod=0;
		req.reqType=0;
		TagData *tagDatas=NULL;
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
					if((tagDatas[j].value<dLimitValue)&&(tagDatas[j].value>-1*dLimitValue)&&((tagDatas[j].status==0)||(tagDatas[j].status==2)))//状态为正确
					{
						tagTemp.value=tagDatas[j].value;
						tagTemp.time=tagDatas[j].time;
						tagTemp.status=tagDatas[j].status;
						tagDataArr.push_back(tagTemp);
					}
				}
				if(nCount>0)
				{
					req.stTime=tagTemp.time+1;	//修改查询开始时间,比最后一个点要偏移1秒，防止重复读
				}
			}
			RTDBFreePointer(tagDatas);
		}while ((nCount==1024) && (req.stTime !=req.enTime));
	}
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRTDBHistSeriesBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("readfun")==0)
	{
		if(!strItem1.IsEmpty())  {iRawOrSnapshot=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {iSnapshotSpan=atoi(strItem2);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==1)
		{
			if(!strItem1.IsEmpty())  {strVarArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strPortArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CRTDBHistSeriesBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="formula")//公式为3
	{
		propType=3;
	}
}
//重载以显示不同的属性对象对话框
void CRTDBHistSeriesBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRTDBHistSeriesBat dlg;

	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strVarArr.Copy(strVarArr);
	dlg.strPortArr.Copy(strPortArr);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.strUnitArr.Copy(strUnitArr);
	dlg.iRawOrSnapshot=iRawOrSnapshot;
	dlg.iSnapshotSpan=iSnapshotSpan;
	if(dlg.DoModal()==IDOK)
	{
		strVarArr.Copy(dlg.strVarArr);
		strPortArr.Copy(dlg.strPortArr);
		strTagDespArr.Copy(dlg.strTagDespArr);
		strUnitArr.Copy(dlg.strUnitArr);
		iRawOrSnapshot=dlg.iRawOrSnapshot;
		iSnapshotSpan=dlg.iSnapshotSpan;
		
		//对输出端口的修改
		for(int i=0;i<strPortArr.GetSize();i++)
		{
			IDArrRightIn.Add(strPortArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj]))	
				{runs++;}
			}
			if(runs==IDArrRightIn.GetSize())
			{
				this->ClearOutputPortByIndex(jj,parent);
			}
		}		
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
		{
			int  runs=0;//遍历次数
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_RTVALUE_SERIES, _T(str));
			}
		}
		parent->RedrawWindow();
	}

}
//重载以将本模块写入到数据库中
void CRTDBHistSeriesBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strPortArr.GetSize();i++)//输出
	{
		CString strInPut=strPortArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",strVarArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strPortArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strUnitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("readfun",i);
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	
	strFieldValue.Format("%d",iRawOrSnapshot); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",iSnapshotSpan); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CRTDBHistSeriesBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strVarArr.Copy(((CRTDBHistSeriesBat*)pBlock)->strVarArr);
	this->strPortArr.Copy(((CRTDBHistSeriesBat*)pBlock)->strPortArr);
	this->strTagDespArr.Copy(((CRTDBHistSeriesBat*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRTDBHistSeriesBat*)pBlock)->strUnitArr);

	this->iRawOrSnapshot=((CRTDBHistSeriesBat*)pBlock)->iRawOrSnapshot;//0:原始；1:快照
	this->iSnapshotSpan=((CRTDBHistSeriesBat*)pBlock)->iSnapshotSpan;//快照间隔
}
//将属性放置传输数据中
void CRTDBHistSeriesBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
