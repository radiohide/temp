// CalcServ.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CalcServ.h"
#include "CalcServDlg.h"
#include "SocketListen.h"
#include "ModelCalcControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcServApp

BEGIN_MESSAGE_MAP(CCalcServApp, CWinApp)
	//{{AFX_MSG_MAP(CCalcServApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcServApp construction

CCalcServApp::CCalcServApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCalcServApp object

CCalcServApp theApp;
extern int g_port;
extern CString g_sNodeName;
/////////////////////////////////////////////////////////////////////////////
// CCalcServApp initialization
extern CSocketListen * g_socketListen;
extern CString gGetExeFileDir();
extern ModelCalcControl *m_pMCalControl;
extern CCalcModel  *m_pModel;
extern CLog g_Log;//全局的输出日志对象
extern unsigned long m_lRunCounter;//模型执行的次数
extern void g_DelObj();
extern CString gGetExeFileDir();
BOOL CCalcServApp::InitInstance()
{
	m_pMCalControl=NULL;
	m_pModel=NULL;
	g_socketListen=NULL;

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

	AfxInitRichEdit();

	char pBuf[256];
	strcpy(pBuf,GetCommandLine());
	CString strTemp(pBuf);
	//AfxMessageBox(strTemp);
	int indexTag=strTemp.Find('#');
	if(indexTag<0)
	{
		AfxMessageBox("启动参数不正确！请通过管理工具或远程启动！");
		return false;//debug 完了拿开注释
	}
	CString str = strTemp.Left(strTemp.Find('#'));
	CString str2 = strTemp.Right(strTemp.GetLength() - str.GetLength() -1);

	g_port = ::atoi(str);//debug 完了拿开注释
	g_sNodeName = str2;//debug 完了拿开注释

	//g_port = 1024;
	//g_sNodeName = "TestMonth";
	
	m_lRunCounter=0;//模型执行计数器归零
	AfxSocketInit();
	//网络通信包启动
	int ret;
	g_socketListen = new CSocketListen();
	//ret = g_socketListen->Create(g_port,SOCK_STREAM);
	ret = g_socketListen->Socket();

	BOOL bOptVal = true;
	g_socketListen->SetSockOpt(SO_REUSEADDR,(void*)&bOptVal,sizeof(BOOL),SOL_SOCKET);

	linger iLinger;
	iLinger.l_linger = 0;
	iLinger.l_onoff = 1;
	g_socketListen->SetSockOpt(SO_LINGER, (char*)&iLinger, sizeof(iLinger),SOL_SOCKET);

	g_socketListen->Bind(g_port);
	ret = g_socketListen->Listen();
	
	//初始化log相关信息
	char pPath[512];memset(pPath,0,sizeof(pPath));
	char pFileName[512];memset(pFileName,0,sizeof(pFileName));
	//::GetCurrentDirectory(sizeof(pPath),pPath);
	CString strPath=gGetExeFileDir();
	strcpy(pPath,strPath);
 	strcpy(pFileName,g_sNodeName);//pFileName,是年文件夹下再建立的文件夹
	g_Log.InitLog(pPath,pFileName,400);
	
	
	CCalcServDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCalcServApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxSocketTerm();
	return CWinApp::ExitInstance();
}

BOOL CCalcServApp::EstablishMySQLConnectPtr(_ConnectionPtr &pCon,CString strDBip,CString strDBname,CString strUsername,CString strPassword)
{
	BOOL bResult = FALSE;
	try
	{	
		HRESULT hr;
		::CoInitialize(NULL);//2014-3-26 rao new 
		hr =pCon.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			CString strsql;
			strsql.Format("Driver={MySQL ODBC 3.51 Driver};Server=%s;Database=%s;User=%s;Password=%s;Option=3",strDBip,strDBname,strUsername,strPassword);
			hr =pCon->Open(_bstr_t(strsql), "", "", adModeUnknown);
			//pCon-> Execute("SET   NAMES   gbk",&vtMissing,adCmdText);
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format("连接MYSQL数据库错误:%s",e.ErrorMessage());
	    //AfxMessageBox(temp);
		return bResult;
	}
	catch(...)
	{
		return bResult;
	}
	return bResult;
}
BOOL CCalcServApp::EstablishMDBConnectPtr(_ConnectionPtr &pCon,CString strFileName)
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
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		pCon=NULL;
		CString temp;
		temp.Format("连接MDB数据库错误:%s",e.ErrorMessage());
	    AfxMessageBox(temp);
	}
	return bResult;
}

void CCalcServApp::CloseConnectPrt(_ConnectionPtr &pCon)
{
	if(pCon==NULL)
		return;
	if(pCon->State)
		pCon->Close();
}
