// CalcModel.cpp: implementation of the CCalcModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CalcModel.h"
#include "CalcModule.h"

#include "ePerfManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CEPerfManageApp theApp;
CCalcModel::CCalcModel()
{
	m_bLoadRTDBDll=FALSE;
	m_bConRTDB=FALSE;
	m_bInitLoadAndConRTDB=FALSE;
	hDLL=NULL;

	m_lDataTime=0;
	m_lDataSTime=0;
	m_lDataETime=0;

	m_iModelWarningCount=0;
	m_iModelErrorCount=0;
	m_pWnd=NULL;
	m_iMsg=0;
	m_pLog=NULL;
	m_iLogOnOrOff=0;//��־�ر�
	m_iTraceOn=1;//׷�ٿ���1:����0:��
	m_iDebugOn=1;//���Կ���1:����0:��

	m_iTagModeLocalOrRemote=-1;
	m_strModelDesp = _T("δ�����ģ��");
	m_strModelName = _T("empty");
	m_strModelIP = _T("empty");
	m_strPort="1024";
	m_iStopTag=0;//��λ//for stop
	InitControlPara();

	for(int i=0;i<100;i++)//add source
	{
		m_ArrSourceRDBTagCon.Add(0);
		m_ArrSourceRDBName.Add("");
		m_ArrSourceRDBType.Add("");
		m_ArrConDBIP.Add("");//�������ݿ�IP
		m_ArrConDBName.Add("");//�������ݿ�����
		m_ArrConDBUser.Add("");//�������ݿ��û���
		m_ArrConDBPass.Add("");//�������ݿ�����
	}
	m_pInSertRtData=NULL;
	m_pInSertRtDataBack=NULL;
}

