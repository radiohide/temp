// StatAvgBlock.cpp: implementation of the CStatAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatAvgBlock.h"
#include "PropertyDlgStatAvg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatAvgBlock::CStatAvgBlock()
{
	dLimitValue=10000000000000000000;//1E19;
	SetBlockType(_T( "StatAvg" ) );
	ConstructBlockInputOutput();
	m_iTagTimeIntM=1;//默认取整分
	m_iTimeLengthIntM=2;//默认整分左右两秒取整分	

	m_iConOn=0;//条件开关
	m_strConVar="";
	m_iConReqType=0;//查询方式
	m_iConSnapShotT=300;//快照周期
	m_iCon1Fun=1;//条件方法1
	m_iCon2Fun=0;//条件方法2
	m_dCon1Value=1;//条件值1
	m_dCon2Value=0;//条件值2
	
}

CStatAvgBlock::~CStatAvgBlock()
{
	
}
//根据类型新建对象的函数
CCalcBlock* CStatAvgBlock::CreateFromString( const CString& strType )
{
	CStatAvgBlock* obj = new CStatAvgBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatAvgBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("STime"), VALUE_DOUBLE);
	AddInputPort(_T("ETime"), VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CStatAvgBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//计算函数，实现本模块的计算
void CStatAvgBlock::Calc()//进行计算的函数,需要重载
{
	if(!this->m_pModel->m_bConRTDB)//如果未连接则不进行计算。
		return;

	bool bTagCalcSuccess=true;
	GetInputValueFromGlobalWS();
    //计算过程
	double inValue1,inValue2;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
	long lSTime=(long)inValue1;
	long lETime=(long)inValue2;
	//统计条件处理
	
	std::vector<long> lGSTime;//好时间段开始时间
	std::vector<long> lGETime;//好时间段结束时间
	std::vector<long> lBSTime;//坏时间段开始时间
	std::vector<long> lBETime;//坏时间段结束时间
	if(this->m_iConOn)
	{
		std::vector<TagData> ConTagDataArr;
		//读取数据
		int iRet=readDataFromRTDB(m_strConVar,lSTime,lETime,m_iConReqType,m_iConSnapShotT,ConTagDataArr);
		if(iRet==0)
		{
			bTagCalcSuccess=false;
		}
		//得到满足条件时间段
		getStatConGBTimeArr(ConTagDataArr,lSTime,lETime,lGSTime,lGETime,lBSTime,lBETime);
	}
	//统计条件处理

	CTime sTime(lSTime);
	std::vector<TagData> tagDataArr;
	CString strTableName;
	for(int i=0;i<tagNameArr.GetSize();i++)
	{
		CString strVarName=tagNameArr[i];
		CString strOutName=strOutPutNameArr[i];
		int iReqType=reqTypeArr[i];
		int isnapshotT=snapshotTArr[i];
		//读数据
		int iRet=readDataFromRTDB(strVarName,lSTime,lETime,iReqType,isnapshotT,tagDataArr);
		if((iRet==0)||(!bTagCalcSuccess))
		{
			bTagCalcSuccess=false;
		}
		
		if(this->m_iConOn)
		{
			for(int i=tagDataArr.size()-1;i>=0;i--)//删除不合格数据
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
		
		if(bTagCalcSuccess)
		{
			//计算数据
			double dSum,dMin,dMax;
			statData(tagDataArr,dSum,dMin,dMax);
			//写数据
			strTableName.Format("%s_%d",strOutName,sTime.GetYear());
			if(!checkTableExist(strTableName))
			{
				createTable(strTableName);
			}
			if(m_iTagTimeIntM==1)//对开始结束时间进行整分处理
			{
				getTimeIntM(lSTime,lETime);
			}
			writeDataToTable(strTableName,lSTime,lETime,dMax,dMin,dSum,tagDataArr.size());
		}
	}
	tagDataArr.clear();
	//OutputResultToGlobalWS();
}
void CStatAvgBlock::getStatConGBTimeArr(std::vector<TagData> &ConTagDataArr,
										long lSTime,
										long lETime,
										std::vector<long> &lGSTime,//好时间段开始时间
										std::vector<long> &lGETime,//好时间段结束时间
										std::vector<long> &lBSTime,//坏时间段开始时间
										std::vector<long> &lBETime//坏时间段结束时间
										)
{
	lGSTime.clear();
	lGETime.clear();
	lBSTime.clear();
	lBETime.clear();
	if(ConTagDataArr.size()==0)//无数据
		return;
	//得到有效区间
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
	if(dRangMin>dRangMax)//使得区间有效
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
			if(i==0)//第一个值
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
			else if(i+1==ConTagDataArr.size())//最后一个值
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
					if(rangStatOld==0)//从好区间跳到坏区间
					{
						lGETime.push_back(lETime);
					}
					else if(rangStat==0)//从坏区间跳到好区间。
					{
						lBETime.push_back(lETime);
					}
					else//从坏区间跳到坏区间
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
					if(rangStatOld==0)//从好区间跳到坏区间
					{
						lGETime.push_back(oneTag.time-1);//避免时刻重复
						lBSTime.push_back(oneTag.time);
					}
					else if(rangStat==0)//从坏区间跳到好区间。
					{
						lBETime.push_back(oneTag.time-1);//避免时刻重复
						lGSTime.push_back(oneTag.time);
					}
					//从坏区间跳到坏区间、则认为保持在坏区间。
				}
				rangStatOld=rangStat;
			}
		}
	}
}
int CStatAvgBlock::getConValueRegionNum(double dValue,const double &dRangMin,const double &dRangMax)//得到条件变量，在条件区间内的位置。
//返回0:在区间内；返回1:在区间上；返回-1:在区间下
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
void CStatAvgBlock::getTimeIntM(long &lSTime,long &lETime)
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
void CStatAvgBlock::statData(std::vector<TagData> &tagDataArr,
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

void CStatAvgBlock::writeDataToTable(
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
	getInRangeValue(dMaxValue);
	getInRangeValue(dMinValue);
	getInRangeValue(dSum);
	strSQL.Format("insert into %s(start_date,end_date,in_date,max_value,min_value,total_value,count) values(\'%s\',\'%s\',\'%s\',\'%f\',\'%f\',\'%f\',\'%d\')",
		strTable,strSTime,strETime,strCurTime,
		dMaxValue,dMinValue,dSum,iCount);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CStatAvgBlock::getInRangeValue(double &dValue)
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
void CStatAvgBlock::createTable(CString strTable)
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
bool CStatAvgBlock::checkTableExist(CString strTable)
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
int CStatAvgBlock::readDataFromRTDB(
									 CString strName,
									 long lSTime,
									 long lETime,
									 int iReqType,//0:快照;1:原始
									 int isnapshotT,
									 std::vector<TagData> &tagDataArr)
{
	int iRet=0;
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
	if(iReqType==0)//快照
	{
		req.tPeriod=isnapshotT;
		req.reqType=4;
		
		if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
		else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
		TagData *tagDatas = new TagData[nCount];
		memset(tagDatas,0,sizeof(TagData)*nCount);
		int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
		if(nRet!=0)
		{
			for(int i=0;i<50;i++)//若读取失败，则尝试50次读取。
			{
				memset(tagDatas,0,sizeof(TagData)*nCount);
				::Sleep(5);
				nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
				if(nRet==0)
					break;
			}
		}
		if(nRet!=0)
		{
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
			return iRet;
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
		iRet=1;
	}
	else if(iReqType==1)//原始
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
				return iRet;
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
		iRet=1;
	}
	return iRet;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStatAvgBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
	}
	else if(strPropName.Compare("TimeMInt")==0)//整分钟取整
	{
		if(!strItem1.IsEmpty())  {m_iTagTimeIntM=atoi(strItem1);}
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
//重载以显示不同的属性对象对话框
void CStatAvgBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatAvg dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);

	dlg.tagNameArr.Copy(tagNameArr);//点名
	dlg.unitArr.Copy(unitArr);//单位
	dlg.dispArr.Copy(dispArr);//描述
	dlg.reqTypeArr.Copy(reqTypeArr);//查询方式
	dlg.snapshotTArr.Copy(snapshotTArr);//快照周期

	dlg.strOutPutNameArr.Copy(strOutPutNameArr);
	dlg.m_iTagTimeIntM=m_iTagTimeIntM;

	dlg.m_iConOn=m_iConOn;//条件开关
	dlg.m_strConVar=m_strConVar;//条件变量
	dlg.m_strConUnit=m_strConUnit;//单位
	dlg.m_strConDesp=m_strConDesp;//描述
	dlg.m_iConReqType=m_iConReqType;//查询方式
	dlg.m_iConSnapShotT=m_iConSnapShotT;//快照周期
	dlg.m_iCon1Fun=m_iCon1Fun;//条件方法1
	dlg.m_iCon2Fun=m_iCon2Fun;//条件方法2
	dlg.m_dCon1Value=m_dCon1Value;//条件值1
	dlg.m_dCon2Value=m_dCon2Value;//条件值2

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
		
		m_iConOn=dlg.m_iConOn;//条件开关
		m_strConVar=dlg.m_strConVar;//条件变量
		m_strConUnit=dlg.m_strConUnit;//单位
		m_strConDesp=dlg.m_strConDesp;//描述
		m_iConReqType=dlg.m_iConReqType;//查询方式
		m_iConSnapShotT=dlg.m_iConSnapShotT;//快照周期
		m_iCon1Fun=dlg.m_iCon1Fun;//条件方法1
		m_iCon2Fun=dlg.m_iCon2Fun;//条件方法2
		m_dCon1Value=dlg.m_dCon1Value;//条件值1
		m_dCon2Value=dlg.m_dCon2Value;//条件值2

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
void CStatAvgBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="TimeMInt";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iTagTimeIntM); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	//条件变量
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="ConExpr";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iConOn); //统计条件开关
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iCon1Fun); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iCon2Fun); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%f",m_dCon1Value); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%f",m_dCon2Value); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="ConVar";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strConVar); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strConUnit); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strConDesp); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iConReqType); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iConSnapShotT); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名
	//条件变量

	for(int i=0;i<tagNameArr.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("VarData%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
		strFieldValue.Format("%s",tagNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",unitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",dispArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",reqTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",snapshotTArr[i]); //属性名
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	
	}
	for(i=0;i<strOutPutNameArr.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("VarOut%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
		strFieldValue.Format("%s",strOutPutNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	}
	
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CStatAvgBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatAvgBlock*)pBlock)->m_strDBName;
	this->m_iTagTimeIntM=((CStatAvgBlock*)pBlock)->m_iTagTimeIntM;
	this->tagNameArr.Copy(((CStatAvgBlock*)pBlock)->tagNameArr);
	this->unitArr.Copy(((CStatAvgBlock*)pBlock)->unitArr);
	this->dispArr.Copy(((CStatAvgBlock*)pBlock)->dispArr);
	this->reqTypeArr.Copy(((CStatAvgBlock*)pBlock)->reqTypeArr);
	this->snapshotTArr.Copy(((CStatAvgBlock*)pBlock)->snapshotTArr);
	this->strOutPutNameArr.Copy(((CStatAvgBlock*)pBlock)->strOutPutNameArr);
	
	m_iConOn=((CStatAvgBlock*)pBlock)->m_iConOn;//条件开关
	m_strConVar=((CStatAvgBlock*)pBlock)->m_strConVar;//条件变量
	m_strConUnit=((CStatAvgBlock*)pBlock)->m_strConUnit;//单位
	m_strConDesp=((CStatAvgBlock*)pBlock)->m_strConDesp;//描述
	m_iConReqType=((CStatAvgBlock*)pBlock)->m_iConReqType;//查询方式
	m_iConSnapShotT=((CStatAvgBlock*)pBlock)->m_iConSnapShotT;//快照周期
	m_iCon1Fun=((CStatAvgBlock*)pBlock)->m_iCon1Fun;//条件方法1
	m_iCon2Fun=((CStatAvgBlock*)pBlock)->m_iCon2Fun;//条件方法2
	m_dCon1Value=((CStatAvgBlock*)pBlock)->m_dCon1Value;//条件值1
	m_dCon2Value=((CStatAvgBlock*)pBlock)->m_dCon2Value;//条件值2
}
//将属性放置传输数据中
void CStatAvgBlock::SetBlockPropertyToTransferData(
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

