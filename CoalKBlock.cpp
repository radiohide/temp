// CoalKBlock.cpp: implementation of the CCoalKBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "CoalKBlock.h"
#include "PropertyDlgCoalK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoalKBlock::CCoalKBlock()
{
	m_iFDay=1;
	m_iSHour=8;
	m_iSMinute=0;
	m_iSSecond=0;

	m_iBDay=0;
	m_iEHour=8;
	m_iEMinute=0;
	m_iESecond=0;

	m_iStepLength=5;
	m_iStepUnit=2;//1:�룬2�֣�3Сʱ

	m_StrPowerUnit="MW";

	m_strTableName="";
	m_strDBName="";

	m_lOffset=0;
	m_TagTimeOffsetOrInput=1;//�����ڲ�ʱ��

	SetBlockType(_T( "CoalK" ) );//��������
	ConstructBlockInputOutput ();
}
	
CCoalKBlock::~CCoalKBlock()
{

}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CCoalKBlock::ConstructBlockInputOutput ()
{
	
}
//���������½�����ĺ���
CCalcBlock* CCoalKBlock::CreateFromString( const CString& strType )
{
	CCoalKBlock* obj = new CCoalKBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CCoalKBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	m_bConDB=(loadDll()>0);
}
BOOL CCoalKBlock::loadDll()
{
	hDLL = ::LoadLibrary("RTDBInterface.dll");
	int Ret =initDbConnect(hDLL);
	if(Ret ==0)
	{
		return true;
	}
	else
	{
		//AfxMessageBox(_T("ʵʱ������ʧ��!"));
		return false;
	}
}
int CCoalKBlock::initDbConnect(HINSTANCE hDLL)
{
	char *strParameters [4];//����ʵʱ�����Ӳ���
	//�������ݿ�����
	try
	{
		for (int i=0;i<4;i++)
		{
			strParameters[i] = (char*)malloc(80);
			memset(strParameters[i],0,80);
		}
		char inBuf[80];
		int n = 0;
		CString path= gGetExeFileDir() + "ePerfManage.CFG";

		n=GetPrivateProfileString ("VeStoreConfig", "rtdb.param1", "192.168.1.122:5450", inBuf, 80, path);
		strcpy(strParameters[0],inBuf);
		n= GetPrivateProfileString ("VeStoreConfig", "rtdb.param2", "piadmin", inBuf, 80, path);
		strcpy(strParameters[1],inBuf);
		n=GetPrivateProfileString ("VeStoreConfig", "rtdb.param3", "gdtzsis", inBuf, 80, path);
		strcpy(strParameters[2],inBuf);
		n=GetPrivateProfileString ("VeStoreConfig", "rtdb.param4", "RTDBDao", inBuf, 80, path);
		strcpy(strParameters[3],inBuf);
		
		typedef int (*pInitConnect)(char* strParameters[],long nCount);
		pInitConnect InitConnect =(pInitConnect)GetProcAddress(hDLL,"InitConnect");
		int nRet = InitConnect(strParameters,4);
		if(nRet!=0)
			return -1;
		else
		{	
			return 0;
		}
	}
	catch(CException *e)
	{
		e->Delete();
		return -1;
	}
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CCoalKBlock::Calc() //���м���ĺ���,��Ҫ����
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	GetInputValueFromGlobalWS();
    //�������
	double timeflag=0;
	if(m_TagTimeOffsetOrInput==1)//�ڲ�����ʱ��+ƫ��
	{
		timeflag=this->m_pModel->m_lDataTime-this->m_lOffset;
	}
	else//�ⲿʱ��
	{
		CCalcPort *pPortObj1 = GetInputPortObj(0);
		int ret = pPortObj1->GetPortUniValue().GetDoubleVal(&timeflag);
	}
	if(timeflag<0)
		timeflag=0;
	//��ȡmysql��Ҫ�����
	CArray<long,long> sTimeArr;
	CArray<long,long> eTimeArr;
	long lRealRecordTime;
	BOOL ret=getSETimeNeedToCalc(m_pConRDB,strDBType,(long)timeflag,lRealRecordTime,sTimeArr,eTimeArr);
	if(!ret)
		return;
	double dCoalK=0.0;
	double dKSum=0;
	int iCounter=0;
	for(int i=0;i<sTimeArr.GetSize();i++)
	{
		if(getCoalK(m_strFlowTagNameArr,sTimeArr[i],eTimeArr[i],dCoalK))//�õ�ϵ��
		{
			dKSum+=dCoalK;
			iCounter++;
		}
	}
	if(iCounter>0)
	{
		dCoalK=dKSum/(iCounter+0.0);
	}
	writeCoalK(m_pConRDB,lRealRecordTime,dCoalK);
}