CCalcModel::~CCalcModel()
{
	while (!m_ModuleList.IsEmpty ())
	{
		delete m_ModuleList.RemoveHead ();
	}
	CloseAllSourceRDBCon();
	if(m_pInSertRtData!=NULL)
	{
		delete[] m_pInSertRtData;
		m_pInSertRtData=NULL;
	}
	if(m_pInSertRtDataBack!=NULL)
	{
		delete[] m_pInSertRtDataBack;
		m_pInSertRtDataBack=NULL;
	}
	typedef int (*pReleaseConnect)();
	pReleaseConnect ReleaseConnect =(pReleaseConnect)GetProcAddress(hDLL,"ReleaseConnect");//�ͷ����ӣ��ͷ�RTDBInterface�е��źŻ��⣩
	if(ReleaseConnect!=NULL)
	{
		int nRet = ReleaseConnect();
	}
}
void CCalcModel::DelModuleByName(CString strName)
{
	CCalcModule* pModule= NULL;
    POSITION pos = m_ModuleList.GetHeadPosition();
	POSITION posReal=pos;
	while(pos)
	{
		posReal=pos;
		pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		if(strName.Compare(pModule->GetModuleName())==0)
		{
			break;
		}
		else
		{
			pModule= NULL;
		}
	}
	if(pModule)
	{
		delete pModule;
		m_ModuleList.RemoveAt(posReal);
	}
}
void CCalcModel::CloseAllSourceRDBCon()
{
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			//int tagBeCon;
			CString dbType;
			_ConnectionPtr &pCon=GetConPtrRefByIndex(i+1);
			closeConnectPrt(pCon);
		}
		m_ArrSourceRDBTagCon[i]=0;
		m_ArrSourceRDBName[i]="";
		m_ArrSourceRDBType[i]="";
		m_ArrConDBIP[i]="";//�������ݿ�IP
		m_ArrConDBName[i]="";//�������ݿ�����
		m_ArrConDBUser[i]="";//�������ݿ��û���
		m_ArrConDBPass[i]="";//�������ݿ�����
	}
}
void CCalcModel::closeConnectPrt(_ConnectionPtr &pCon)
{
	if(pCon==NULL)
		return;
	if(pCon->State)
		pCon->Close();
}
void CCalcModel::EstablishSourceRDBConnect(_ConnectionPtr &pCon)
{
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		m_ArrSourceRDBTagCon[i]=0;
		m_ArrSourceRDBName[i]="";
		m_ArrSourceRDBType[i]="";
		m_ArrConDBIP[i]="";//�������ݿ�IP
		m_ArrConDBName[i]="";//�������ݿ�����
		m_ArrConDBUser[i]="";//�������ݿ��û���
		m_ArrConDBPass[i]="";//�������ݿ�����
	}
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	//////////////////////////////////////////////////////
	//��������������
	CString strTable;
	strTable.Format("SourceRDB");
	strSQL.Format("SELECT * FROM %s Where ModelName='%s' order by ID asc",strTable,this->m_strModelName);
    try	
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		return ;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	int counter=0;
	while(!pRecord->adoEOF)
	{
		if(counter+1>100)//���֧��10����ϵ�����ӣ�ȡ���õ�ǰʮ����//add source
			break;
		int tagBeCon=0;
		CString strDBType="";
		_ConnectionPtr &m_pConRDB=GetConPtrRefByIndex(counter+1);		
		CString RDBType,IP,RDBName,RDBUser,RDBPass,FileName,SourceName;
		BOOL bValid =TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("RDBType");
			if(Tempvalue.vt != VT_NULL) RDBType = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("IP");
			if(Tempvalue.vt != VT_NULL) IP = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBName");
			if(Tempvalue.vt != VT_NULL) RDBName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBUser");
			if(Tempvalue.vt != VT_NULL) RDBUser = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("RDBPass");
			if(Tempvalue.vt != VT_NULL) RDBPass = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("FileName");
			if(Tempvalue.vt != VT_NULL) FileName = (LPCSTR)_bstr_t(Tempvalue);
			Tempvalue = pRecord->GetCollect("SourceName");
			if(Tempvalue.vt != VT_NULL) SourceName = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {bValid=FALSE;}
        if(bValid) 
		{
			BOOL bResult=FALSE;
			CString upType=RDBType;
			upType.MakeUpper();
			if(upType=="MYSQL")
			{
				bResult=theApp.EstablishMySQLConnectPtr(m_pConRDB,IP,RDBName,RDBUser,RDBPass);
			}
			else if(upType=="MDB")
			{
				CString strFullFile;
				strFullFile.Format("%sRDBData\\%s",gGetExeFileDir(),FileName);
				bResult = theApp.EstablishMDBConnectPtr(m_pConRDB,strFullFile);	
			}
			if(bResult)
			{
				m_ArrSourceRDBName[counter]=SourceName;
				m_ArrSourceRDBTagCon[counter]=1;
				m_ArrSourceRDBType[counter]=RDBType;

				m_ArrConDBIP[counter]=IP;//�������ݿ�IP
				m_ArrConDBName[counter]=RDBName;//�������ݿ�����
				m_ArrConDBUser[counter]=RDBUser;//�������ݿ��û���
				m_ArrConDBPass[counter]=RDBPass;//�������ݿ�����
			}
		}
		pRecord->MoveNext();
		counter++;
	}
	pRecord->Close();
}
_ConnectionPtr& CCalcModel::GetSourceRDBByDesp(CString strDesp,int &tagBeCon,CString &dbType)
{
	int ibeFind=-1;
	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if((m_ArrSourceRDBTagCon[i]==1)&&(m_ArrSourceRDBName[i]==strDesp))
		{
			ibeFind=i;
			break;
		}
	}
	return GetSourceRDBByIndex(ibeFind+1,tagBeCon,dbType);
}
_ConnectionPtr &CCalcModel::GetConPtrRefByIndex(int index)
{
	if(index==1) return m_pConRDB1;
	else if(index==2) return m_pConRDB2;
	else if(index==3) return m_pConRDB3;
	else if(index==4) return m_pConRDB4;
	else if(index==5) return m_pConRDB5;
	else if(index==6) return m_pConRDB6;
	else if(index==7) return m_pConRDB7;
	else if(index==8) return m_pConRDB8;
	else if(index==9) return m_pConRDB9;
	else if(index==10) return m_pConRDB10;
	else if(index==11) return m_pConRDB11;
	else if(index==12) return m_pConRDB12;
	else if(index==13) return m_pConRDB13;
	else if(index==14) return m_pConRDB14;
	else if(index==15) return m_pConRDB15;
	else if(index==16) return m_pConRDB16;
	else if(index==17) return m_pConRDB17;
	else if(index==18) return m_pConRDB18;
	else if(index==19) return m_pConRDB19;
	else if(index==20) return m_pConRDB20;
	else if(index==21) return m_pConRDB21;
	else if(index==22) return m_pConRDB22;
	else if(index==23) return m_pConRDB23;
	else if(index==24) return m_pConRDB24;
	else if(index==25) return m_pConRDB25;
	else if(index==26) return m_pConRDB26;
	else if(index==27) return m_pConRDB27;
	else if(index==28) return m_pConRDB28;
	else if(index==29) return m_pConRDB29;
	else if(index==30) return m_pConRDB30;
	else if(index==31) return m_pConRDB31;
	else if(index==32) return m_pConRDB32;
	else if(index==33) return m_pConRDB33;
	else if(index==34) return m_pConRDB34;
	else if(index==35) return m_pConRDB35;
	else if(index==36) return m_pConRDB36;
	else if(index==37) return m_pConRDB37;
	else if(index==38) return m_pConRDB38;
	else if(index==39) return m_pConRDB39;
	else if(index==40) return m_pConRDB40;
	else if(index==41) return m_pConRDB41;
	else if(index==42) return m_pConRDB42;
	else if(index==43) return m_pConRDB43;
	else if(index==44) return m_pConRDB44;
	else if(index==45) return m_pConRDB45;
	else if(index==46) return m_pConRDB46;
	else if(index==47) return m_pConRDB47;
	else if(index==48) return m_pConRDB48;
	else if(index==49) return m_pConRDB49;
	else if(index==50) return m_pConRDB50;
	else if(index==51) return m_pConRDB51;
	else if(index==52) return m_pConRDB52;
	else if(index==53) return m_pConRDB53;
	else if(index==54) return m_pConRDB54;
	else if(index==55) return m_pConRDB55;
	else if(index==56) return m_pConRDB56;
	else if(index==57) return m_pConRDB57;
	else if(index==58) return m_pConRDB58;
	else if(index==59) return m_pConRDB59;
	else if(index==60) return m_pConRDB60;
	else if(index==61) return m_pConRDB61;
	else if(index==62) return m_pConRDB62;
	else if(index==63) return m_pConRDB63;
	else if(index==64) return m_pConRDB64;
	else if(index==65) return m_pConRDB65;
	else if(index==66) return m_pConRDB66;
	else if(index==67) return m_pConRDB67;
	else if(index==68) return m_pConRDB68;
	else if(index==69) return m_pConRDB69;
	else if(index==70) return m_pConRDB70;
	else if(index==71) return m_pConRDB71;
	else if(index==72) return m_pConRDB72;
	else if(index==73) return m_pConRDB73;
	else if(index==74) return m_pConRDB74;
	else if(index==75) return m_pConRDB75;
	else if(index==76) return m_pConRDB76;
	else if(index==77) return m_pConRDB77;
	else if(index==78) return m_pConRDB78;
	else if(index==79) return m_pConRDB79;
	else if(index==80) return m_pConRDB80;
	else if(index==81) return m_pConRDB81;
	else if(index==82) return m_pConRDB82;
	else if(index==83) return m_pConRDB83;
	else if(index==84) return m_pConRDB84;
	else if(index==85) return m_pConRDB85;
	else if(index==86) return m_pConRDB86;
	else if(index==87) return m_pConRDB87;
	else if(index==88) return m_pConRDB88;
	else if(index==89) return m_pConRDB89;
	else if(index==90) return m_pConRDB90;
	else if(index==91) return m_pConRDB91;
	else if(index==92) return m_pConRDB92;
	else if(index==93) return m_pConRDB93;
	else if(index==94) return m_pConRDB94;
	else if(index==95) return m_pConRDB95;
	else if(index==96) return m_pConRDB96;
	else if(index==97) return m_pConRDB97;
	else if(index==98) return m_pConRDB98;
	else if(index==99) return m_pConRDB99;
	else return m_pConRDB10;//if(index==100)
}
_ConnectionPtr &CCalcModel::GetSourceRDBByIndex(int index,int &tagBeCon,CString &dbType)//add source
{
	tagBeCon=m_ArrSourceRDBTagCon[index-1];
	dbType=m_ArrSourceRDBType[index-1];
	_ConnectionPtr &pCon=GetConPtrRefByIndex(index);
	CString strTypeTemp=dbType;
	strTypeTemp.MakeUpper();
	if(strTypeTemp=="MYSQL")
	{
		//int iCon=this->TestRDBCon(pCon);�ж���Ч�������ظ�����mysql����
		//if(iCon==0)
		if(pCon->GetState() == adStateClosed)
		{//����
			tagBeCon=theApp.EstablishMySQLConnectPtr(pCon,m_ArrConDBIP[index-1],m_ArrConDBName[index-1],m_ArrConDBUser[index-1],m_ArrConDBPass[index-1]);
			int iCount=0;
			while(tagBeCon==FALSE)
			{
				Sleep(50);
				tagBeCon=theApp.EstablishMySQLConnectPtr(pCon,m_ArrConDBIP[index-1],m_ArrConDBName[index-1],m_ArrConDBUser[index-1],m_ArrConDBPass[index-1]);
				iCount++;
				if(iCount>200)
					break;
			}
			m_ArrSourceRDBTagCon[index-1]=tagBeCon;
		}
	}
	return pCon;
}
int   CCalcModel::TestRDBCon(_ConnectionPtr &pCon)
{
	int iRet=0;
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select id from 'modellist' order by id asc limit 0,1");
	try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(...)
	{
		//AfxMessageBox(e.Description());
		return iRet;
	} 
	iRet=1;
	return iRet;
}
void  CCalcModel::AddModule(CCalcModule* pModule)
{
	m_ModuleList.AddTail(pModule);
}
int CCalcModel::LookUpMoudle(CString name,CString desp)
{
	int ret =0;
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		if(pModule->GetModuleName()==name)
		{
			ret = 1;
			break;
		}
	}
	return ret;

}
//���ÿ��ģ��ļ���������,���벻��Ϊ�գ�Ϊ�յĻ�
void CCalcModel::CheckBlockCalcIntegrity(CStringArray& LogStrings)
{
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->CheckBlockCalcIntegrity(LogStrings);
	}
}
//��������˳�򣬽��ƶ�ģ���еĶ����λ��
void CCalcModel::AdjustModuleOrder()
{
	//1. ������ģ�����ʱ������ı�־ΪFALSE
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->m_bCompared = FALSE;
	}
   //2. ÿ������Сֵ
	int k=0;
	int max = m_ModuleList.GetCount();
	POSITION posFind;
	POSITION pos1;
	int t=0;
	int min;
	while(k<max)
	{
		t=0;
		posFind=NULL;
		pos = m_ModuleList.GetHeadPosition();
		pos1 = pos;
		while(pos)
		{
			CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
			if(pModule->m_bCompared ==FALSE)
			{
				if(t==0) //��һ��
				{
					t=1;
					min = pModule->GetCalcOrderID();
					posFind = pos1;
				}
				else //
				{
					if(pModule->GetCalcOrderID()<min)
					{
						posFind = pos1;
						min = pModule->GetCalcOrderID();
					}
				}
			}
			pos1 =pos;
		}
		//�ҵ���С��,Ȼ��������ж����ƶ������е�β��
		if(posFind!=NULL)
		{
			CCalcModule* pModule = (CCalcModule*)m_ModuleList.GetAt(posFind);
			pModule->m_bCompared = TRUE;
			m_ModuleList.RemoveAt(posFind);
			m_ModuleList.AddTail(pModule);
		}
		k++;
	}
}

