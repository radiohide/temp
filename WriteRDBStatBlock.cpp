// WriteRDBStatBlock.cpp: implementation of the CWriteRDBStatBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WriteRDBStatBlock.h"
#include "PropertyDlgWRDBStat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteRDBStatBlock::CWriteRDBStatBlock()
{
	m_BCount=FALSE;
	m_BMaxValue=FALSE;
	m_BMinValue=FALSE;
	m_BStat=FALSE;

	m_iTagTimeIntM=1;//默认取整分
	m_iTimeLengthIntM=2;//默认整分左右两秒取整分

	SetBlockType(_T( "WriteRDBStat" ) );
	ConstructBlockInputOutput();
}

CWriteRDBStatBlock::~CWriteRDBStatBlock()
{

}
CCalcBlock* CWriteRDBStatBlock::CreateFromString( const CString& strType )
{
	CWriteRDBStatBlock* obj = new CWriteRDBStatBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CWriteRDBStatBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("STime"), VALUE_DOUBLE);
	AddInputPort(_T("ETime"), VALUE_DOUBLE);
	AddInputPort(_T("Value"), VALUE_DOUBLE);

	//AddInputPort(_T("State"), VALUE_DOUBLE);
	//AddInputPort(_T("MaxValue"), VALUE_DOUBLE);
	//AddInputPort(_T("MinValue"), VALUE_DOUBLE);
	//AddInputPort(_T("Count"), VALUE_DOUBLE);
}
void CWriteRDBStatBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CWriteRDBStatBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strVarName,m_strVarDesp);
	return str;
}
void CWriteRDBStatBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_strVarName=str.Left(str.Find('@'));
	m_strVarDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//默认该计算块的计算是可组态公式的基本计算
void CWriteRDBStatBlock::Calc()
{
	if(!this->m_pModel->m_bConRTDB)//如果未连接则不进行计算。
		return;
	
	GetInputValueFromGlobalWS();
    //计算过程
	double inValue1,inValue2,inValue3;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
	CCalcPort *pInPortObj3 = GetInputPortObj(2);
	pInPortObj3->GetPortUniValue().GetDoubleVal(&inValue3);
	long lSTime=(long)inValue1;
	long lETime=(long)inValue2;
	double dTotal=inValue3;

	CTime eTime(lETime);
		
	//计算数据
	double dTemp=0;
	int iState=0;
	if(m_BStat)
	{
		CCalcPort *pInPortObj = GetInputPortObj(3);
		pInPortObj->GetPortUniValue().GetDoubleVal(&dTemp);
		iState=(int)dTemp;
	}
	double dMax=0;
	if(m_BMaxValue)
	{
		CCalcPort *pInPortObj = GetInputPortObj(3+m_BStat);
		pInPortObj->GetPortUniValue().GetDoubleVal(&dMax);
	}
	double dMin=0;
	if(m_BMinValue)
	{
		CCalcPort *pInPortObj = GetInputPortObj(3+m_BStat+m_BMaxValue);
		pInPortObj->GetPortUniValue().GetDoubleVal(&dMin);
	}
	int iCount=0;
	if(m_BCount)
	{
		CCalcPort *pInPortObj = GetInputPortObj(3+m_BStat+m_BMaxValue+m_BMinValue);
		pInPortObj->GetPortUniValue().GetDoubleVal(&dTemp);
		iCount=(int)dTemp;
	}
	//写数据
	CString strTableName;
	strTableName.Format("%s_%d",m_strVarName,eTime.GetYear());
	if(!checkTableExist(strTableName))
	{
		createTable(strTableName);
	}
	if(m_iTagTimeIntM==1)//对开始结束时间进行整分处理
	{
		getTimeIntM(lSTime,lETime);
	}
	writeDataToTable(strTableName,lSTime,lETime,dMax,dMin,iState,dTotal,iCount);
}
void CWriteRDBStatBlock::createTable(CString strTable)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	_variant_t RecordsAffected;
	CString strSQL;
	strSQL.Format("create table %s (id bigint not null auto_increment, start_date datetime,end_date datetime,in_date datetime, max_value decimal(18,6), min_value decimal(18,6), state_type tinyint, total_value decimal(18,6),count int, primary key (id));",strTable);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}

