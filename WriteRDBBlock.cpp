// WriteRDBBlock.cpp: implementation of the CWriteRDBBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WriteRDBBlock.h"
#include "PropertyDlgWriteRDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteRDBBlock::CWriteRDBBlock()
{
	m_TagTimeOffsetOrInput=1;//1:采用系统时间+偏移。
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";
	m_strTableName="point_data";
	m_strDBDesp="";
	m_iPointType=1;
	m_iPointStatus=1;
	
	m_strFieldVarName="var_name";
	m_strFieldDateTime="DateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";

	SetBlockType(_T( "WriteRDB" ) );
	ConstructBlockInputOutput();
}

CWriteRDBBlock::~CWriteRDBBlock()
{

}
CCalcBlock* CWriteRDBBlock::CreateFromString( const CString& strType )
{
	CWriteRDBBlock* obj = new CWriteRDBBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CWriteRDBBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Value"), VALUE_DOUBLE);
}
//返回属性的逗点字符串
CString CWriteRDBBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRDBBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//默认该计算块的计算是可组态公式的基本计算
void CWriteRDBBlock::Calc()
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	GetInputValueFromGlobalWS();
    //计算过程
	double result=0;
	long timeflag=0;
	double inValue=0;
	if(m_TagTimeOffsetOrInput==1)
	{
		timeflag=this->m_pModel->m_lDataTime-this->m_lOffset;
		CCalcPort *pPortObj = GetInputPortObj(0);
		int ret = pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
	}
	else
	{
		CCalcPort *pPortObj2 = GetInputPortObj(0);
		int ret = pPortObj2->GetPortUniValue().GetDoubleVal(&inValue);

		double outOffsetTime;
		CCalcPort *pPortObj1 = GetInputPortObj(1);
		ret = pPortObj1->GetPortUniValue().GetDoubleVal(&outOffsetTime);
		timeflag=(long)outOffsetTime;	
	}
	if(timeflag<0)
		timeflag=0;
	if(m_strTagName!="")
	{
		_variant_t RecordsAffected;
		CString strSQL;
		if(!gIsTableExist(m_pConRDB,this->m_strTableName))
		{
			//strDBType.TrimLeft();strDBType.TrimRight();
			//strDBType.MakeUpper();
			strDBType.MakeUpper();
			if(strDBType=="MYSQL")
			{
				//strSQL.Format("CREATE TABLE `%s` (  `ID` int(11) NOT NULL AUTO_INCREMENT,  `LongID` varchar(50) NOT NULL, 
				//`DateTime` datetime DEFAULT NULL,  `State` int(1) NOT NULL DEFAULT '0',  `Value` double NOT NULL DEFAULT '0', 
				//	PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk",m_strTableName);	
				strSQL.Format("CREATE TABLE `%s` (  `ID` int(11) NOT NULL AUTO_INCREMENT,  `%s` varchar(50) NOT NULL,`%s` datetime DEFAULT NULL,  `%s` int(1) NOT NULL DEFAULT '0',  `%s` double NOT NULL DEFAULT '0',PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk",
					m_strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue);	
			}
			else if(strDBType=="MDB")
			{
				strSQL.Format("Create TABLE %s(ID COUNTER(1, 1) CONSTRAINT PK_TVIPLevel26 PRIMARY KEY,%s varchar(250), %s DATETIME, %s SMALLINT,%s varchar(250)",
					m_strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue);
			}
			m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText); 
		}
		try
		{
			//strDBType.TrimLeft();strDBType.TrimRight();
			//strDBType.MakeUpper();

			CTime t(timeflag);
			CString strTime=t.Format("%Y-%m-%d %H:%M:%S");
			strDBType.MakeUpper();
			if(strDBType=="MYSQL")
			{
				strSQL.Format("insert into %s(%s,%s,%s,%s) values(\'%s\',\'%s\',\'%d\',\'%f\')",
					m_strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue,
					this->m_strTagName,strTime,this->m_iPointStatus,inValue);
			}
			else if(strDBType=="MDB")
			{
				strSQL.Format("insert into [%s]([%s],[%s],[%s],[%s]) values(\'%s\',\'%s\',\'%d\',\'%f\')",
				m_strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue,
				this->m_strTagName,strTime,this->m_iPointStatus,inValue);
			}
			m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //添加记录
		}
		catch(...)
		{
			CString str="写入关系库失败！";
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
	}
}

