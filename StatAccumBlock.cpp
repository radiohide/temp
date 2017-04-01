// StatAccumBlock.cpp: implementation of the CStatAccumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatAccumBlock.h"
#include "PropertyDlgStatAccum.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatAccumBlock::CStatAccumBlock()
{
	m_dGain=1;
	dLimitValue=10000000000000000000;//1E19;
	m_iStatType=0;//Ĭ��ȡ����
	SetBlockType(_T( "StatAccum" ) );
	ConstructBlockInputOutput();
	m_iTagTimeIntM=1;//Ĭ��ȡ����
	m_iTimeLengthIntM=2;//Ĭ��������������ȡ����

	m_iConOn=0;//��������
	m_strConVar="";
	m_iConReqType=0;//��ѯ��ʽ
	m_iConSnapShotT=300;//��������
	m_iCon1Fun=1;//��������1
	m_iCon2Fun=0;//��������2
	m_dCon1Value=1;//����ֵ1
	m_dCon2Value=0;//����ֵ2
}

CStatAccumBlock::~CStatAccumBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatAccumBlock::CreateFromString( const CString& strType )
{
	CStatAccumBlock* obj = new CStatAccumBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CStatAccumBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("STime"), VALUE_DOUBLE);
	AddInputPort(_T("ETime"), VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatAccumBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
void CStatAccumBlock::Calc()//���м���ĺ���,��Ҫ����
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

	
	//ͳ����������
	
	std::vector<long> lGSTime;//��ʱ��ο�ʼʱ��
	std::vector<long> lGETime;//��ʱ��ν���ʱ��
	std::vector<long> lBSTime;//��ʱ��ο�ʼʱ��
	std::vector<long> lBETime;//��ʱ��ν���ʱ��
	if((this->m_iConOn)&&(m_iStatType!=1))//ͳ��������������������֡�
	{
		std::vector<TagData> ConTagDataArr;
		//��ȡ����
		readDataFromRTDB(m_strConVar,lSTime,lETime,m_iConReqType,m_iConSnapShotT,ConTagDataArr);
		//�õ���������ʱ���
		getStatConGBTimeArr(ConTagDataArr,lSTime,lETime,lGSTime,lGETime,lBSTime,lBETime);
	}
	//ͳ����������

	CTime sTime(lSTime);

	CString strTableName;
	std::vector<TagData> tagDataArr;
	for(int i=0;i<tagNameArr.GetSize();i++)
	{
		CString strVarName=tagNameArr[i];
		CString strOutName=strOutPutNameArr[i];
		if(m_iStatType==1)//��β��ֵ
		{
			//������
			TagData valueData1,valueData2;
			readDataFromRTDB(strVarName,lSTime,lETime,valueData1,valueData2);
			//��������
			double dTotal=0;
			double dMin=0;
			double dMax=0;
			if((valueData1.status!=-1)&&(valueData2.status!=-1))
			{
				if(valueData1.value<valueData2.value)
				{
					dMin=valueData1.value;
					dMax=valueData2.value;
				}
				else
				{
					dMin=valueData2.value;
					dMax=valueData1.value;
				}
				dTotal=(dMax-dMin)*m_dGain;//��������
			}
			//д����
			strTableName.Format("%s_%d",strOutName,sTime.GetYear());
			if(!checkTableExist(strTableName))
			{
				createTable(strTableName);
			}
			if(m_iTagTimeIntM==1)//�Կ�ʼ����ʱ��������ִ���
			{
				getTimeIntM(lSTime,lETime);
			}
			writeDataToTable(strTableName,lSTime,lETime,dMax,dMin,dTotal,2);
		}
		else//�����
		{
			int iReqType=reqTypeArr[i];
			int isnapshotT=snapshotTArr[i];
			//������
			readDataFromRTDB(strVarName,lSTime,lETime,iReqType,isnapshotT,tagDataArr);
			if(this->m_iConOn)
			{
				for(int i=tagDataArr.size()-1;i>=0;i--)//ɾ�����ϸ�����
				{
					TagData oneTag=tagDataArr[i];
					bool bBad=false;
					for(int j=0;j<lBSTime.size();j++)
					{
						if((oneTag.time>=lBSTime[j])&&(oneTag.time<=lBETime[j]))
						{
							bBad=true;
							break;
						}
					}
					if(bBad)
					{
						tagDataArr.erase(tagDataArr.begin()+i);
					}
				}
			}
			//��������
			double dAvg,dMin,dMax;
			statData(tagDataArr,dAvg,dMin,dMax);
			long lTimeLength=0;
			if(this->m_iConOn)
			{
				for(int i=0;i<lGSTime.size();i++)
				{
					lTimeLength+=lGETime[i]-lGSTime[i];
				}
			}
			else
			{
				lTimeLength=lETime-lSTime;
			}
			double dTotal=lTimeLength*(dAvg)*m_dGain;//��������
			//д����
			strTableName.Format("%s_%d",strOutName,sTime.GetYear());
			if(!checkTableExist(strTableName))
			{
				createTable(strTableName);
			}
			if(m_iTagTimeIntM==1)//�Կ�ʼ����ʱ��������ִ���
			{
				getTimeIntM(lSTime,lETime);
			}
			writeDataToTable(strTableName,lSTime,lETime,dMax,dMin,dTotal,tagDataArr.size());
		}
	}
	tagDataArr.clear();
}
void CStatAccumBlock::getStatConGBTimeArr(std::vector<TagData> &ConTagDataArr,
										long lSTime,
										long lETime,
										std::vector<long> &lGSTime,//��ʱ��ο�ʼʱ��
										std::vector<long> &lGETime,//��ʱ��ν���ʱ��
										std::vector<long> &lBSTime,//��ʱ��ο�ʼʱ��
										std::vector<long> &lBETime//��ʱ��ν���ʱ��
										)
{
	lGSTime.clear();
	lGETime.clear();
	lBSTime.clear();
	lBETime.clear();
	if(ConTagDataArr.size()==0)//������
		return;
	//�õ���Ч����
	double dRangMin=-9E+30;
	double dRangMax=9E+30;
	if((m_iCon1Fun==0)||(m_iCon1Fun==1))
	{
		dRangMin=m_dCon1Value;
	}
	else
	{
		dRangMax=m_dCon1Value;
	}
	if((m_iCon2Fun==1)||(m_iCon2Fun==2))
	{
		dRangMin=m_dCon2Value;
	}
	else if((m_iCon2Fun==3)||(m_iCon2Fun==4))
	{
		dRangMax=m_dCon2Value;
	}
	if(dRangMin>dRangMax)//ʹ��������Ч
	{
		double temp=dRangMin;
		dRangMin=dRangMax;
		dRangMax=dRangMin;
	}
	if(ConTagDataArr.size()==1)
	{
		TagData oneTag=ConTagDataArr[0];
		int rangStat=getConValueRegionNum(oneTag.value,dRangMin,dRangMax);
		if(rangStat==0)
		{
			lGSTime.push_back(lSTime);
			lGETime.push_back(lETime);
		}
		else
		{
			lBSTime.push_back(lSTime);
			lBETime.push_back(lETime);
		}
	}
	else
	{
		int rangStatOld=0;
		for(unsigned int i=0;i<ConTagDataArr.size();i++)
		{
			TagData oneTag=ConTagDataArr[i];
			int rangStat=getConValueRegionNum(oneTag.value,dRangMin,dRangMax);
			if(i==0)//��һ��ֵ
			{
				rangStatOld=rangStat;
				if(rangStat==0)
				{
					lGSTime.push_back(lSTime);
				}
				else
				{
					lBSTime.push_back(lSTime);
				}
			}
			else if(i+1==ConTagDataArr.size())//���һ��ֵ
			{
				if(rangStatOld==rangStat)
				{
					if(rangStat==0)
					{
						lGETime.push_back(lETime);
					}
					else
					{
						lBETime.push_back(lETime);
					}
				}
				else
				{
					if(rangStatOld==0)//�Ӻ���������������
					{
						lGETime.push_back(lETime);
					}
					else if(rangStat==0)//�ӻ��������������䡣
					{
						lBETime.push_back(lETime);
					}
					else//�ӻ���������������
					{
						lBETime.push_back(lETime);
					}
				}
			}
			else
			{
				if(rangStatOld==rangStat)
				{
					continue;
				}
				else
				{
					if(rangStatOld==0)//�Ӻ���������������
					{
						lGETime.push_back(oneTag.time-1);//����ʱ���ظ�
						lBSTime.push_back(oneTag.time);
					}
					else if(rangStat==0)//�ӻ��������������䡣
					{
						lBETime.push_back(oneTag.time-1);//����ʱ���ظ�
						lGSTime.push_back(oneTag.time);
					}
					//�ӻ��������������䡢����Ϊ�����ڻ����䡣
				}
				rangStatOld=rangStat;
			}
		}
	}
}
int CStatAccumBlock::getConValueRegionNum(double dValue,const double &dRangMin,const double &dRangMax)//�õ����������������������ڵ�λ�á�
//����0:�������ڣ�����1:�������ϣ�����-1:��������
{
	int iRet=-2;
	if(dValue>dRangMax)
	{
		iRet=1;
	}
	else if(dValue<dRangMin)
	{
		iRet=-1;
	}
	else
	{
		iRet=0;
	}
	return iRet;
}
void CStatAccumBlock::getTimeIntM(long &lSTime,long &lETime)
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
void CStatAccumBlock::statData(std::vector<TagData> &tagDataArr,
							 double &dAvg,
							 double &dMin,
							 double &dMax)
{
	double dSum=0;
	dAvg=0;
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
	if(tagDataArr.size()==0)
	{
		dAvg=0;
	}
	else
	{
		dAvg=dSum/tagDataArr.size();
	}
}
void CStatAccumBlock::readDataFromRTDB(
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
void CStatAccumBlock::readDataFromRTDB(
									   CString strVarName,
									   long lSTime,
									   long lETime,
									   TagData &valueData1,
									   TagData &valueData2)
{
	typedef  int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
	pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");
	memset(&valueData1,0,sizeof(TagData));
	memset(&valueData2,0,sizeof(TagData));
	int nRet = m_GetHistoryDataByTime(strVarName,lSTime, &valueData1);
	if(nRet!=0)
	{
		valueData1.status=-1;//������ʧ��
		valueData1.time=0;
		valueData1.value=0;
	}
	else
	{
		if((valueData1.value<dLimitValue)&&(valueData1.value>-1*dLimitValue)&&((valueData1.status==0)||(valueData1.status==2)))//״̬Ϊ��ȷ
		{}
		else
		{
			valueData1.status=-1;//������ʧ��
		}
	}
	nRet = m_GetHistoryDataByTime(strVarName,lETime, &valueData2);
	if(nRet!=0)
	{
		valueData2.status=-1;//������ʧ��
		valueData2.time=0;
		valueData2.value=0;
	}
	else
	{
		if((valueData2.value<dLimitValue)&&(valueData2.value>-1*dLimitValue)&&((valueData2.status==0)||(valueData2.status==2)))//״̬Ϊ��ȷ
		{}
		else
		{
			valueData2.status=-1;//������ʧ��
		}
	}
}
void CStatAccumBlock::writeDataToTable(
									 CString strTable,
									 long lSTime,
									 long lETime,
									 double dMaxValue,
									 double dMinValue,
									 double dSum,
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
	getInRangeValue(dMaxValue);
	getInRangeValue(dMinValue);
	getInRangeValue(dSum);
	strSQL.Format("insert into %s(start_date,end_date,in_date,max_value,min_value,total_value,count) values(\'%s\',\'%s\',\'%s\',\'%f\',\'%f\',\'%f\',\'%d\')",
		strTable,strSTime,strETime,strCurTime,
		dMaxValue,dMinValue,dSum,iCount);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CStatAccumBlock::getInRangeValue(double &dValue)
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
void CStatAccumBlock::createTable(CString strTable)
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
bool CStatAccumBlock::checkTableExist(CString strTable)
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
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatAccumBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
	}
	else if(strPropName.Compare("TimeMInt")==0)//������ȡ��
	{
		if(!strItem1.IsEmpty())  {m_iTagTimeIntM=atoi(strItem1);}
	}
	else if(strPropName.Compare("StatType")==0)
	{
		if(!strItem1.IsEmpty())  {m_iStatType=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_dGain=atof(strItem2);}
	}
	else if(strPropName.Compare("ConExpr")==0)
	{
		if(!strItem1.IsEmpty())  {m_iConOn=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iCon1Fun=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iCon2Fun=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_dCon1Value=atof(strItem4);}
		if(!strItem5.IsEmpty())  {m_dCon2Value=atof(strItem5);}
	}
	else if(strPropName.Compare("ConVar")==0)
	{
		if(!strItem1.IsEmpty())  {m_strConVar=strItem1;}
		if(!strItem2.IsEmpty())  {m_strConUnit=strItem2;}
		if(!strItem3.IsEmpty())  {m_strConDesp=strItem3;}
		if(!strItem4.IsEmpty())  {m_iConReqType=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_iConSnapShotT=atoi(strItem5);}
	}
	else if(strPropName.Left(7)=="VarData")
	{
		int iIndex=atoi(strPropName.Right(strPropName.GetLength()-7));
		tagNameArr.SetAtGrow(iIndex,strItem1);
		unitArr.SetAtGrow(iIndex,strItem2);
		dispArr.SetAtGrow(iIndex,strItem3);
		reqTypeArr.SetAtGrow(iIndex,atoi(strItem4));
		snapshotTArr.SetAtGrow(iIndex,atoi(strItem5));
	}
	else if(strPropName.Left(6)=="VarOut")
	{
		int iIndex=atoi(strPropName.Right(strPropName.GetLength()-6));
		strOutPutNameArr.SetAtGrow(iIndex,strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatAccumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatAccum dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);

	dlg.tagNameArr.Copy(tagNameArr);//����
	dlg.unitArr.Copy(unitArr);//��λ
	dlg.dispArr.Copy(dispArr);//����
	dlg.reqTypeArr.Copy(reqTypeArr);
	dlg.snapshotTArr.Copy(snapshotTArr);
	dlg.strOutPutNameArr.Copy(strOutPutNameArr);
	dlg.m_comboBoxStatType=m_iStatType;
	dlg.m_dGain=m_dGain;
	dlg.m_iTagTimeIntM=m_iTagTimeIntM;

		dlg.m_iConOn=m_iConOn;//��������
	dlg.m_strConVar=m_strConVar;//��������
	dlg.m_strConUnit=m_strConUnit;//��λ
	dlg.m_strConDesp=m_strConDesp;//����
	dlg.m_iConReqType=m_iConReqType;//��ѯ��ʽ
	dlg.m_iConSnapShotT=m_iConSnapShotT;//��������
	dlg.m_iCon1Fun=m_iCon1Fun;//��������1
	dlg.m_iCon2Fun=m_iCon2Fun;//��������2
	dlg.m_dCon1Value=m_dCon1Value;//����ֵ1
	dlg.m_dCon2Value=m_dCon2Value;//����ֵ2

	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_iTagTimeIntM=dlg.m_iTagTimeIntM;
		tagNameArr.Copy(dlg.tagNameArr);
		unitArr.Copy(dlg.unitArr);
		dispArr.Copy(dlg.dispArr);
		reqTypeArr.Copy(dlg.reqTypeArr);
		snapshotTArr.Copy(dlg.snapshotTArr);
		strOutPutNameArr.Copy(dlg.strOutPutNameArr);
		m_iStatType=dlg.m_comboBoxStatType;
		m_dGain=dlg.m_dGain;

		m_iConOn=dlg.m_iConOn;//��������
		m_strConVar=dlg.m_strConVar;//��������
		m_strConUnit=dlg.m_strConUnit;//��λ
		m_strConDesp=dlg.m_strConDesp;//����
		m_iConReqType=dlg.m_iConReqType;//��ѯ��ʽ
		m_iConSnapShotT=dlg.m_iConSnapShotT;//��������
		m_iCon1Fun=dlg.m_iCon1Fun;//��������1
		m_iCon2Fun=dlg.m_iCon2Fun;//��������2
		m_dCon1Value=dlg.m_dCon1Value;//����ֵ1
		m_dCon2Value=dlg.m_dCon2Value;//����ֵ2

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
void CStatAccumBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="StatType";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iStatType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%.18f",m_dGain); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

	//��������
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="ConExpr";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iConOn); //ͳ����������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iCon1Fun); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iCon2Fun); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%f",m_dCon1Value); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%f",m_dCon2Value); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="ConVar";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strConVar); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strConUnit); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strConDesp); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iConReqType); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iConSnapShotT); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	//��������

	for(int i=0;i<tagNameArr.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("VarData%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
		strFieldValue.Format("%s",tagNameArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",unitArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",dispArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",reqTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%d",snapshotTArr[i]); //������
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������	

	}
	for(i=0;i<strOutPutNameArr.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("VarOut%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
		strFieldValue.Format("%s",strOutPutNameArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CStatAccumBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatAccumBlock*)pBlock)->m_strDBName;
	this->m_iStatType=((CStatAccumBlock*)pBlock)->m_iStatType;
	this->m_iTagTimeIntM=((CStatAccumBlock*)pBlock)->m_iTagTimeIntM;
	this->tagNameArr.Copy(((CStatAccumBlock*)pBlock)->tagNameArr);
	this->unitArr.Copy(((CStatAccumBlock*)pBlock)->unitArr);
	this->dispArr.Copy(((CStatAccumBlock*)pBlock)->dispArr);
	this->reqTypeArr.Copy(((CStatAccumBlock*)pBlock)->reqTypeArr);
	this->snapshotTArr.Copy(((CStatAccumBlock*)pBlock)->snapshotTArr);
	this->strOutPutNameArr.Copy(((CStatAccumBlock*)pBlock)->strOutPutNameArr);
	this->m_dGain=((CStatAccumBlock*)pBlock)->m_dGain;

	m_iConOn=((CStatAccumBlock*)pBlock)->m_iConOn;//��������
	m_strConVar=((CStatAccumBlock*)pBlock)->m_strConVar;//��������
	m_strConUnit=((CStatAccumBlock*)pBlock)->m_strConUnit;//��λ
	m_strConDesp=((CStatAccumBlock*)pBlock)->m_strConDesp;//����
	m_iConReqType=((CStatAccumBlock*)pBlock)->m_iConReqType;//��ѯ��ʽ
	m_iConSnapShotT=((CStatAccumBlock*)pBlock)->m_iConSnapShotT;//��������
	m_iCon1Fun=((CStatAccumBlock*)pBlock)->m_iCon1Fun;//��������1
	m_iCon2Fun=((CStatAccumBlock*)pBlock)->m_iCon2Fun;//��������2
	m_dCon1Value=((CStatAccumBlock*)pBlock)->m_dCon1Value;//����ֵ1
	m_dCon2Value=((CStatAccumBlock*)pBlock)->m_dCon2Value;//����ֵ2
}	
//�����Է��ô���������
void CStatAccumBlock::SetBlockPropertyToTransferData(
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
	strcpy(pData->m_cPropName,"StatType");
	
	str.Format("%d",m_iStatType);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dGain);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeMInt");
	
	str.Format("%d",m_iTagTimeIntM);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ConExpr");
	
	str.Format("%d",m_iConOn);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iCon1Fun);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iCon2Fun);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dCon1Value);
	strcpy(pData->m_cValueItem4,str);
	str.Format("%f",m_dCon2Value);
	strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ConVar");
	
	str.Format("%s",m_strConVar);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strConUnit);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strConDesp);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iConReqType);
	strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_iConSnapShotT);
	strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	CString strFieldValue;
	for(int i=0;i<tagNameArr.GetSize();i++)
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("VarData%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",tagNameArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",unitArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",dispArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",reqTypeArr[i]);
		strcpy(pData->m_cValueItem4,str);
		str.Format("%d",snapshotTArr[i]);
		strcpy(pData->m_cValueItem5,str);
		dataArr.Add(pData);
	}
	for(i=0;i<strOutPutNameArr.GetSize();i++)
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("VarOut%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",strOutPutNameArr[i]);
		strcpy(pData->m_cValueItem1,str);
		dataArr.Add(pData);
	}
}

