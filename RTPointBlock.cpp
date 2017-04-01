// RTPointBlock.cpp: implementation of the CRTPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTPointBlock.h"
#include "PropertyDlgRTPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPointBlock::CRTPointBlock()
{
	m_TagTimeOffsetOrInput=1;//1:采用系统时间+偏移。
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";

	m_iQueryType=0;
	m_lTimeLength=300;
	m_TagQueryTimeSpan=0;
	SetBlockType(_T( "RTPoint" ) );
	ConstructBlockInputOutput();
}

CRTPointBlock::~CRTPointBlock()
{
	
}
//根据类型新建对象的函数
CCalcBlock* CRTPointBlock::CreateFromString( const CString& strType )
{
	CRTPointBlock* obj = new CRTPointBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRTPointBlock::ConstructBlockInputOutput ()
{
	AddOutputPort("Out",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CRTPointBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//返回属性的逗点字符串
CString CRTPointBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTPointBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//计算函数，实现本模块的计算
void CRTPointBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double result=0;
	if(this->m_pModel->m_bConRTDB)
	{
		typedef  int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
		pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");
		
		typedef int (*pGetAggregationDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * pTagData);
		pGetAggregationDataByTagName GetAggregationDataByTagName=(pGetAggregationDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetAggregationDataByTagName");
		long timeflag=0;
		if(m_TagTimeOffsetOrInput==1)	
		{
			timeflag=this->m_pModel->m_lDataTime-m_lOffset;
		}
		else
		{
			double inValue=0;
			CCalcPort *pPortObj = GetInputPortObj(0);
			pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
			timeflag=(long)inValue;
		}
		if(timeflag<0)
			timeflag=0;
		if(m_strTagName!="")
		{
			int nRet=-1;
			if(m_TagQueryTimeSpan==0)
			{
				TagData *tagData=new TagData;
				memset(tagData,0,sizeof(TagData));
				nRet = m_GetHistoryDataByTime(m_strTagName,timeflag, tagData);
				if(nRet!=0)
				{
					for(int i=0;i<50;i++)//若读取失败，则尝试50次读取。
					{
						memset(tagData,0,sizeof(TagData));
						::Sleep(5);//有时候确实会读取失败，在第二次读取的时候会成功
						nRet = m_GetHistoryDataByTime(m_strTagName,timeflag, tagData);
						if(nRet==0)
							break;
					}
					if(nRet!=0)
					{
						tagData->status=0;
						tagData->time=0;
						tagData->value=0;
					}
				}
				result=tagData->value;
				if(tagData!=NULL)
				{
					delete tagData;
					tagData=NULL;
				}
			}
			else
			{
				TagData *pTagData=new TagData[1];
				memset(pTagData,0,sizeof(TagData)*1);
				ReadHiDataRequest req;
				memset(&req,0,sizeof(ReadHiDataRequest));
				strcpy(req.pointName,m_strTagName);
				req.stTime= timeflag-m_lTimeLength;
				req.enTime= timeflag;
				if(req.enTime<0)
				{
					req.enTime= 0;
				}
				if(m_iQueryType==0)//均值
				{
					req.reqType=3;
				}
				else if(m_iQueryType==1)
				{
					req.reqType=1;
				}
				else if(m_iQueryType==2)
				{
					req.reqType=2;
				}
				req.tPeriod=m_lTimeLength;
				nRet = GetAggregationDataByTagName(&req,pTagData);
				if(nRet==0)
				{
					result=pTagData[0].value;
				}
				else
				{
					for(int i=0;i<50;i++)
					{
						memset(pTagData,0,sizeof(TagData)*1);
						::Sleep(5);
						nRet = GetAggregationDataByTagName(&req,pTagData);
						if(nRet==0)
						{
							result=pTagData[0].value;
							break;
						}
					}
				}
				if(pTagData!=NULL)
				{
					delete[] pTagData;//debug 泄露
					pTagData=NULL;//debug 泄露
				}
			}
		}
	}
	CUniValue UniVar;
	UniVar.AddData(result);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
void CRTPointBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//dc->Rectangle(rect);
	CPoint roundness;
	////////////////////////////////////////////////
	CRect rc(rect);
	rc.top = rect.top + 2*CutSize;
	rc.bottom = rect.bottom - 2*CutSize; 
    roundness.x = int(double(rc.Width())*0.6);
	roundness.y = int(double(rc.Height())*0.6);
	dc->RoundRect(rc,roundness);
 
	//画中线
	int MidX = rc.left+int(double(rc.Width())/2);
	//dc->MoveTo(MidX, rc.top);
	//dc->LineTo(MidX,rc.bottom);
	//画中间的园表示实时
	CRect rcCenter;
	int MidY = rc.top+int(double(rc.Height())/2);
	int Dis =  int(double(rc.Height())*0.25);
	rcCenter.SetRect(MidX-Dis,MidY-Dis,MidX+Dis,MidY+Dis);
    dc->Ellipse(rcCenter);
	///////////////////////////////////////////////
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRTPointBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("TimeOffsetOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagTimeOffsetOrInput=atoi(strItem1);}
	}
	else if(strPropName.Compare("lOffset")==0)
	{
		if(!strItem1.IsEmpty())  {m_lOffset=atol(strItem1);}
	}
	else if(strPropName.Compare("TagName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagName=strItem1;}
	}
	else if(strPropName.Compare("TagDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagDesp=strItem1;}
	}
	else if(strPropName.Compare("QueryPar")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagQueryTimeSpan=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_lTimeLength=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iQueryType=atoi(strItem3);}
	}
}
//重载以显示不同的属性对象对话框
void CRTPointBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgRTPoint dlg;
    //给对话框变量赋值
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;

	dlg.m_lTimeLength=m_lTimeLength;
	dlg.m_TagQueryTimeSpan=m_TagQueryTimeSpan;
	dlg.m_iQueryType=m_iQueryType;
	if(dlg.DoModal()==IDOK)
	{
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_lTimeLength=dlg.m_lTimeLength;
		m_TagQueryTimeSpan=dlg.m_TagQueryTimeSpan;
		m_iQueryType=dlg.m_iQueryType;

		if(m_TagTimeOffsetOrInput==0)//外部
		{
			if(IDArrLeftOut.GetSize()==1){}
			else
			{
				AddInputPort(_T("Offset"), VALUE_DOUBLE, _T("in1"), _T("in1"));
			}
		}
		else
			ClearInputPorts();
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
void CRTPointBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TimeOffsetOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_TagTimeOffsetOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="lOffset";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_lOffset); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TagName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTagName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TagDesp";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTagDesp); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="QueryPar";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_TagQueryTimeSpan); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_lTimeLength); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iQueryType); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CRTPointBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_TagTimeOffsetOrInput=((CRTPointBlock*)pBlock)->m_TagTimeOffsetOrInput;
	this->m_lOffset=((CRTPointBlock*)pBlock)->m_lOffset;
	this->m_strTagName=((CRTPointBlock*)pBlock)->m_strTagName;
	this->m_strTagDesp=((CRTPointBlock*)pBlock)->m_strTagDesp;
	this->m_TagQueryTimeSpan=((CRTPointBlock*)pBlock)->m_TagQueryTimeSpan;
	this->m_lTimeLength=((CRTPointBlock*)pBlock)->m_lTimeLength;
	this->m_iQueryType=((CRTPointBlock*)pBlock)->m_iQueryType;
}
//将属性放置传输数据中
void CRTPointBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffsetOrInput");	
	CString str;
	str.Format("%d",m_TagTimeOffsetOrInput);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"lOffset");	
	str.Format("%d",m_lOffset);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagName");	
	str.Format("%s",m_strTagName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagDesp");	
	str.Format("%s",m_strTagDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"QueryPar");	
	str.Format("%d",m_TagQueryTimeSpan);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_lTimeLength);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iQueryType);
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}