void CCalcModel::AdjustCalcSequence()
{
	AdjustModuleOrder();
	
	//����ģ���еļ�����˳��
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->AdjustBlockCalcOrder();
	}

}
void CCalcModel::LoadRTDBDllAndCon()
{
	if(!m_bInitLoadAndConRTDB)
	{
		m_bInitLoadAndConRTDB=TRUE;
		m_bLoadRTDBDll=LoadRTDBDll();
		if(m_bLoadRTDBDll==FALSE)
		{
			AfxMessageBox("����RTDBInterface.dllʧ�ܣ������Ŀ¼����ļ���");
			return;
		}
		m_bConRTDB=InitRTDBConnect(hDLL);
		if(FALSE==m_bConRTDB)
		{
			AfxMessageBox("����ʵʱ��ʷ��ʧ�ܣ��������������Ϣ��");
			return;
		}
	}
}
BOOL CCalcModel::LoadRTDBDll()
{
	hDLL = ::LoadLibrary("RTDBInterface.dll");
	if(hDLL!=NULL)
		return TRUE;
	else
	{
		return FALSE;
	}
}
BOOL CCalcModel::InitRTDBConnect(HINSTANCE hDLL)
{
	char *strParameters [4];//����ʵʱ�����Ӳ���
	//�������ݿ�����
	BOOL bRet=TRUE;
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
		{
			bRet = FALSE;
		}
	}
	catch(CException *e)
	{
		e->Delete();
		bRet = FALSE;
	}
	if(strParameters!=NULL)
	{
		for(int i=0;i<4;i++)
		{
			free(strParameters[i]);
		}
	}
	return bRet;
}

