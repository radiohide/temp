// StatEnumBlock.cpp: implementation of the CStatEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatEnumBlock.h"
#include "PropertyDlgStatEnum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatEnumBlock::CStatEnumBlock()
{
	m_iReqType=0;//Ĭ��ȡ����
	m_lSnapshotT=300;//Ĭ�Ͽ���ȡ300��
	m_iStatType=0;//Ĭ��ͳ�ƴ���
	dLimitValue=10000000000000000000;//1E19;
	SetBlockType(_T( "StatEnum" ) );
	ConstructBlockInputOutput();
	m_iTagTimeIntM=1;//Ĭ��ȡ����
	m_iTimeLengthIntM=2;//Ĭ��������������ȡ����
}

CStatEnumBlock::~CStatEnumBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatEnumBlock::CreateFromString( const CString& strType )
{
	CStatEnumBlock* obj = new CStatEnumBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatEnumBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("STime"), VALUE_DOUBLE);
	AddInputPort(_T("ETime"), VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatEnumBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStatEnumBlock::Calc()//���м���ĺ���,��Ҫ����
{
	if(!this->m_pModel->m_bConRTDB)//���δ�����򲻽��м��㡣
		return;

	GetInputValueFromGlobalWS();
    //�������
	double inValue1,inValue2;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
	long lSTime=(long)inValue1;
	long lETime=(long)inValue2;

	CTime sTime(lSTime);
	
	std::vector<TagData> tagDataArr;
	
	if(m_strVarName=="")
	{
		return;
	}
	CString strTableName;
	strTableName.Format("%s_%d",m_strOutName,sTime.GetYear());
	//������
	readDataFromRTDB(m_strVarName,lSTime,lETime,m_iReqType,m_lSnapshotT,tagDataArr);
	//��������
	if(m_iStatType==0)//ͳ�Ƴ��ִ���
	{
		for(int i=0;i<m_dSValueArr.GetSize();i++)
		{
			int iStatType=m_iSETag[i];
			int iCounter=0;
			for(int j=0;j<(long)tagDataArr.size();j++)
			{
				if((m_dSValueArr[i]<tagDataArr[j].value)&&(tagDataArr[j].value<=m_dEValueArr[i]))
				{
					iCounter++;
				}
			}
			//д����
			if(iCounter!=0)//
			{
				if(!checkTableExist(strTableName))
				{
					createTable(strTableName);
				}
				if(m_iTagTimeIntM==1)//�Կ�ʼ����ʱ��������ִ���
				{
					getTimeIntM(lSTime,lETime);
				}
				writeDataToTable(strTableName,lSTime,lETime,iStatType,iCounter,iCounter);
			}
		}
	}
	else if(m_iStatType==1)//ͳ��ʱ��
	{
		for(int i=0;i<m_dSValueArr.GetSize();i++)
		{
			int iStatType=m_iSETag[i];
			int iCounter=0;
			int iLenght=0;
			for(int j=0;j<(long)tagDataArr.size();j++)
			{
				if((m_dSValueArr[i]<tagDataArr[j].value)&&(tagDataArr[j].value<=m_dEValueArr[i]))
				{	
					iCounter++;
					int iFindIndex=-1; 
					for(int k=j+1;k<(long)tagDataArr.size();k++)
					{
						if((m_dSValueArr[i]>=tagDataArr[k].value)||(tagDataArr[k].value>m_dEValueArr[i]))
						{
							iFindIndex=k;
							break;
						}
					}
					if(iFindIndex==-1)//һֱ�����������
					{
						iFindIndex=tagDataArr.size()-1;
					}
					iLenght+=tagDataArr[iFindIndex].time-tagDataArr[j].time;
					j=iFindIndex;
				}
			}
			//д����
			if(iCounter!=0)//
			{
				if(!checkTableExist(strTableName))
				{
					createTable(strTableName);
				}
				if(m_iTagTimeIntM==1)//�Կ�ʼ����ʱ��������ִ���
				{
					getTimeIntM(lSTime,lETime);
				}
				writeDataToTable(strTableName,lSTime,lETime,iStatType,iLenght,iCounter);
			}
		}
	}
	else if(m_iStatType==2)//ͳ�ƽ�������Ĵ���
	{}
	tagDataArr.clear();
	//OutputResultToGlobalWS();
}
void CStatEnumBlock::getTimeIntM(long &lSTime,long &lETime)
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
void CStatEnumBlock::statData(std::vector<TagData> &tagDataArr,
							 double &dSum,
							 double &dMin,
							 double &dMax)
{
	dSum=0;
	dMin=0;
	dMax=0;
	for(long j=0;j<(long)tagDataArr.size();j++)
	{
		if(j==0)
		{
			dMin=tagDataArr[j].value;
			dMax=dMin;
		}
		else
		{
			if(tagDataArr[j].value<dMin)
			{
				dMin=tagDataArr[j].value;
			}
			if(tagDataArr[j].value>dMax)
			{
				dMax=tagDataArr[j].value;
			}
		}
		dSum+=tagDataArr[j].value;
	}
}

void CStatEnumBlock::writeDataToTable(
									 CString strTable,
									 long lSTime,
									 long lETime,
									 int  iStatType,
									 double dTotalValue,
									 long iCount)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
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
	strSQL.Format("insert into %s(start_date,end_date,in_date,state_type,total_value,count)	values(\'%s\',\'%s\',\'%s\',\'%d\',\'%f\',\'%d\')",
		strTable,strSTime,strETime,strCurTime,
		iStatType,dTotalValue,iCount);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CStatEnumBlock::getInRangeValue(double &dValue)
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
void CStatEnumBlock::createTable(CString strTable)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	_variant_t RecordsAffected;
	CString strSQL;
	strSQL.Format("create table %s (id bigint not null auto_increment, start_date datetime,end_date datetime,in_date datetime, max_value decimal(18,6), min_value decimal(18,6), state_type tinyint, total_value decimal(18,6),count int, primary key (id));",strTable);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
bool CStatEnumBlock::checkTableExist(CString strTable)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
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
void CStatEnumBlock::readDataFromRTDB(
									 CString strName,
									 long lSTime,
									 long lETime,
									 int iReqType,//0:����;1:ԭʼ
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
	if(iReqType==0)//����
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
			if((tagDatas[i].value<dLimitValue)&&(tagDatas[i].value>-1*dLimitValue)&&((tagDatas[i].status==0)||(tagDatas[i].status==2)))//״̬Ϊ��ȷ
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
	else if(iReqType==1)//ԭʼ
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
					if((tagDatas[j].value<dLimitValue)&&(tagDatas[j].value>-1*dLimitValue)&&((tagDatas[j].status==0)||(tagDatas[j].status==2)))//״̬Ϊ��ȷ
					{
						tagTemp.value=tagDatas[j].value;
						tagTemp.time=tagDatas[j].time;
						tagTemp.status=tagDatas[j].status;
						tagDataArr.push_back(tagTemp);
					}
				}
				if(nCount>0)
				{
					req.stTime=tagTemp.time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
				}
			}
			RTDBFreePointer(tagDatas);
		}while ((nCount==1024) && (req.stTime !=req.enTime));
	}
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatEnumBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
	}
	else if(strPropName.Compare("TimeMInt")==0)//������ȡ��
	{
		if(!strItem1.IsEmpty())  {m_iTagTimeIntM=atoi(strItem1);}
	}
	else if(strPropName.Compare("VarInf")==0)
	{
		if(!strItem1.IsEmpty())  {m_strVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strVarUnit=strItem2;}
		if(!strItem3.IsEmpty())  {m_strVarDesp=strItem3;}
		if(!strItem4.IsEmpty())  {m_iReqType=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_lSnapshotT=atoi(strItem5);}
	}
	else if(strPropName.Compare("VarOut")==0)
	{
		if(!strItem1.IsEmpty())  {m_strOutName=strItem1;}
	}
	else if(strPropName.Compare("StatType")==0)
	{
		if(!strItem1.IsEmpty())  {m_iStatType=atoi(strItem1);}
	}
	else if(strPropName.Left(7)=="SEValue")
	{
		int iIndex=atoi(strPropName.Right(strPropName.GetLength()-7));
		m_dSValueArr.SetAtGrow(iIndex,atof(strItem1));
		m_dEValueArr.SetAtGrow(iIndex,atof(strItem2));
		m_iSETag.SetAtGrow(iIndex,atoi(strItem3));
		m_iUpOrDowmLimit.SetAtGrow(iIndex,atoi(strItem4));
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatEnumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatEnum dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	
	dlg.m_strVarName=m_strVarName;
	dlg.m_strVarUnit=m_strVarUnit;
	dlg.m_strVarDesp=m_strVarDesp;
	dlg.m_iReqType=m_iReqType;
	dlg.m_lSnapshotT=m_lSnapshotT;
	dlg.m_iStatType=m_iStatType;
	dlg.m_strOutName=m_strOutName;
	
	dlg.m_dSValueArr.Copy(m_dSValueArr);
	dlg.m_dEValueArr.Copy(m_dEValueArr);
	dlg.m_iSETag.Copy(m_iSETag);
	dlg.m_iUpOrDowmLimit.Copy(m_iUpOrDowmLimit);
	dlg.m_iTagTimeIntM=m_iTagTimeIntM;
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_strVarName=dlg.m_strVarName;
		m_strVarUnit=dlg.m_strVarUnit;
		m_strVarDesp=dlg.m_strVarDesp;
		m_iReqType=dlg.m_iReqType;
		m_lSnapshotT=dlg.m_lSnapshotT;
		m_iStatType=dlg.m_iStatType;
		m_strOutName=dlg.m_strOutName;
		m_iTagTimeIntM=dlg.m_iTagTimeIntM;

		m_dSValueArr.Copy(dlg.m_dSValueArr);
		m_dEValueArr.Copy(dlg.m_dEValueArr);
		m_iSETag.Copy(dlg.m_iSETag);
		m_iUpOrDowmLimit.Copy(dlg.m_iUpOrDowmLimit);

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
void CStatEnumBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="DBName";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="TimeMInt";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iTagTimeIntM); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	


	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="VarOut";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strOutName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="VarInf";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strVarName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strVarUnit); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strVarDesp); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iReqType); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_lSnapshotT); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="StatType";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iStatType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������

	for(int i=0;i<m_dSValueArr.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("SEValue%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
		strFieldValue.Format("%f",m_dSValueArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%f",m_dEValueArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%d",m_iSETag[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%d",m_iUpOrDowmLimit[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CStatEnumBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatEnumBlock*)pBlock)->m_strDBName;

	this->m_iTagTimeIntM=((CStatEnumBlock*)pBlock)->m_iTagTimeIntM;

	this->m_strVarName=((CStatEnumBlock*)pBlock)->m_strVarName;
	this->m_strVarUnit=((CStatEnumBlock*)pBlock)->m_strVarUnit;
	this->m_strVarDesp=((CStatEnumBlock*)pBlock)->m_strVarDesp;
	this->m_strOutName=((CStatEnumBlock*)pBlock)->m_strOutName;

	this->m_iReqType=((CStatEnumBlock*)pBlock)->m_iReqType;
	this->m_lSnapshotT=((CStatEnumBlock*)pBlock)->m_lSnapshotT;
	this->m_iStatType=((CStatEnumBlock*)pBlock)->m_iStatType;

	this->m_dSValueArr.Copy(((CStatEnumBlock*)pBlock)->m_dSValueArr);
	this->m_dEValueArr.Copy(((CStatEnumBlock*)pBlock)->m_dEValueArr);
	this->m_iSETag.Copy(((CStatEnumBlock*)pBlock)->m_iSETag);
	this->m_iUpOrDowmLimit.Copy(((CStatEnumBlock*)pBlock)->m_iUpOrDowmLimit);
}
//�����Է��ô���������
void CStatEnumBlock::SetBlockPropertyToTransferData(
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
	strcpy(pData->m_cPropName,"TimeMInt");
	
	str.Format("%d",m_iTagTimeIntM);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	CString strFieldValue;

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strFieldValue.Format("VarOut");
	strcpy(pData->m_cPropName,strFieldValue);
	str.Format("%s",m_strOutName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strFieldValue.Format("VarInf");
	strcpy(pData->m_cPropName,strFieldValue);
	str.Format("%s",m_strVarName);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strVarUnit);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strVarDesp);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iReqType);
	strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_lSnapshotT);
	strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strFieldValue.Format("StatType");
	strcpy(pData->m_cPropName,strFieldValue);
	str.Format("%d",m_iStatType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
	
	for(int i=0;i<m_dSValueArr.GetSize();i++)
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("SEValue%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%f",m_dSValueArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%f",m_dEValueArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%d",m_iSETag[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",m_iUpOrDowmLimit[i]);
		strcpy(pData->m_cValueItem4,str);
		dataArr.Add(pData);
	}
}