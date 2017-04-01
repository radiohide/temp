// WriteRTDBBlock.cpp: implementation of the CWriteRTDBBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WriteRTDBBlock.h"
#include "PropertyDlgWriteRTDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteRTDBBlock::CWriteRTDBBlock()
{
	m_TagTimeOffsetOrInput=1;//1:采用系统时间+偏移。
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";
	m_iPointType=1;
	m_iPointStatus=1;
	m_iCheckPort=0;

	SetBlockType(_T( "WriteRTDB" ) );
	ConstructBlockInputOutput();
}

CWriteRTDBBlock::~CWriteRTDBBlock()
{

}
CCalcBlock* CWriteRTDBBlock::CreateFromString( const CString& strType )
{
	CWriteRTDBBlock* obj = new CWriteRTDBBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}

void CWriteRTDBBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Value"), VALUE_DOUBLE, _T("in1"), _T("in1"));
}
//返回属性的逗点字符串
CString CWriteRTDBBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRTDBBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//默认该计算块的计算是可组态公式的基本计算
void CWriteRTDBBlock::Calc()
{
	GetInputValueFromGlobalWS();
    //计算过程
	double result=0;
	long timeflag=0;
	double inValue=0;
	if(this->m_pModel->m_bConRTDB)
	{
		double dEnableCalc=1;//若无使能端口则默认为可以计算写入
		double dInputValue=0;
		double dTime=0;
		CString strPortID;
		for(int i=0;i<this->GetInputNum();i++)
		{	
			CCalcPort *pPortObj = GetInputPortObj(i);
			strPortID=pPortObj->GetPortID();
			if(strPortID=="Enable")
			{
				pPortObj->GetPortUniValue().GetDoubleVal(&dEnableCalc);
			}
			else if(strPortID=="Value")
			{
				pPortObj->GetPortUniValue().GetDoubleVal(&dInputValue);
			}
			else if(strPortID=="Offset")
			{
				pPortObj->GetPortUniValue().GetDoubleVal(&dTime);
			}
		}
		if(dEnableCalc<=0)//有使能端口，且为关闭
			return;
		if(m_TagTimeOffsetOrInput==1)//内部时间加偏移
		{
			timeflag=this->m_pModel->m_lDataTime-this->m_lOffset;
		}
		else
		{
			timeflag=(long)dTime;
		}
		if(timeflag<0)
			timeflag=0;
		if(m_strTagName!="")
		{
			bool bFind=false;
			CString strPointName;
			int iFindIndex=0;
			for(int i=0;i<m_pModel->m_iWCounter;i++)
			{
				strPointName.Format("%s",m_pModel->m_pInSertRtData[i].pointName);
				if(strPointName==m_strTagName)
				{
					iFindIndex=i;
					bFind=true;
					break;
				}
			}
			if(bFind)//若有重复则覆盖
			{
				strcpy(m_pModel->m_pInSertRtData[iFindIndex].pointName,(LPSTR)(LPCTSTR)m_strTagName);
				m_pModel->m_pInSertRtData[iFindIndex].status=m_iPointStatus;
				m_pModel->m_pInSertRtData[iFindIndex].time=timeflag;
				m_pModel->m_pInSertRtData[iFindIndex].type=m_iPointType;
				m_pModel->m_pInSertRtData[iFindIndex].value=dInputValue;
				CString str;
				str.Format("模型内部含有重复写入点,点名:%s",m_strTagName);
				if(m_iDebugOn)//调试模式
					AfxMessageBox(str);
				else if(m_iTraceOn)//运行模式,且追踪信息
				{
					if(m_pWnd!=NULL)
						m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
				}
				if(m_iLogOnOrOff)
				{
					if(m_pLog!=NULL)
						m_pLog->WriteFileData(str);
				}
			}	
			else//若无重复则添加
			{
				strcpy(m_pModel->m_pInSertRtData[m_pModel->m_iWCounter].pointName,(LPSTR)(LPCTSTR)m_strTagName);
				m_pModel->m_pInSertRtData[m_pModel->m_iWCounter].status=m_iPointStatus;
				m_pModel->m_pInSertRtData[m_pModel->m_iWCounter].time=timeflag;
				m_pModel->m_pInSertRtData[m_pModel->m_iWCounter].type=m_iPointType;
				m_pModel->m_pInSertRtData[m_pModel->m_iWCounter].value=dInputValue;
				m_pModel->m_iWCounter++;
			}
		}
	}	
}

/////////////////////////////////////////////////////////////////////////
void CWriteRTDBBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
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
	else if(strPropName.Compare("PointType")==0)
	{
		if(!strItem1.IsEmpty())  {m_iPointType=atoi(strItem1);}
	}
	else if(strPropName.Compare("PointStatus")==0)
	{
		if(!strItem1.IsEmpty())  {m_iPointStatus=atoi(strItem1);}
	}
	else if(strPropName.Compare("EnablePort")==0)
	{
		if(!strItem1.IsEmpty())  {m_iCheckPort=atoi(strItem1);}
	}
}
////////////////////////////////////////////////////////////////////////


void CWriteRTDBBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
void CWriteRTDBBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口名称
	CStringArray  IDArrLeftIn;//计算块内部的输入端口名称
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgWriteRTDB dlg;	
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_iPointType=m_iPointType;
	dlg.m_iPointStatus=m_iPointStatus;
	dlg.m_BCheckPort=m_iCheckPort;
	
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_iPointType=dlg.m_iPointType;
		m_iPointStatus=dlg.m_iPointStatus;
		m_iCheckPort=dlg.m_BCheckPort;
		
		if(m_iCheckPort==1)
		{
			IDArrLeftIn.Add("Enable");
		}
		IDArrLeftIn.Add("Value");
		if(m_TagTimeOffsetOrInput==0)//外部
		{
			IDArrLeftIn.Add("Offset");
		}
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j]))	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
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
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}

	}
	//////////////////////////////////////////////////
}
//将属性写到记录的当前位置
void CWriteRTDBBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="PointType";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iPointType); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="PointStatus";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iPointStatus); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="EnablePort";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iCheckPort); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CWriteRTDBBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_TagTimeOffsetOrInput=((CWriteRTDBBlock*)pBlock)->m_TagTimeOffsetOrInput;
	this->m_lOffset=((CWriteRTDBBlock*)pBlock)->m_lOffset;
	this->m_strTagName=((CWriteRTDBBlock*)pBlock)->m_strTagName;
	this->m_strTagDesp=((CWriteRTDBBlock*)pBlock)->m_strTagDesp;
	
	this->m_iPointType=((CWriteRTDBBlock*)pBlock)->m_iPointType;
	this->m_iPointStatus=((CWriteRTDBBlock*)pBlock)->m_iPointStatus;
	this->m_iCheckPort=((CWriteRTDBBlock*)pBlock)->m_iCheckPort;
}
//将属性放置传输数据中
void CWriteRTDBBlock::SetBlockPropertyToTransferData(
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
	strcpy(pData->m_cPropName,"PointType");	
	str.Format("%d",m_iPointType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
	
	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"PointStatus");	
	str.Format("%d",m_iPointStatus);
	strcpy(pData->m_cValueItem1,str);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"EnablePort");	
	str.Format("%d",m_iCheckPort);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