void  CCalcModel::InitforCalc()
{
	m_iWAllCounter=0;
	m_iWCounter=0;
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->InitforCalc(this);
		m_iWAllCounter+=pModule->GetCounterByBType("WriteRTDB");
	}
	if(m_pInSertRtData!=NULL)
	{
		delete[] m_pInSertRtData;
		m_pInSertRtData=NULL;
	}
	if(m_pInSertRtDataBack!=NULL)
	{
		delete[] m_pInSertRtDataBack;
		m_pInSertRtDataBack=NULL;
	}
	m_pInSertRtData=new InsertData[m_iWAllCounter];
	memset(m_pInSertRtData,0,sizeof(InsertData)*m_iWAllCounter);
	m_pInSertRtDataBack=new InsertData[m_iWAllCounter];
	memset(m_pInSertRtDataBack,0,sizeof(InsertData)*m_iWAllCounter);
}
void  CCalcModel::ResetCalc()
{
	m_iWCounter=0;//��������λ
	memset(m_pInSertRtData,0,sizeof(InsertData)*m_iWAllCounter);
	memset(m_pInSertRtDataBack,0,sizeof(InsertData)*m_iWAllCounter);
}
void  CCalcModel::WriteDataToRTDB()
{
	if(m_iWCounter<=0)
		return;
	//��������д�빦��
	typedef int (*pAppendRTTagDataByBatch)(InsertData *pInsertData,int nCount);
	pAppendRTTagDataByBatch AppendRTTagDataByBatch=(pAppendRTTagDataByBatch)::GetProcAddress(this->hDLL,"AppendRTTagDataByBatch");

	memcpy(m_pInSertRtDataBack,m_pInSertRtData,m_iWCounter*sizeof(InsertData));
	int nRet = AppendRTTagDataByBatch(m_pInSertRtData,m_iWCounter);
	if (nRet != 0)//�쳣
	{
		Sleep(500);
		memcpy(m_pInSertRtData,m_pInSertRtDataBack,m_iWCounter*sizeof(InsertData));//�ָ����ݣ�����д�롣
		nRet = AppendRTTagDataByBatch(m_pInSertRtData,m_iWCounter);
		if (nRet != 0)//�쳣
		{
			CString str;
			str.Format("����д�����,����ret=%d",nRet);
			if(m_iDebugOn)//����ģʽ
				AfxMessageBox(str);
			else if(m_iTraceOn)//����ģʽ,��׷����Ϣ
			{
				if(m_pWnd!=NULL)
				{
					m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
					str="----------д��ǰ����----------";
					m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
					printDebugData(m_pInSertRtDataBack,m_iWCounter);
					str="----------д�������----------";
					m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
					printDebugData(m_pInSertRtData,m_iWCounter);
				}
			}
			if(m_iLogOnOrOff)
			{
				if(m_pLog!=NULL)
				{
					m_pLog->WriteFileData(str);
				}
			}
		}
	}
}
void  CCalcModel::printDebugData(InsertData *pData,int count)
{
	CString str;
	CString strTime;
	for(int i=0;i<count;i++)
	{
		CTime t(pData[i].time);
		strTime=t.Format("%Y-%m-%d %H:%M:%S"); 
		str.Format("LongID:%s---Value:%f---State:%d---Time:%s",pData[i].pointName,pData[i].value,pData[i].status,strTime);
		m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
		if(m_iLogOnOrOff)
		{
			if(m_pLog!=NULL)
			{
				m_pLog->WriteFileData(str);
			}
		}
	}
}
//Ӧ���Ǹ���ģ���˳����м��㣬ģ����һ��˳�����ԣ���С������м���
//�ټ���֮ǰ������˳���������n��������˳��ţ�����С����������м���
//�ȿ��ж��ٸ���Ȼ���ҵ����ʵģ���Ҫѭ����Σ���Խ��Խ�١�
//Ȼ�����λ�õĵ���
void  CCalcModel::Calc()
{
   POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->Calc();
		if(m_iStopTag)//for stop
		{
			this->ResetStopCalc();//ģ�͸�λ
			break;
		}
	}
	TRACE("Call model calc\n");
}
void CCalcModel::StopCalc()//for stop
{
	m_iStopTag=1;//ֹͣ����
}
void CCalcModel::ResetStopCalc()//ֹͣ��־��λ//for stop
{
	m_iStopTag=0;//��λ
}
//����ģ�͵����ֶ�ȡģ�͵�����
BOOL CCalcModel::ReadModelDesp(_ConnectionPtr &pCon)
{
	BOOL bRet=TRUE;
   CString strTb_ModelList="ModelList";
   /////////////////////////////////////////////////////////
   	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
   VARIANT_BOOL bBOF ;
   pRecord.CreateInstance(__uuidof(Recordset));
   //pRecord->CursorLocation = adUseClient;

   CString strSQL;
   strSQL.Format("select * from %s where model_name='%s' ",strTb_ModelList,this->m_strModelName);
   try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
   }
   	catch(_com_error e)
	{
		AfxMessageBox("��ģ��ģ���б����!");
		return FALSE;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	if(!pRecord->adoEOF )
	{
		CString strModelDesp;
		BOOL bThisOk = TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("model_desp");
			strModelDesp = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e)
		{
			//AfxMessageBox(e.Description());
			bThisOk = FALSE;
			bRet = FALSE;
			//break;
		}
		if(bThisOk)
			this->m_strModelDesp=strModelDesp;
	}
	pRecord->Close();
    return bRet;
}
//ģ�͵����ƾ��������õı�����
BOOL CCalcModel::ReadModel(_ConnectionPtr &pCon)
{
   BOOL bRet=TRUE;
   //������е�ģ�飬�����õ���
   CString strTb_ModuleList;
   strTb_ModuleList.Format("%s_modulelist",m_strModelName);
   /////////////////////////////////////////////////////////
   	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
   VARIANT_BOOL bBOF ;
   pRecord.CreateInstance(__uuidof(Recordset));
   //pRecord->CursorLocation = adUseClient;

   CString strSQL;
   strSQL.Format("select * from %s ",strTb_ModuleList);
   try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
   }
   	catch(_com_error e)
	{
		AfxMessageBox("��ģ��ģ���б����!");
		return FALSE;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	while(!pRecord->adoEOF )
	{
		CString strModuleName;
		CString strModuleDesp;
		long      lCalcOrder;
		BOOL bThisOk = TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("module_name");
			strModuleName = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("module_desp");
			strModuleDesp = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("calcindex");
			if(Tempvalue.vt !=VT_NULL)
			      lCalcOrder = long(Tempvalue);
			else
				lCalcOrder = -1;
		}
		catch(_com_error e)
		{
			//AfxMessageBox(e.Description());
			//bThisOk = FALSE;
			//bRet = FALSE;
			//break;
		}
		CCalcModule *pModule = new CCalcModule();
		pModule->SetModuleName(strModuleName);
		pModule->SetModuleDesp(strModuleDesp);
		pModule->SetParentName(&m_strModelName); //д�������ĸ�ģ��
		pModule->SetCalcOrderID(lCalcOrder);
		AddModule(pModule);
		///////////////////////////
		pRecord->MoveNext();
	}
	pRecord->Close();

    return bRet;
   
}

