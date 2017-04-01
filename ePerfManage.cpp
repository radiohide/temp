// ePerfManage.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ePerfManage.h"

#include "MainFrm.h"
#include "ePerfManageDoc.h"
#include "ePerfManageView.h"
//#include "ModuleListView.h"

#include "ThreadRunPara.h"
#include "CalcModel.h"
#include "ElementInOut.h"
#include "CalcPort.h"

#include "SocketManage.h"
#include "SocketServ.h"
#include "Log.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp


BEGIN_MESSAGE_MAP(CEPerfManageApp, CWinApp)
	//{{AFX_MSG_MAP(CEPerfManageApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEW_CLOSEPAGE, OnViewClosepage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLOSEPAGE, OnUpdateViewClosepage)
	ON_COMMAND(ID_VIEW_CLOSEALLPAGE, OnViewCloseallpage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLOSEALLPAGE, OnUpdateViewCloseallpage)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp construction

CEPerfManageApp::CEPerfManageApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    m_pView = NULL;
	m_bExistModelFrame = FALSE;

	m_pConLocalModel=NULL; //连接本地模型的连接
	m_pConRemoteModel=NULL; //连接远程模型的连接
	m_pConLib=NULL; //连接模块库的连接
	m_bModelRemoteConOK= FALSE;
	m_bLibConOK=FALSE;
	m_bModelLocalConOK=FALSE;

	m_pTreeItemObj = new CTreeItemObj("",1,"","",false);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEPerfManageApp object

CLog g_Log;//全局的输出日志对象
char g_sNewLog[256];	//新日志
CEPerfManageApp theApp;
CSocketManage *g_pSocketManage;
CSocketServ* g_pSocketLogServ;
CSocketServ* g_pSocketWorkSpaceServ;
int g_SocketServPort;
//线程控制全局变量

//CCalcModel*         g_pCalModel; //全局的模型指针

//定义全局的工作空间
CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace(4000);	//工作空间jj

CPtrArray ListPoteDll;
//定义全局的工作空间


//实时数据库调用各个函数
pInitConnect InitConnect;//建立链接
pReleaseConnect ReleaseConnect;//断开链接
pGetRTDataByTagName GetRTDataByTagName;//单点实时数据查询
pGetRTDataByBatch GetRTDataByBatch;//批量实时数据查询
pGetRawDataByTagName GetRawDataByTagName;//测点原始数据查询
pGetSnapshotDataByTagName GetSnapshotDataByTagName;//测点快照数据查询
pGetAggregationDataByTagName GetAggregationDataByTagName;//测点统计数据查询
pGetHistoryDataByTime GetHistoryDataByTime;//历史时刻数据查询
pAppendRTTagDataByTagName AppendRTTagDataByTagName;//单点数据写入功能
pAppendRTTagDataByBatch AppendRTTagDataByBatch;//批量数据写入功能
pRTDBFreePointer RTDBFreePointer;//释放连接功能，原始数据查询时使用
////////////////////////////////////////////////////////////////////////////////////////

bool IsDigit(char byte)
{
	if((byte <= '9')&&(byte >='0'))
		return true;
	else
		return false;
}
CString Substring(CString str,int pos1,int count)	//取出字符串
{
	CString strTemp = str.Right(str.GetLength()-pos1);
	CString strTemp2 = strTemp.Left(count);
	return strTemp2;
}
bool IsLetter(char byte)
{
	if(((byte >= 0x0041)&&(byte <= 0x005A))||((byte >= 0x0061)&&(byte <= 0x007A))||((byte >= 0x002B0)&&(byte <= 0x02C1))||((byte >= 0x05D0)&&(byte <= 0x05EA)))
		return true;
	else
		return false;
}
CString gGetExeFileDir();