/////////////////////////////////////////////////////////////////////////
void CWriteRDBBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
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
	else if(strPropName.Compare("TableName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTableName=strItem1;}
	}
	else if(strPropName.Compare("DBDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBDesp=strItem1;}
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
	else if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
	}
}
////////////////////////////////////////////////////////////////////////
//将属性放置传输数据中
void CWriteRDBBlock::SetBlockPropertyToTransferData(
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
	str.Format("%d",m_strTagName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TableName");	
	str.Format("%d",m_strTableName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBDesp");	
	str.Format("%d",m_strTagDesp);
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
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrField");
	str.Format("%s",m_strFieldVarName);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strFieldDateTime);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strFieldState);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_strFieldValue);
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}
//复制图形时，需要拷贝属性。
void CWriteRDBBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_iPointStatus=((CWriteRDBBlock*)pBlock)->m_iPointStatus;
	this->m_iPointType=((CWriteRDBBlock*)pBlock)->m_iPointType;
	this->m_strTagDesp=((CWriteRDBBlock*)pBlock)->m_strTagDesp;
	this->m_strDBDesp=((CWriteRDBBlock*)pBlock)->m_strDBDesp;

	this->m_strTableName=((CWriteRDBBlock*)pBlock)->m_strTableName;
	this->m_strTagName=((CWriteRDBBlock*)pBlock)->m_strTagName;
	this->m_lOffset=((CWriteRDBBlock*)pBlock)->m_lOffset;
	this->m_TagTimeOffsetOrInput=((CWriteRDBBlock*)pBlock)->m_TagTimeOffsetOrInput;

	this->m_strFieldVarName=((CWriteRDBBlock*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CWriteRDBBlock*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CWriteRDBBlock*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CWriteRDBBlock*)pBlock)->m_strFieldValue;
}
void CWriteRDBBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
void CWriteRDBBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgWriteRDB dlg;
	dlg.m_TagTimeOffsetOrInput = m_TagTimeOffsetOrInput;
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBDesp=m_strDBDesp;
	dlg.m_iPointType=m_iPointType;
	dlg.m_iPointStatus=m_iPointStatus;
	dlg.m_strFieldVarName=m_strFieldVarName;
	dlg.m_strFieldDateTime=m_strFieldDateTime;
	dlg.m_strFieldState=m_strFieldState;
	dlg.m_strFieldValue=m_strFieldValue;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	if(dlg.DoModal()==IDOK)
	{
		m_TagTimeOffsetOrInput = dlg.m_TagTimeOffsetOrInput;
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_strTableName=dlg.m_strTableName;
		m_strDBDesp=dlg.m_strDBDesp;
		m_iPointType=dlg.m_iPointType;
		m_iPointStatus=dlg.m_iPointStatus;

		m_strFieldVarName=dlg.m_strFieldVarName;
		m_strFieldDateTime=dlg.m_strFieldDateTime;
		m_strFieldState=dlg.m_strFieldState;
		m_strFieldValue=dlg.m_strFieldValue;

		if(IDArrLeftOut.GetSize()==1)
		{
			if(m_TagTimeOffsetOrInput==0)//外部
			{
				AddInputPort(_T("Offset"), VALUE_DOUBLE);
			}
			else{}//内部
		}
		if(IDArrLeftOut.GetSize()==2)
		{
			if(m_TagTimeOffsetOrInput==0)//外部
			{}
			else//内部
			{
				this->ClearInputPortByIndex(1);
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
void CWriteRDBBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="TableName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTableName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="DBDesp";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBDesp); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="StrField";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strFieldVarName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldDateTime); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldState); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldValue); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}
BOOL CWriteRDBBlock::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s limit 0,1",strTableName);
	try
	{
		pRecord->Open((const char*)strSQL,pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	} 
	return TRUE;
}