BOOL CCalcModel::LoadAllModuleBlocks(_ConnectionPtr &pCon)
{
	BOOL  bRet = TRUE;
    POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		BOOL bValid = pModule->CheckTableExist(pCon);
		if(bValid) 
		{
			pModule->ReadModule(pCon);
		}
	}
	return bRet;
}
int CCalcModel::GetModuleCount()
{
	return m_ModuleList.GetCount();
}

int CCalcModel::GetBlockCount()
{
	int sum =0;
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		sum+=pModule->GetBlockCount();	
	}
	return sum;
}
CCalcModule* CCalcModel::GetModuleByIndex(int index)
{
	CCalcModule* pModule= NULL;
	if((index>=0)&&(index<m_ModuleList.GetCount()))
	{
		POSITION pos = m_ModuleList.FindIndex(index);
		if(pos)  pModule =  (CCalcModule *)m_ModuleList.GetAt(pos);
	}
    return pModule;
}
CCalcModule* CCalcModel::FindModuleByName(CString str)
{
	CCalcModule* pModule= NULL;
    POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		if(str.Compare(pModule->GetModuleName())==0)
		{
			break;
		}
		else
		{
			pModule= NULL;
		}
	}
	return pModule;
}
CCalcBlock* CCalcModel::FindBlockByName(CString strModule,CString strBlockName)
{
	CCalcBlock* pBlock = NULL;
	CCalcModule* pModule= FindModuleByName(strModule);
	if(pModule!=NULL)
	{
		pBlock = pModule->FindBlockByName(strBlockName);
	}
	return pBlock;
}
CCalcBlock* CCalcModel::FindBlockByName(CString strKey)
{
	CCalcBlock* pBlock = NULL;

	CCalcModule* pModule= NULL;
    POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
	    pBlock = FindBlockByName(strKey);
		if( pBlock!=NULL)
			break; 
	}
	return pBlock;
}
//���ģ��
void CCalcModel::ResetModelContent()
{
	m_strModelDesp = _T("δ�����ģ��");
	m_strModelName = _T("empty");
	m_strModelIP = _T("empty");
   	while (!m_ModuleList.IsEmpty ())
	{
		delete m_ModuleList.RemoveHead ();
	}
	CloseAllSourceRDBCon();
}
void CCalcModel::ClearModuleBlocks()
{
	CCalcModule* pModule= NULL;
    POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->ClearAllBlocks();
	}
}