bool CWriteRDBStatBlock::checkTableExist(CString strTable)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return false;
	_RecordsetPtr m_pRec;
	HRESULT hr;
	hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
	CString strSql;
	strSql.Format("SHOW TABLES LIKE '%s';",strTable);
	m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	if(!m_pRec->adoEOF)
	{
		return true;
	}
	return false;
}
void CWriteRDBStatBlock::getTimeIntM(long &lSTime,long &lETime)
{
	CTime tS(lSTime);
	if(tS.GetSecond()!=0)
	{
		if(tS.GetSecond()>=(60-m_iTimeLengthIntM))
		{
			CTime nextMinu=tS+CTimeSpan(0,0,1,0);
			CTime newTime(nextMinu.GetYear(),nextMinu.GetMonth(),nextMinu.GetDay(),nextMinu.GetHour(),nextMinu.GetMinute(),0);
			lSTime=newTime.GetTime();
		}
		if(tS.GetSecond()<=m_iTimeLengthIntM)
		{
			CTime newTime(tS.GetYear(),tS.GetMonth(),tS.GetDay(),tS.GetHour(),tS.GetMinute(),0);
			lSTime=newTime.GetTime();
		}
	}
	CTime tE(lETime);
	if(tE.GetSecond()!=0)
	{
		if(tE.GetSecond()>=(60-m_iTimeLengthIntM))
		{
			CTime nextMinu=tE+CTimeSpan(0,0,1,0);
			CTime newTime(nextMinu.GetYear(),nextMinu.GetMonth(),nextMinu.GetDay(),nextMinu.GetHour(),nextMinu.GetMinute(),0);
			lSTime=newTime.GetTime();
		}
		if(tE.GetSecond()<=m_iTimeLengthIntM)
		{
			CTime newTime(tE.GetYear(),tE.GetMonth(),tE.GetDay(),tE.GetHour(),tE.GetMinute(),0);
			lETime=newTime.GetTime();
		}
	}
}
void CWriteRDBStatBlock::writeDataToTable(
									 CString strTable,
									 long lSTime,
									 long lETime,
									 double dMax,
									 double dMin,
									 int  iStatType,
									 double dTotalValue,
									 long iCount)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	CString strSQL;
	_variant_t RecordsAffected;

	CTime sTime(lSTime);
	CTime eTime(lETime);
	CString strSTime,strETime,strCurTime;
	strSTime=sTime.Format("%Y-%m-%d %H:%M:%S");
	strETime=eTime.Format("%Y-%m-%d %H:%M:%S");
	strCurTime=CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	getInRangeValue(dTotalValue);
	strSQL.Format("insert into %s(start_date,end_date,in_date,max_value,min_value,state_type,total_value,count)	values(\'%s\',\'%s\',\'%s\',\'%f\',\'%f\',\'%d\',\'%f\',\'%d\')",
		strTable,strSTime,strETime,strCurTime,
		dMax,dMin,
		iStatType,dTotalValue,iCount);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CWriteRDBStatBlock::getInRangeValue(double &dValue)
{
	if(dValue>99999999999.999999)
	{
		//dValue=99999999999.999999;
		dValue=0.0123456789;
	}
	else if(dValue<-99999999999.999999)
	{
		//dValue=-99999999999.999999;
		dValue=-0.0123456789;
	}
}
void CWriteRDBStatBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgWRDBStat dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_BCount=m_BCount;
	dlg.m_BMaxValue=m_BMaxValue;
	dlg.m_BMinValue=m_BMinValue;
	dlg.m_BStat=m_BStat;
	dlg.m_strVarDesp=m_strVarDesp;
	dlg.m_strVarName=m_strVarName;
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_BCount=dlg.m_BCount;
		m_BMaxValue=dlg.m_BMaxValue;
		m_BMinValue=dlg.m_BMinValue;
		m_BStat=dlg.m_BStat;
		m_strVarDesp=dlg.m_strVarDesp;
		m_strVarName=dlg.m_strVarName;
			
		int iFindIndex=-1;
		int iInputNum=this->GetInputNum();
		for(int i=0;i<iInputNum;i++)
		{
			CString strPortID=this->GetInputPortObj(i)->GetPortID();
			if("State"==strPortID)
			{
				iFindIndex=i;
			}
		}
		if(iFindIndex!=-1)//有该端口
		{
			if(!m_BStat)
				this->ClearInputPortByIndex(iFindIndex);
		}
		else
		{
			if(m_BStat)
				InSertInputPortBefore(3,"State", VALUE_DOUBLE, "State");
		}

		iFindIndex=-1;
		iInputNum=this->GetInputNum();
		for(i=0;i<iInputNum;i++)
		{
			CString strPortID=this->GetInputPortObj(i)->GetPortID();
			if("MaxValue"==strPortID)
			{
				iFindIndex=i;
			}
		}
		if(iFindIndex!=-1)//有该端口
		{
			if(!m_BMaxValue)
				this->ClearInputPortByIndex(iFindIndex);
		}
		else
		{
			if(m_BMaxValue)
				InSertInputPortBefore(3+m_BStat,"MaxValue", VALUE_DOUBLE, "MaxValue");
		}

		iFindIndex=-1;
		iInputNum=this->GetInputNum();
		for(i=0;i<iInputNum;i++)
		{
			CString strPortID=this->GetInputPortObj(i)->GetPortID();
			if("MinValue"==strPortID)
			{
				iFindIndex=i;
			}
		}
		if(iFindIndex!=-1)//有该端口
		{
			if(!m_BMinValue)
				this->ClearInputPortByIndex(iFindIndex);
		}
		else
		{
			if(m_BMinValue)
				InSertInputPortBefore(3+m_BStat+m_BMaxValue,"MinValue", VALUE_DOUBLE, "MinValue");
		}

		iFindIndex=-1;
		iInputNum=this->GetInputNum();
		for(i=0;i<iInputNum;i++)
		{
			CString strPortID=this->GetInputPortObj(i)->GetPortID();
			if("Count"==strPortID)
			{
				iFindIndex=i;
			}
		}
		if(iFindIndex!=-1)//有该端口
		{
			if(!m_BCount)
				this->ClearInputPortByIndex(iFindIndex);
		}
		else
		{
			if(m_BCount)
				InSertInputPortBefore(3+m_BStat+m_BMaxValue+m_BMinValue,"Count", VALUE_DOUBLE, "Count");
		}

		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
}
//将属性写到记录的当前位置
void CWriteRDBStatBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="DBName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="VarInf";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strVarName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strVarDesp); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="InputPort";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_BStat); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_BMaxValue); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_BMinValue); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_BCount); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
}
//复制图形时，需要拷贝属性。
void CWriteRDBStatBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strDBName=((CWriteRDBStatBlock*)pBlock)->m_strDBName;//数据库名称
	m_BStat=((CWriteRDBStatBlock*)pBlock)->m_BStat;
	m_BMaxValue=((CWriteRDBStatBlock*)pBlock)->m_BMaxValue;
	m_BMinValue=((CWriteRDBStatBlock*)pBlock)->m_BMinValue;
	m_BCount=((CWriteRDBStatBlock*)pBlock)->m_BCount;//是否选择输出个数	
	m_strVarDesp=((CWriteRDBStatBlock*)pBlock)->m_strVarDesp;
	m_strVarName=((CWriteRDBStatBlock*)pBlock)->m_strVarName;
}
////////////////////////////////////////////////////////////////////////
//将属性放置传输数据中
void CWriteRDBStatBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBName");
	
	CString str;
	str.Format("%s",m_strDBName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
	
	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"VarInf");
	
	str.Format("%s",m_strVarName);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strVarDesp);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"InputPort");
	
	str.Format("%d",m_BStat);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_BMaxValue);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_BMinValue);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_BCount);
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}
/////////////////////////////////////////////////////////////////////////
void CWriteRDBStatBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
	}
	else if(strPropName.Compare("VarInf")==0)//整分钟取整
	{
		if(!strItem1.IsEmpty())  {m_strVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strVarDesp=strItem2;}
	}
	else if(strPropName.Compare("InputPort")==0)
	{
		if(!strItem1.IsEmpty())  {m_BStat=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_BMaxValue=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_BMinValue=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_BCount=atoi(strItem4);}
	}
}