void CCoalKBlock::writeCoalK(_ConnectionPtr &m_pConRDB,long ltime,double dCoalK)
{
	CString strSQL;
	_variant_t RecordsAffected;
	CTime t(ltime);
	CString strTime=t.Format("%Y-%m-%d %H:%M:%S");

	strSQL.Format("UPDATE `%s` SET CoalK=%.5f WHERE DateTime='%s'",m_strTableName,dCoalK,strTime);
	try	
	{
		m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}
}
//��ȡ�������ݽ��м��㣬�õ�С�����ú��ϵ��K
BOOL CCoalKBlock::getCoalK(CStringArray &TagFlowArr,long sTime,long eTime,double &dCoalK)
{
	BOOL ret=FALSE;
	if(!m_bConDB)
	{
		m_bConDB=(loadDll()>0);
	}
	if(m_bConDB)
	{
		typedef int (*pGetAggregationDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * pTagData);
		pGetAggregationDataByTagName GetAggregationDataByTagName=(pGetAggregationDataByTagName)::GetProcAddress(hDLL,"GetAggregationDataByTagName");//���ͳ�����ݲ�ѯ
		
		TagData *pTagData;
		ReadHiDataRequest req;
		int nPeriod=30;//ȡ30��һ��
		int nCount=0;
		req.stTime=sTime;
		req.enTime=eTime;
		req.tPeriod=nPeriod;
		req.reqType=3;//ȡƽ��
	
		CString strName=m_strPowerTagName;
		strcpy(req.pointName,strName);
		if ((req.enTime - req.stTime)%nPeriod)
		{
			nCount = (req.enTime - req.stTime)/nPeriod+1;
		}
		else
		{
			nCount = (req.enTime - req.stTime)/nPeriod;
		}
		pTagData = (TagData*)malloc(nCount*sizeof(TagData));
		int nRet = GetAggregationDataByTagName(&req,pTagData);	
		double dPower=pTagData->value;
		
		if(nRet==0)//
		{
			double dFlowSum=0;
			for(int i=0;i<TagFlowArr.GetSize();i++)
			{
				CString strName=TagFlowArr[i];
				strcpy(req.pointName,strName);
				nRet = GetAggregationDataByTagName(&req,pTagData);	
				if(nRet==0)
				{
					dFlowSum+=(pTagData->value);//ȡ����
				}
			}
			if(dFlowSum==0)
			{
				ret=FALSE;
			}
			else
			{
				dCoalK=dPower/dFlowSum;
				if(dCoalK<0)
					ret=FALSE;
				else
					ret=TRUE;
			}
		}
		else//��ȡ����ʧ��
			ret=FALSE;
		if(pTagData!=NULL)
		{
			delete pTagData;
			pTagData=NULL;
		}
	}
	else
	{
		ret=FALSE;
	}
	return ret;
}
//�õ���ǰ����ʱ����Ҫ����ľ����ʱ�������
BOOL CCoalKBlock::getSETimeNeedToCalc(_ConnectionPtr &m_pConRDB,
									  CString strDBType,
									  long timeflag,
									  long &lRealRecordTime,
									  CArray<long,long> &sTimeArr,
									  CArray<long,long> &eTimeArr)
{
	sTimeArr.RemoveAll();
	eTimeArr.RemoveAll();

	BOOL ret=FALSE;
	CTime  t1(timeflag);
	CString timestr1;
	timestr1.Format("%d-%.2d-%.2d %.2d:%.2d:%.2d",t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());
	
	_RecordsetPtr m_pRec;
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strDBType.MakeUpper();
		if(strDBType=="MYSQL")
		{
			strSql.Format("select  * from %s  where CoalK=0 and DateTime<='%s' order by DateTime desc limit 0,1;",
				m_strTableName,timestr1);
		}
		else if(strDBType=="MDB")
		{
			strSql.Format("select top 1 * from %s  where CoalK=0 and DateTime<=#%s# order by DateTime desc;",
				m_strTableName,timestr1);
		}
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str=e->ErrorMessage();	//AfxMessageBox(e->ErrorMessage());   
	}  
	long lRetTime=0;
	while(!m_pRec->adoEOF)
	{
		CString strDataTime =(char *)_bstr_t(m_pRec->GetCollect("DateTime"));
		
		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;     
		sscanf(strDataTime, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);   
		CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
		lRetTime=t.GetTime();
		lRealRecordTime=lRetTime;
		ret=TRUE;
		break; 
	}
	if(ret)//�õ���Ҫ�����ʱ����
	{
		CTime retTime(lRetTime);
		CTime tempTimeS=retTime-CTimeSpan(m_iFDay,0,0,0);
		CTime tempTimeE=retTime-CTimeSpan(m_iBDay,0,0,0);
		CTime sTime(tempTimeS.GetYear(),tempTimeS.GetMonth(),tempTimeS.GetDay(),m_iSHour,m_iSMinute,m_iSSecond);
		CTime eTime(tempTimeE.GetYear(),tempTimeE.GetMonth(),tempTimeE.GetDay(),m_iEHour,m_iEMinute,m_iESecond);
		if(sTime.GetTime()==eTime.GetTime())//�������ʱ��γ���Ϊ����ȡ�õ��Ĳ���ʱ��ǰ5���ӵ�ʱ��������м��㡣
		{
			sTimeArr.Add(sTime.GetTime()-5*60);//
			eTimeArr.Add(eTime.GetTime());
		}
		else
		{
			long lStep=0;
			if(m_iStepUnit==1)//��
				lStep=m_iStepLength;
			else if(m_iStepUnit==2)//��
				lStep=m_iStepLength*60;
			else if(m_iStepUnit==3)//Сʱ
				lStep=m_iStepLength*60*60;
			if(lStep==0)//�����������Ϊ0��������Ϊ300�롣
				lStep=300;
			
			long lTempTime=sTime.GetTime();
			while(lTempTime+lStep<eTime.GetTime())
			{
				sTimeArr.Add(lTempTime);
				eTimeArr.Add(lTempTime+lStep);
				lTempTime=lTempTime+lStep;
			}
		}
	}
	return ret;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CCoalKBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("FOffSet")==0)
	{
		if(!strItem1.IsEmpty())  {m_iFDay=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iSHour=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iSMinute=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iSSecond=atoi(strItem4);}
	}
	else if(strPropName.Compare("BOffSet")==0)
	{
		if(!strItem1.IsEmpty())  {m_iBDay=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iEHour=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iEMinute=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iESecond=atoi(strItem4);}
	}
	else if(strPropName.Compare("Step")==0)
	{
		if(!strItem1.IsEmpty())  {m_iStepLength=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iStepUnit=atoi(strItem2);}
	}
	else if(strPropName.Compare("PowerTag")==0)
	{
		if(!strItem1.IsEmpty())  {m_strPowerTagName=strItem1;}
		if(!strItem2.IsEmpty())  {m_StrPowerUnit=strItem2;}
	}
	else if(strPropName.Compare("RDB")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strTableName=strItem2;}
	}
	else if(strPropName.Compare("TimeOffset")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagTimeOffsetOrInput=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_lOffset=atoi(strItem2);}
	}
	else//��������
	{
		if(!strItem1.IsEmpty())  
			m_strFlowTagNameArr.Add(strItem1);
		else
			return;
		if(!strItem2.IsEmpty())  
			m_strFlowUnitArr.Add(strItem2);
		else
			m_strFlowUnitArr.Add("");
		if(!strItem3.IsEmpty())  
			m_strFlowDespArr.Add(strItem3);
		else
			m_strFlowDespArr.Add("");
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CCoalKBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgCoalK dlg;
	dlg.m_iFDay=m_iFDay;
	dlg.m_iBDay=m_iBDay;
	dlg.SetSETime(this->m_iSHour,this->m_iSMinute,this->m_iSSecond,this->m_iEHour,this->m_iEMinute,this->m_iESecond);
	dlg.m_iStepLength=m_iStepLength;
	dlg.SetStepUnit(this->m_iStepUnit);
	dlg.m_strPowerTagName=m_strPowerTagName;
	dlg.m_StrPowerUnit=	m_StrPowerUnit;
	dlg.SetFlowTagData(this->m_strFlowTagNameArr,this->m_strFlowUnitArr,this->m_strFlowDespArr);
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_lOffset=m_lOffset;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;

	if(dlg.DoModal()==IDOK)
	{
		m_iFDay=dlg.m_iFDay;
		m_iBDay=dlg.m_iBDay;
		m_iSHour=dlg.m_TimeS.GetHour();
		m_iSMinute=dlg.m_TimeS.GetMinute();
		m_iSSecond=dlg.m_TimeS.GetSecond();
		m_iEHour=dlg.m_TimeE.GetHour();
		m_iEMinute=dlg.m_TimeE.GetMinute();
		m_iESecond=dlg.m_TimeE.GetSecond();
		m_iStepLength=dlg.m_iStepLength;
		m_iStepUnit=dlg.m_iStepUnit;
		m_strPowerTagName=dlg.m_strPowerTagName;
		m_StrPowerUnit=dlg.m_StrPowerUnit;
		m_strFlowTagNameArr.Copy(dlg.m_strFlowTagNameArr);
		m_strFlowUnitArr.Copy(dlg.m_strFlowUnitArr);
		m_strFlowDespArr.Copy(dlg.m_strFlowDespArr);
		m_strTableName=dlg.m_strTableName;
		m_strDBName=dlg.m_strDBName;
		m_lOffset=dlg.m_lOffset;
		if(m_TagTimeOffsetOrInput!=dlg.m_TagTimeOffsetOrInput)
		{
			m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
			if(m_TagTimeOffsetOrInput==0)//�ⲿ
			{
				ClearInputPorts();
				AddInputPort(_T("Time"), VALUE_DOUBLE);
			}
			else
			{
				ClearInputPorts();
			}
		}
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CCoalKBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("FOffSet");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iFDay); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iSHour); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iSMinute); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iSSecond); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("BOffSet");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iBDay); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iEHour); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iEMinute); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iESecond); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("Step");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iStepLength); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iStepUnit); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("PowerTag");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strPowerTagName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_StrPowerUnit); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("RDB");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strTableName); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("TimeOffset");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_TagTimeOffsetOrInput); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_lOffset); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������

	for(int i=0;i<m_strFlowTagNameArr.GetSize();i++)//���ʽ
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("FlowTag.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",m_strFlowTagNameArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",m_strFlowUnitArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",m_strFlowDespArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CCoalKBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_iFDay=((CCoalKBlock*)pBlock)->m_iFDay;
	this->m_iSHour=((CCoalKBlock*)pBlock)->m_iSHour;
	this->m_iSMinute=((CCoalKBlock*)pBlock)->m_iSMinute;
	this->m_iSSecond=((CCoalKBlock*)pBlock)->m_iSSecond;

	this->m_iBDay=((CCoalKBlock*)pBlock)->m_iBDay;
	this->m_iEHour=((CCoalKBlock*)pBlock)->m_iEHour;
	this->m_iEMinute=((CCoalKBlock*)pBlock)->m_iEMinute;
	this->m_iESecond=((CCoalKBlock*)pBlock)->m_iESecond;

	this->m_iStepLength=((CCoalKBlock*)pBlock)->m_iStepLength;
	this->m_iStepUnit=((CCoalKBlock*)pBlock)->m_iStepUnit;

	this->m_strPowerTagName=((CCoalKBlock*)pBlock)->m_strPowerTagName;
	this->m_StrPowerUnit=((CCoalKBlock*)pBlock)->m_StrPowerUnit;

	this->m_strDBName=((CCoalKBlock*)pBlock)->m_strDBName;
	this->m_strTableName=((CCoalKBlock*)pBlock)->m_strTableName;

	this->m_lOffset=((CCoalKBlock*)pBlock)->m_lOffset;
	this->m_TagTimeOffsetOrInput=((CCoalKBlock*)pBlock)->m_TagTimeOffsetOrInput;

	this->m_strFlowTagNameArr.Copy(((CCoalKBlock*)pBlock)->m_strFlowTagNameArr);
	this->m_strFlowUnitArr.Copy(((CCoalKBlock*)pBlock)->m_strFlowUnitArr);
	this->m_strFlowDespArr.Copy(((CCoalKBlock*)pBlock)->m_strFlowDespArr);
}
//�����Է��ô���������
void CCoalKBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"FOffSet");	
	CString str;
	str.Format("%d",m_iFDay);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iSHour);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iSMinute);strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iSSecond);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BOffSet");	
	str.Format("%d",m_iBDay);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iEHour);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iEMinute);strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iESecond);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Step");
	str.Format("%d",m_iStepLength);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iStepUnit);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"PowerTag");
	str.Format("%s",m_strPowerTagName);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_StrPowerUnit);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"RDB");
	str.Format("%s",m_strDBName);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strTableName);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffset");
	str.Format("%d",m_TagTimeOffsetOrInput);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_lOffset);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	CString strFieldValue;
	for(int i=0;i<m_strFlowTagNameArr.GetSize();i++)//���ʽ
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("FlowTag.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",m_strFlowTagNameArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",m_strFlowUnitArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",m_strFlowDespArr[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
}