void CCalcModel::SetRunMode(int mode)
{
    m_CalcControlPara.RunMode = mode;
}

int CCalcModel::GetRunMode()
{
	return m_CalcControlPara.RunMode;
}
void  CCalcModel::SetRunState(int state)
{
	m_RunState = state;
}
int CCalcModel::GetRunState()
{
	return m_RunState;
}

BOOL CCalcModel::ExecuteBlock(CString strModuleName, CString strBlockName)
{
		BOOL bret=TRUE;
		CCalcBlock* pBlock = FindBlockByName(strModuleName,strBlockName);
		if(pBlock != NULL)
		{
			pBlock->Calc();
		}
		return bret;
}
//�����Ʋ�������ʼֵ
void CCalcModel::InitControlPara()
{
	//����ģʽ
	m_CalcControlPara.RunMode = RUN_MODE_REALTIME;
	//ʵʱ����Ĳ���
	m_CalcControlPara.RealCalc_Cycle=5; //5��
    //����Ǽƻ���Ĭ�ϲ���
	m_CalcControlPara.TaskFrequence =	RUN_TASKMODE_EVERYDAY;
	m_CalcControlPara.TaskDate =0;
	m_CalcControlPara.TaskTime = CTime(2010,1,1,1,0,0); //��ʹ��ʱ����
    m_CalcControlPara.Task_ForwardTimeCount = 1;
	//��ʷ����
	m_CalcControlPara.HistSpan_lTimeBegin =0;
	m_CalcControlPara.HistSpan_lTimeEnd =0;
	m_CalcControlPara.HistSpan_TimeMode = RUN_TIMEMODE_SPAN;
	m_CalcControlPara.HistSpan_TimeInterval =0; 
}