void InitDllMapping()
{
	CString path= gGetExeFileDir() + "ePerfManage.CFG";

	char inBuf[80];
	int n = 0;
	int dllNum=0;
	CString strKey;
	char* cNodesName=new char[20];
	try
	{
		CPoteDll* PoteDll;
		
		n=GetPrivateProfileString ("DLL", "dll.fileNum", "0", inBuf, 80, path);
		dllNum=::atoi(inBuf);
		
		for (int i = 0; i<dllNum; i++)
		{
			CString str;	str.Format("%d",i+1);
			strKey = "dll.filename"+str;
			strcpy(cNodesName,strKey);
			n=GetPrivateProfileString ("DLL", cNodesName, NULL, inBuf, 80, path);
			strKey = CString(inBuf);
			strcpy(cNodesName,CString(inBuf));
			
			PoteDll = new CPoteDll(strKey);
			if( PoteDll->IsDllValidate(cNodesName)==TRUE )
			{
				PoteDll->LoadDll(cNodesName);
				ListPoteDll.Add(PoteDll);
			}
		}
		if (cNodesName) delete[] cNodesName;
		PoteDll =NULL;  
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

//运行由外部指定时间段的一次计算
DWORD WINAPI TRunUserSetting(LPVOID pthread)
{
	ThreadRunPara *runPara=(ThreadRunPara*)pthread;
    //////////////////////////////////////////////////////
	CString strOutPut;
	strOutPut.Format("计算用户指定历史时间段:%d-%d-%d %d:%d:%d to %d-%d-%d %d:%d:%d 间隔:%d秒",
		runPara->startTime.GetYear(),runPara->startTime.GetMonth(),runPara->startTime.GetDay(),
		runPara->startTime.GetHour(),runPara->startTime.GetMinute(),runPara->startTime.GetSecond(),
		runPara->endTime.GetYear(),runPara->endTime.GetMonth(),runPara->endTime.GetDay(),
		runPara->endTime.GetHour(),runPara->endTime.GetMinute(),runPara->endTime.GetSecond(),
		runPara->TimeInterval);

	::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
	//----------------------------------------------------------------------
	//runPara->Calc();
	CTimeSpan ts =CTimeSpan(0,0,0,runPara->TimeInterval);
	CTime CalcTime=runPara->startTime;
	CTime CalcTimeE=runPara->startTime;
	CTime CalcTimeS=runPara->startTime-ts;
	bool bRunTag=true;
	do
	{
		strOutPut.Format("本次计算时刻值:%d/%d/%d %d:%d:%d;时段值:%d-%d-%d %d:%d:%d to %d-%d-%d %d:%d:%d",
			CalcTime.GetYear(),CalcTime.GetMonth(),CalcTime.GetDay(),CalcTime.GetHour(),CalcTime.GetMinute(),CalcTime.GetSecond(),
			CalcTimeS.GetYear(),CalcTimeS.GetMonth(),CalcTimeS.GetDay(),CalcTimeS.GetHour(),CalcTimeS.GetMinute(),CalcTimeS.GetSecond(),
			CalcTimeE.GetYear(),CalcTimeE.GetMonth(),CalcTimeE.GetDay(),CalcTimeE.GetHour(),CalcTimeE.GetMinute(),CalcTimeE.GetSecond()
			);
		::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
		
		runPara->m_pModel->SetCalcDataTime(CalcTime.GetTime(),CalcTimeS.GetTime(),CalcTimeE.GetTime()); 
		runPara->m_pModel->ResetCalc();
		runPara->m_pModel->Calc();
		runPara->m_pModel->WriteDataToRTDB();
		CalcTime = CalcTime + ts;
		CalcTimeE = CalcTime;
		CalcTimeS = CalcTimeE - ts ;
		theApp.m_pView->PostMessage(WM_TASKGUID,30);
		if(CalcTime.GetTime()>runPara->endTime.GetTime())
		{
			strOutPut.Format("计算用户指定时间段计算完毕!");
			::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
			break;
		}
	}while(bRunTag);
	
	//----------------------------------------------------------------------
	//strOutPut.Format("计算用户指定时间段计算完毕!");
	//::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
	//////////////////////////////////////////////////////
    return 1;
}

//线程入口函数，并在线程中完成定时的操作,每次计算应该修改模型本身的时间属性
DWORD WINAPI TRunInCycle(LPVOID pthread)					
{
	ThreadRunPara *runPara=(ThreadRunPara*)pthread;
	HANDLE hTime = CreateWaitableTimer(NULL, TRUE, NULL ); //创建等待的时钟
	if ( NULL == hTime )  	return -1;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart=runPara->intCycle*-10000000; 
	if ( !SetWaitableTimer(hTime, &liDueTime, 0, NULL, NULL, FALSE))  return -1;
	//显示信息
	CString strOutPut;
	CTimeSpan ts ;
    CTime CalcTime;
	CTime CalcTimeE;
	CTime CalcTimeS;
	//开始进行计算
	if(runPara->CalcMode == 0)
	{
		
	}
	else if(runPara->CalcMode == 1)
	{
		ts = CTimeSpan(0,0,0,runPara->TimeInterval);
		CalcTime = runPara->startTime ;
		CalcTimeE = CalcTime ;
		CalcTimeS = CalcTimeE - ts ;
	}
	while(1)
	{
		if ( WaitForSingleObject(hTime, INFINITE) != WAIT_OBJECT_0 )
		{
			return -1;
		}
		else//时间到，开始一次计算
		{
			CTime time1 = CTime::GetCurrentTime();  
            //===========================================================
			
			if(runPara->CalcMode == 0) //实时
			{
				CalcTime = time1;
				strOutPut.Format("本次计算时刻值:%d/%d/%d %d:%d:%d",
					CalcTime.GetYear(),CalcTime.GetMonth(),CalcTime.GetDay(),CalcTime.GetHour(),CalcTime.GetMinute(),CalcTime.GetSecond()
					);
				::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
				runPara->m_pModel->SetCalcDataTime(CalcTime.GetTime(),CalcTimeS.GetTime(),CalcTimeE.GetTime()); 
				runPara->m_pModel->ResetCalc();
				runPara->m_pModel->Calc();
				runPara->m_pModel->WriteDataToRTDB();
				
				theApp.m_pView->PostMessage(WM_TASKGUID,30);
			}
			else if(runPara->CalcMode == 1)
			{
				if(CalcTime<runPara->endTime)
				{
					//本次计算的
					strOutPut.Format("本次计算时刻值:%d/%d/%d %d:%d:%d;时段值:%d-%d-%d %d:%d:%d to %d-%d-%d %d:%d:%d",
						CalcTime.GetYear(),CalcTime.GetMonth(),CalcTime.GetDay(),CalcTime.GetHour(),CalcTime.GetMinute(),CalcTime.GetSecond(),
						CalcTimeS.GetYear(),CalcTimeS.GetMonth(),CalcTimeS.GetDay(),CalcTimeS.GetHour(),CalcTimeS.GetMinute(),CalcTimeS.GetSecond(),
						CalcTimeE.GetYear(),CalcTimeE.GetMonth(),CalcTimeE.GetDay(),CalcTimeE.GetHour(),CalcTimeE.GetMinute(),CalcTimeE.GetSecond()
						);
					::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
					runPara->m_pModel->SetCalcDataTime(CalcTime.GetTime(),CalcTimeS.GetTime(),CalcTimeE.GetTime()); 
					runPara->m_pModel->ResetCalc();
					runPara->m_pModel->Calc();
					runPara->m_pModel->WriteDataToRTDB();
					
					CalcTime = CalcTime + ts;
					CalcTimeE = CalcTime;
					CalcTimeS = CalcTimeE - ts ;
					theApp.m_pView->PostMessage(WM_TASKGUID,30);
					if(CalcTime.GetTime()>runPara->endTime.GetTime())
					{
						strOutPut.Format("历史仿真计算完毕!");
						::PostMessage(runPara->m_hWnd,WM_MSG_MODELRUN_STATUS, 0, (LPARAM) AllocBuffer(strOutPut));
						break;
					}
				}
			}
			//===========================================================
			CTime time2 = CTime::GetCurrentTime();
			CTimeSpan ts = time2 - time1; // Subtract 2 Ctimes 
			int totalSeconds=ts.GetTotalSeconds();
			liDueTime.QuadPart=runPara->intCycle*-10000000+totalSeconds*10000000;
			if(liDueTime.QuadPart>0)
				liDueTime.QuadPart=0;
			SetWaitableTimer(hTime, &liDueTime, 0, NULL, NULL, FALSE);
		}
	}
	CloseHandle(hTime);
	return true;
}
///////////////////////////////////////////////////////////////////////////////////
BOOL gIsTableExist(_ConnectionPtr& pConnection, CString strTableName)
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

BOOL gIsTableExistInMDB(_ConnectionPtr& pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;

	strSQL.Format("select top 1 * from %s",strTableName);
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


CString gGetExeFileDir()
{
	TCHAR exeFullpath[200];
	GetModuleFileName(NULL,exeFullpath,200);
	CString strExeFile = exeFullpath ;
	CString strExeDir ;
    strExeFile.TrimLeft() ;
	strExeFile.TrimRight();
	CString strAppName = theApp.m_pszExeName; 
	int cnt = strExeFile.GetLength()-strAppName.GetLength()-4;
    strExeDir = strExeFile.Left(cnt);
    return strExeDir;

}
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp initialization
bool g_SocketInit(CSocketManage *&pSocket,const char* IP,int port)
{
	pSocket = new CSocketManage();

	//char address[30] = {"127.0.0.1"};

	sockaddr_in addrSrv;
	addrSrv.sin_family = AF_INET;
	//addrSrv.sin_addr.s_addr = inet_addr(address);
	addrSrv.sin_addr.s_addr = inet_addr(IP);
	//addrSrv.sin_port = htons(11650);
	addrSrv.sin_port = htons(port);

	pSocket->Create(0,SOCK_STREAM);

	BOOL bOptVal = true;
	pSocket->SetSockOpt(SO_REUSEADDR,(void*)&bOptVal,sizeof(BOOL),SOL_SOCKET);

	linger iLinger;
	iLinger.l_linger = 0;
	iLinger.l_onoff = 1;
	pSocket->SetSockOpt(SO_LINGER, (char*)&iLinger, sizeof(iLinger),SOL_SOCKET);

	BOOL ret = pSocket->Connect((SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if(!ret)
	{
		DWORD error = ::GetLastError();
		TRACE("not connected");
		return false;
	}
	return true;
}
bool g_SocketManageInit(const char* IP,int port)
{
	return g_SocketInit(g_pSocketManage,IP,port);
}
void g_SocketExit(CSocketManage *&pSocket)
{
	if(pSocket)
	{
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
	}
}
void g_SocketManageExit()
{
	g_SocketExit(g_pSocketManage);
}

BOOL CEPerfManageApp::InitInstance()
{
   	if (!AfxOleInit())
	{
		AfxMessageBox("ole初始化失败!");
		return FALSE;
	}
	if ( FAILED(::CoInitialize(NULL) ) )//用来告诉 Windows以单线程的方式创建com对象
      return FALSE;
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));
   	////////////////////////////////////////
	m_bModelRemoteConOK=EstablishRemoteModelConnect();
	m_bLibConOK =  EstablishLibaryConnect();
	m_bModelLocalConOK= EstablishLocalModelConnect();
    
	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEPerfManageDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEPerfManageView));
		//RUNTIME_CLASS(CModuleListView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	((CMainFrame*)AfxGetMainWnd())->SetWindowText("VeStore-DisCalc");

	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();


	CMainFrame* hMainWnd = (CMainFrame*)GetMainWnd();
	if(hMainWnd)
	{
		hMainWnd->RefreshModelTree(NULL);
		CView* pView = hMainWnd->GetActiveView();
		if(pView)
		{
			m_pView = pView;
		}
	}
	AfxSocketInit();
	InitDllMapping();
	
	//初始化日志
	char pPath[512];memset(pPath,0,sizeof(pPath));
	char pFileName[512];memset(pFileName,0,sizeof(pFileName));
	::GetCurrentDirectory(sizeof(pPath),pPath);
	strcpy(pFileName,"");//pFileName,是年文件夹下再建立的文件夹
	g_Log.InitLog(pPath,pFileName,2);
	//初始化日志
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp message handlers

int CEPerfManageApp::ExitInstance() 
{
	delete m_pTreeItemObj;

	//删除线程信息对象
	if(g_pThreadPara!=NULL) delete g_pThreadPara;

	//关闭和数据库的连接
	CloseConnectPrt(m_pConRemoteModel);
	CloseConnectPrt(m_pConLocalModel);
	CloseConnectPrt(m_pConLib);


	for (int i=0; i<(ListPoteDll).GetSize(); i++)
	{
		CPoteDll* PoteDll = (CPoteDll*)((ListPoteDll)[i]);
		if(PoteDll!=NULL)
		{
			delete PoteDll;
			PoteDll=NULL;
		}
	}
	ListPoteDll.RemoveAll();
	
	BCGCBProCleanUp();

	g_SocketManageExit();
	AfxSocketTerm();
	
	return CWinApp::ExitInstance();
}

void CEPerfManageApp::SetActiveFrame (OUTLOOK_FRAME frame)
{
	CString strOldRegPath = GetRegSectionPath ();

	// Save user tools:
	if (GetUserToolsManager () != NULL)
	{
		GetUserToolsManager ()->SaveState (GetRegSectionPath ());
	}

	switch (frame)
	{
	case MainFrame:
		SetRegistryBase (_T("Main Frame Settings"));
		break;

	case ModelFrame:
		SetRegistryBase (_T("Model Settings"));
		break;

	default:
		ASSERT (FALSE);
	}

	// Load user tools:
	if (GetUserToolsManager () != NULL)
	{
		GetUserToolsManager ()->LoadState (strOldRegPath);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton	m_btnURL;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEPerfManageApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CEPerfManageApp message handlers

void CEPerfManageApp::OnFileNew() 
{
	// TODO: Add your command handler code here

	CWinApp::OnFileNew();
}
void CEPerfManageApp::OnFileOpen()
{
	CWinApp::OnFileOpen();
}
//建立模型的连接，远程模型
BOOL CEPerfManageApp::EstablishRemoteModelConnect()
{
	BOOL bResult = FALSE;
	///////////////////////////
	CString path;
	CString strItemValue;
	path= gGetExeFileDir() + "ePerfManage.CFG";
	

	CString databaseip;
	CString databasename;
	CString username;
	CString pass;
	
	::GetPrivateProfileString("MYSQLCFG","dbIP","NOSET",databaseip.GetBuffer(255),255,path);
	databaseip.TrimLeft();databaseip.TrimRight();
	   
	::GetPrivateProfileString("MYSQLCFG","dbName","NOSET",databasename.GetBuffer(255),255,path);
	databasename.TrimLeft();databasename.TrimRight();
	 
	::GetPrivateProfileString("MYSQLCFG","dbUser ","NOSET",username.GetBuffer(255),255,path);
	username.TrimLeft();username.TrimRight();
	 
	::GetPrivateProfileString("MYSQLCFG","dbPass","NOSET",pass.GetBuffer(255),255,path);
	pass.TrimLeft();pass.TrimRight();
	 
	bResult =EstablishMySQLConnectPtr(m_pConRemoteModel,databaseip,databasename,username,pass);
	//AfxMessageBox("a");
	 
	return bResult;
}
//建立模型的连接，本地模型
BOOL CEPerfManageApp::EstablishLocalModelConnect()
{
	BOOL bResult = FALSE;
	///////////////////////////
	CString path;
	CString strItemValue;
	path= gGetExeFileDir() + "ePerfManage.CFG";
	
	CString strFile;
	CString strFullFile;
	::GetPrivateProfileString("MDBCFG","FileName","model.mdb",strFile.GetBuffer(255),255,path);
	strFile.TrimLeft();strFile.TrimRight();
	strFullFile.Format("%smodel\\%s",gGetExeFileDir(),strFile);
	bResult = EstablishMDBConnectPtr(m_pConLocalModel,strFullFile);
	return bResult;
}
//建立和本地的库文件的连接
BOOL CEPerfManageApp::EstablishLibaryConnect()
{
   CString strLibFile;
   strLibFile.Format("%slib\\blocklib.mdb",gGetExeFileDir());
   BOOL bOK = EstablishMDBConnectPtr(m_pConLib,strLibFile);
   return bOK;
}
BOOL CEPerfManageApp::EstablishMySQLConnectPtr(_ConnectionPtr &pCon,CString strDBip,CString strDBname,CString strUsername,CString strPassword)
{
	BOOL bResult = FALSE;
	try
	{
		HRESULT hr;
		hr =pCon.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			CString strsql;
			strsql.Format("Driver={MySQL ODBC 3.51 Driver};Server=%s;Database=%s;User=%s;Password=%s;Option=3",strDBip,strDBname,strUsername,strPassword);
			hr =pCon->Open(_bstr_t(strsql), "", "", adModeUnknown);
			//pCon-> Execute("SET   NAMES   gbk",&vtMissing,adCmdText);
			pCon-> Execute("SET   NAMES   gbk",NULL,adCmdText);
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format("连接MYSQL数据库错误:%s",e.ErrorMessage());
	    AfxMessageBox(temp);
	}
	catch(CException *e)
	{
		TCHAR   szError[1024];
		e->GetErrorMessage(szError,1024);
	    AfxMessageBox(szError);
	}
	return bResult;
}
BOOL CEPerfManageApp::EstablishMDBConnectPtr(_ConnectionPtr &pCon,CString strFileName)
{
	BOOL bResult = FALSE;
	try
	{
		HRESULT hr;
		hr =pCon.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			CString strSqlConn;
			strSqlConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+strFileName;
			hr =pCon->Open(_bstr_t(strSqlConn), "", "", adModeUnknown);
			//hr =pCon->Open(_bstr_t(strSqlConn), "", "", NULL);
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format("连接MDB数据库错误:%s",e.ErrorMessage());
	    AfxMessageBox(temp);
	}
	return bResult;
}

void CEPerfManageApp::CloseConnectPrt(_ConnectionPtr &pCon)
{
	if(pCon==NULL)
		return;
	if(pCon->State)
		pCon->Close();
}

void CEPerfManageApp::OnViewClosepage() 
{
	// TODO: Add your command handler code here
	if(m_pView != NULL)
	{
		m_pView->PostMessage(WM_TASKGUID_CLOSE_CURRENT_TABVIEW,0);
	}
}

void CEPerfManageApp::OnUpdateViewClosepage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pView != NULL)
	{
		if(m_pView->IsKindOf(RUNTIME_CLASS(CEPerfManageView)))
		{
			CEPerfManageView* pMainView = (CEPerfManageView*)m_pView;
	        pCmdUI->Enable(pMainView->GetTabShowCount()>0);
		}
	}
}
void CEPerfManageApp::OnViewCloseallpage() 
{
	// TODO: Add your command handler code here
	if(m_pView != NULL)
	{
		m_pView->PostMessage(WM_TASKGUID_CLOSE_ALL_TABVIEW,0);
	}
}

void CEPerfManageApp::OnUpdateViewCloseallpage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pView != NULL)
	{
		if(m_pView->IsKindOf(RUNTIME_CLASS(CEPerfManageView)))
		{
			CEPerfManageView* pMainView = (CEPerfManageView*)m_pView;
	        pCmdUI->Enable(pMainView->GetTabShowCount()>1);
		}
	}
}