void CCalcModel::InitCompile(CWnd *pWnd,int iMsg,CLog *pLog)
{
	m_iModelWarningCount=0;
	m_iModelErrorCount=0;
	m_pWnd=pWnd;
	m_iMsg=iMsg;
	m_pLog=pLog;

	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->InitCompile(&m_iModelWarningCount,&m_iModelErrorCount,m_pWnd,m_iMsg,pLog);
	}
}

void CCalcModel::RunCompile()
{
	//�������������Ϣ
	CString str;
	str.Format("---------------------------- ģ�ͣ�%s ----------------------------",this->m_strModelName);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("��ʼ����...");
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	//
	bool beHave=false;
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->RunCompile();
		beHave=true;
	}
	if(!beHave)
	{
		str.Format("���棺��ģ����ģ�����.");
		m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
		m_iModelWarningCount++;
	}
	str.Format("---------------------------- ģ�ͣ�'%s'������ɣ� ----------------------------",this->m_strModelName);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("");
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("ģ�ͣ�'%s' -- ���������%d�����������%d",this->m_strModelName,m_iModelErrorCount,m_iModelWarningCount);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
}
void CCalcModel::SetDebugOnOrOff(int iOnOrOff)
{
	m_iDebugOn=iOnOrOff;//������־����
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->SetDebugOnOrOff(iOnOrOff);
	}
}
void CCalcModel::SetTraceOnOrOff(int iOnOrOff)
{
	m_iTraceOn=iOnOrOff;//������־����
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->SetTraceOnOrOff(iOnOrOff);
	}
}
void CCalcModel::SetWriteLogOnOrOff(int iOnOrOff)
{
	m_iLogOnOrOff=iOnOrOff;//������־����
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->SetWriteLogOnOrOff(iOnOrOff);
	}
}

void CCalcModel::GetBlockInputValueFromGlobalWS()//�ӹ����ռ���������˿�ֵ
{
	POSITION pos = m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule* pModule=  (CCalcModule*)m_ModuleList.GetNext(pos);
		pModule->GetBlockInputValueFromGlobalWS();
	}
}
void CCalcModel::SetCalcDataTime(long lCalcTime, long lDataSTime, long lDataETime)
{
	m_lDataTime=lCalcTime;
	m_lDataSTime=lDataSTime;
	m_lDataETime=lDataETime;
}

void CCalcModel::GetDateSETime(long &lSTime, long &lETime)
{
	lSTime=m_lDataSTime;
	lETime=m_lDataETime;
}

void CCalcModel::GetDataTheTime(long &lTime)
{
	lTime=m_lDataTime;
}
