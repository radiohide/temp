// ePerfManageDoc.cpp : implementation of the CEPerfManageDoc class
//

#include "stdafx.h"
#include "ePerfManage.h"

#include "ePerfManageDoc.h"
#include "MainFrm.h"

#include "ePerfManageView.h"

#include "ModelFrame.h"
#include "ModelRunView.h"
#include "ViewModuleFrm.h"

#include "DlgAddModule.h"
#include "DlgNewModel.h"
#include "DlgOpenModel.h"
#include "DlgHistTimeSet.h"
#include "DlgDelModel.h"

#include "DBOperator.h"
#include "DlgModeRunCtrl.h"
#include "DlgCfgModuleOrder.h"
#include "DlgDepModel.h"
#include "DlgSourceRDB.h"

#include "SocketManage.h"
#include "SocketServ.h"
#include "PortTransferData.h"
#include "DlgModelSynNote.h"
#include "DlgProgress.h"


#include "DlgLocalRunSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool g_SocketManageInit(const char* IP,int port);
extern void g_SocketManageExit();
extern CSocketManage* g_pSocketManage;
extern CSocketServ* g_pSocketLogServ;
extern CSocketServ* g_pSocketWorkSpaceServ;
extern int g_SocketServPort;

extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
extern CLog g_Log;//全局的输出日志对象;

extern DWORD WINAPI TRunInCycle(LPVOID pthread);
extern DWORD WINAPI TRunUserSetting(LPVOID pthread);

enum RunCommand{Start=1,Stop,Pause,Resume,Kill};
enum RunState{Unknown=-1,Running=1,Starting,Stopped,Stopping,Paused,StartFail};

extern BOOL g_ReceivePacketsToCharArr(CSocketServ *pSockServ,char *pFirst,int iFirstCount,CArray<char,char> &bufArr);
extern void g_PutCharArrToWSpace(const CArray<char,char> &bufArr);

BOOL g_bToExit = false;	//退出信号
HANDLE g_RunEvent;	//挂起时间
HANDLE hThread;

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageDoc
void g_ClearWorkSpaceElement()
{
	POSITION pos = g_WorkSpace.GetStartPosition();
    while(pos)
    {
		CCalcPort* pPortSame=NULL;
		CString strKey;
		CCalcPort* pPort=NULL;
        g_WorkSpace.GetNextAssoc(pos, strKey, pPort);
		if(pPort!=NULL)
		{
			pPortSame=pPort;
			delete pPort;
			pPort=NULL;
		}
		g_WorkSpace.RemoveKey(strKey);
		POSITION pos1 = g_WorkSpace.GetStartPosition();
		while(pos1)//删除不同Key而元素相同的元素。
		{
			g_WorkSpace.GetNextAssoc(pos1, strKey, pPort);
			if(pPort==pPortSame)
			{
				g_WorkSpace.RemoveKey(strKey);
			}
		}
	}
	g_WorkSpace.RemoveAll();
}
CSocketServ* g_ServSocketInit(const char* IP,int port)
{
	CSocketServ* pSockServ = new CSocketServ();
	//char address[30] = {"127.0.0.1"};

	sockaddr_in addrSrv;
	addrSrv.sin_family = AF_INET;
	//addrSrv.sin_addr.s_addr = inet_addr(address);
	addrSrv.sin_addr.s_addr = inet_addr(IP);
	//addrSrv.sin_port = htons(::atoi(lbuf+6));
	addrSrv.sin_port = htons(port);

	pSockServ->Create(0,SOCK_STREAM);

	BOOL bOptVal = true;
	pSockServ->SetSockOpt(SO_REUSEADDR,(void*)&bOptVal,sizeof(BOOL),SOL_SOCKET);

	linger iLinger;
	iLinger.l_linger = 0;
	iLinger.l_onoff = 1;
	pSockServ->SetSockOpt(SO_LINGER, (char*)&iLinger, sizeof(iLinger),SOL_SOCKET);

	int count = 0;
	while(1)
	{
		BOOL ret2 = pSockServ->Connect((SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
		if(!ret2)
		{
			DWORD error = ::GetLastError();
			TRACE("not connected");
			if(count++ > 1)
				return NULL;
		}
		else
			break;
	}
	return pSockServ;
}
void g_ServSocketExit(CSocketServ*& pSockServ)
{
	if(pSockServ)
	{
		pSockServ->Close();
		delete pSockServ;
		pSockServ = NULL;
	}
}

IMPLEMENT_DYNCREATE(CEPerfManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CEPerfManageDoc, CDocument)
	//{{AFX_MSG_MAP(CEPerfManageDoc)
	ON_COMMAND(ID_FILE_NEWMODEL, OnFileNewmodel)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWMODEL, OnUpdateFileNewmodel)
	ON_COMMAND(ID_CFG_ADD_MODULE, OnCfgAddModule)
	ON_UPDATE_COMMAND_UI(ID_CFG_ADD_MODULE, OnUpdateCfgAddModule)
	ON_UPDATE_COMMAND_UI(ID_CFG_ADD_BLOCK, OnUpdateCfgAddBlock)
	ON_COMMAND(ID_MODULE_ADDBLOCK, OnModuleAddblock)
	ON_UPDATE_COMMAND_UI(ID_MODULE_ADDBLOCK, OnUpdateModuleAddblock)
	ON_COMMAND(ID_FILE_OPEN_MODEL, OnFileOpenModel)
	ON_COMMAND(ID_FILE_SAVEMODEL, OnFileSavemodel)
	ON_COMMAND(ID_RUN_COMPILE, OnRunCompile)
	ON_UPDATE_COMMAND_UI(ID_RUN_COMPILE, OnUpdateRunCompile)
	ON_COMMAND(ID_RUN_DEBUG, OnRunDebug)
	ON_COMMAND(ID_CFG_MODULEORDER, OnCfgModuleorder)
	ON_COMMAND(ID_MAG_RUN, OnMagRun)
	ON_UPDATE_COMMAND_UI(ID_MAG_RUN, OnUpdateMagRun)
	ON_COMMAND(ID_MAG_STOP, OnMagStop)
	ON_UPDATE_COMMAND_UI(ID_MAG_STOP, OnUpdateMagStop)
	ON_COMMAND(ID_MAG_PAUSE, OnMagPause)
	ON_UPDATE_COMMAND_UI(ID_MAG_PAUSE, OnUpdateMagPause)
	ON_COMMAND(ID_MAG_RESUME, OnMagResume)
	ON_UPDATE_COMMAND_UI(ID_MAG_RESUME, OnUpdateMagResume)
	ON_COMMAND(ID_FILE_UPLOADMODEL, OnFileUploadmodel)
	ON_UPDATE_COMMAND_UI(ID_FILE_UPLOADMODEL, OnUpdateFileUploadmodel)
	ON_COMMAND(ID_MAG_REFRESHWS, OnMagRefreshws)
	ON_UPDATE_COMMAND_UI(ID_MAG_REFRESHWS, OnUpdateMagRefreshws)
	ON_COMMAND(ID_CFG_ADD_BLOCK, OnCfgAddBlock)
	ON_COMMAND(ID_CFG_MODERUNCONTROL, OnCfgModeRunControl)
	ON_UPDATE_COMMAND_UI(ID_CFG_MODERUNCONTROL, OnUpdateCfgModeRunControl)
	ON_COMMAND(ID_DEP_MODEL, OnDepModel)
	ON_COMMAND(ID_MAG_STARTGETLOG, OnMagStartGetLog)
	ON_UPDATE_COMMAND_UI(ID_MAG_STARTGETLOG, OnUpdateMagStartGetLog)
	ON_COMMAND(ID_MAG_ENDGETLOG, OnMagEndGetLog)
	ON_UPDATE_COMMAND_UI(ID_MAG_ENDGETLOG, OnUpdateMagEndGetLog)
	ON_COMMAND(ID_MAG_REFRESHWS_START, OnMagRefreshWSStart)
	ON_UPDATE_COMMAND_UI(ID_MAG_REFRESHWS_START, OnUpdateMagRefreshWSStart)
	ON_COMMAND(ID_MAG_REFRESHWS_END, OnMagRefreshWSEnd)
	ON_UPDATE_COMMAND_UI(ID_MAG_REFRESHWS_END, OnUpdateMagRefreshWSEnd)
	ON_COMMAND(ID_MAG_MODELSYN, OnMagModelSyn)
	ON_UPDATE_COMMAND_UI(ID_MAG_MODELSYN, OnUpdateMagModelSyn)
	ON_COMMAND(ID_TOOL_SOURCE_RDB, OnToolSourceRdb)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEMODEL, OnUpdateFileSavemodel)
	ON_COMMAND(ID_FILE_DEL_MODEL, OnFileDelModel)
	ON_COMMAND(ID_CFG_DEL_MODULE, OnCfgDelModule)
	ON_UPDATE_COMMAND_UI(ID_CFG_DEL_MODULE, OnUpdateCfgDelModule)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SOURCE_RDB, OnUpdateToolSourceRdb)
	ON_COMMAND(ID_RUN_LOCAL_START, OnRunLocalStart)
	ON_UPDATE_COMMAND_UI(ID_RUN_LOCAL_START, OnUpdateRunLocalStart)
	ON_COMMAND(ID_RUN_LOCAL_STOP, OnRunLocalStop)
	ON_UPDATE_COMMAND_UI(ID_RUN_LOCAL_STOP, OnUpdateRunLocalStop)
	ON_COMMAND(ID_CFG_FIND_BLOCK, OnCfgFindBlock)
	ON_UPDATE_COMMAND_UI(ID_CFG_FIND_BLOCK, OnUpdateCfgFindBlock)
	ON_COMMAND(ID_MAG_PORTVALUESHOW, OnMagPortValueShow)
	ON_COMMAND(ID_FILE_OPEN_ALLMODULE, OnFileOpenAllModule)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_ALLMODULE, OnUpdateFileOpenAllModule)
	ON_COMMAND(ID_FILE_CLOSE_ALLMODULE, OnFileCloseAllModule)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALLMODULE, OnUpdateFileCloseAllModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageDoc construction/destruction

CEPerfManageDoc::CEPerfManageDoc()
{
	// TODO: add one-time construction code here
	m_iRemoteModeState=-1;
	m_bTagGettingWS=false;
	m_bTagGettingLog=false;
   m_pModel = new CCalcModel();
   m_pMCalControl=NULL;
   g_RunEvent = CreateEvent(NULL,TRUE,TRUE,NULL);	

   m_pLocalThreadPara =new ThreadRunPara; //add at 2011-12-09
   pDlgFindBlock=NULL;
   m_bInLocalRun =FALSE; //add at 2011-12-09
   m_BShowPortValue=FALSE;
}

CEPerfManageDoc::~CEPerfManageDoc()
{
	if(m_pLocalThreadPara!=NULL) delete m_pLocalThreadPara; //add at 2011-12-09

	if(m_pModel!=NULL)
	{
		delete m_pModel;
		m_pModel=NULL;
	}
	if(m_pMCalControl!=NULL)
	{
		delete m_pMCalControl;
		m_pMCalControl=NULL;
	}
	if(pDlgFindBlock!=NULL)
	{
		delete pDlgFindBlock;
		pDlgFindBlock=NULL;
	}
}

BOOL CEPerfManageDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageDoc serialization

void CEPerfManageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageDoc diagnostics

#ifdef _DEBUG
void CEPerfManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEPerfManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEPerfManageDoc commands
void CEPerfManageDoc::OnToolSourceRdb() 
{
	// TODO: Add your command handler code here
	CDlgSourceRDB dlg;
	dlg.m_StrModelName=this->m_pModel->m_strModelName;
	dlg.DoModal();
}
void CEPerfManageDoc::OnUpdateToolSourceRdb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}

void CEPerfManageDoc::OnDepModel() 
{
	// TODO: Add your command handler code here
	if((!theApp.m_bModelLocalConOK)&&(theApp.m_bModelRemoteConOK))
	{
		AfxMessageBox("与模型数据的连接未建立成功，不能打开模型");
		return;
	}
	CDlgDepModel dlg;
	dlg.DoModal();
}

void CEPerfManageDoc::OnFileOpenModel()
{
   if((!theApp.m_bModelLocalConOK)&&(theApp.m_bModelRemoteConOK))
   {
	   AfxMessageBox("与模型数据的连接未建立成功，不能打开模型");
	   return;
   }
  
   ///////////////////////////////////////////////////////////////
   CDlgOpenModel dlg;
   if(dlg.DoModal()==IDOK)
   {
       ////////////////
	   //获得了选择的模型名字和描述，据此，可打开模型的模块列表以及对应的
	   //得到文档PDOC指针
	   theApp.m_pView->PostMessage(WM_TASKGUID,0);
	   m_pModel->ResetModelContent();
	   m_pModel->m_strModelName = dlg.m_strSelModelName;
	   m_pModel->m_strModelDesp = dlg.m_strSelModelDesp;
	   m_pModel->m_strModelIP = dlg.m_strSelModelIP;
	   m_pModel->m_strPort=dlg.m_strPort;
	   m_pModel->m_iTagModeLocalOrRemote=dlg.m_iTagModeLocalOrRemote;
	   if(m_pMCalControl!=NULL)
	   {
		   delete m_pMCalControl;
		   m_pMCalControl=NULL;
	   }
	   if(m_pMCalControl==NULL)
	   {
			m_pMCalControl= new ModelCalcControl(m_pModel->m_strModelName);
	   }
	   ModelData modelDataObj;
        //读取模型并形成对象
	   if(m_pModel->m_iTagModeLocalOrRemote==0)
	   {
		   m_pModel->ReadModel(theApp.m_pConLocalModel); //读取本地
		   m_pModel->EstablishSourceRDBConnect(theApp.m_pConLocalModel);//加载可用的数据源
		   m_pModel->ClearModuleBlocks();
		   m_pModel->LoadAllModuleBlocks(theApp.m_pConLocalModel); 
		   m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
		   m_pMCalControl->ReadCtrlData();
		   CString str;
		   str.Format("VeStore-DisCalc--%s--本地",m_pModel->m_strModelName);
		   ((CMainFrame*)AfxGetMainWnd())->SetWindowText(str);
	   }
	   else
	   {
		   ModelData modelDataLocal;//删除本地模型
		   modelDataLocal.ReadDate(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		   modelDataLocal.DelTheModelTables(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		   modelDataLocal.DelModelInModelListTable(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		  
		   modelDataObj.ReadDate(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		   modelDataObj.CreatTheModelTablesInMDB(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		   modelDataObj.WriteData(theApp.m_pConLocalModel,m_pModel->m_strModelName);
			
		   m_pMCalControl->SetConPtr(theApp.m_pConRemoteModel);
		   m_pMCalControl->ReadCtrlData();
		   m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
		   m_pMCalControl->SaveAllCtrlDataToDB();
		   
		   m_pModel->ReadModel(theApp.m_pConRemoteModel); //读取远程
		   m_pModel->EstablishSourceRDBConnect(theApp.m_pConRemoteModel);//加载可用的数据源
		   m_pModel->ClearModuleBlocks();
		   m_pModel->LoadAllModuleBlocks(theApp.m_pConRemoteModel); 

		   CString str;
		   str.Format("VeStore-DisCalc--%s--远程",m_pModel->m_strModelName);
		   ((CMainFrame*)AfxGetMainWnd())->SetWindowText(str);
		   	   
		   g_SocketManageExit();
		   if(!g_SocketManageInit(m_pModel->m_strModelIP,11650))
		   {
				AfxMessageBox("远程管理工具无法连接,将无法进行远程控制！");
		   }
		   m_iRemoteModeState=GetNodeState(g_pSocketManage,m_pModel->m_strModelName);
	   }
	   m_pModel->AdjustCalcSequence();//包括
	   g_WorkSpace.RemoveAll();  //清除工作空间中的变量//to do
	   m_pModel->InitforCalc();  //建立各计算块到内部和外部工作空间的变量映射关系公共变量池，可以查看变量的情况
	   m_pModel->SetDebugOnOrOff(1);//调试运行
       //更新树形显示
	   CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	   pMain->m_LastShortCutID = -1;

	   pMain->RefreshModelTree(m_pModel);

	   //CString strLogDebug;
	   //strLogDebug.Format("***********open***********");
	   //g_Log.WriteFileData(strLogDebug);
   }
   //////////////////////////////////////////////////////////////
}

/*
对于输入输出，如何存储，存储输入输出情况
有几个输入，每个输入
inputNum:3
outputNum:2
inputName:in1,in2,in3
outputName:out1,out2,out3
inputLinkString{BlockName1.out1,BlockName2,BlockName3}
*/
void CEPerfManageDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(theApp.m_pView !=NULL) theApp.m_pView=NULL;
	CDocument::OnCloseDocument();
}

void CEPerfManageDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(theApp.m_pView !=NULL)
	{
		theApp.m_pView->PostMessage(WM_TASKGUID,0);
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->m_LastShortCutID = -1;
	}

	//m_pModel->ResetModelContent();

	CDocument::DeleteContents();
}

BOOL CEPerfManageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

	AfxMessageBox("open");
	return TRUE;
}

void CEPerfManageDoc::OnUpdateFileNewmodel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(!theApp.m_bExistModelFrame);
}
void CEPerfManageDoc::OnFileNewmodel() 
{
	//新建一个计算模型，需要清空当前的模型信息，
	//输入模型的名称和描述，然后成为一个新的模型
	CDlgNewModel dlg;
	if(dlg.DoModal()==IDOK)
	{
		//检查有没有同名的模型,当没有的话加入到
		//检查有没有，如果有，则需要修改为另一个名称
		BOOL bHased = CDBOperator::CheckRecordExisted(theApp.m_pConLocalModel,_T("ModelList"),_T("model_name"),dlg.m_strName);
		if(bHased)
		{
			AfxMessageBox("已经有一个同名的模型存在，请选择其他名字.");
			return;
		}
		theApp.m_pView->PostMessage(WM_TASKGUID,0);//关闭当前打开模型
		m_pModel->m_iTagModeLocalOrRemote=0;//标记为本地模型
		//在模型表中加入该模型名称，并建立
		BOOL bOK = AddModelInfoToDB(theApp.m_pConLocalModel,dlg.m_strName,dlg.m_strDesp);  //写入模型表中
		//生成模型记录模块表,
		bOK =  CreateModuleListTable(theApp.m_pConLocalModel,dlg.m_strName);
		if(!bOK) 
		{
			AfxMessageBox("错误：不能创建相应存储表.");
			return;
		}
		
		m_pModel->ResetModelContent();
		m_pModel->m_strModelDesp = dlg.m_strDesp;
		m_pModel->m_strModelName = dlg.m_strName;
		
		if(m_pMCalControl==NULL)//创建运行控制数据并保存
		{
			m_pMCalControl= new ModelCalcControl(m_pModel->m_strModelName);
			m_pMCalControl->SetDefaulControlData();//设置默认控制方式
			m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
			m_pMCalControl->SaveAllCtrlDataToDB();
		}
		//CEPerfManageView* pView = GetMainView();
		//pView->CloseAllPages();
		
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		if(pMain!=NULL)
			pMain->RefreshModelTree(m_pModel);
		///////////////////////////////////////////////////////////
		//UpdateAllViews(NULL);
		//CString strLog;
	    //strLog.Format("***********New***********");
	    //g_Log.WriteFileData(strLog);
	}
}
//在当前的模型下，添加一个模块
void CEPerfManageDoc::OnCfgAddModule() 
{
	//定义模块名
	CDlgAddModule dlg;
	if(dlg.DoModal()==IDOK)
	{
	   //查找是否有模块的重名现象
		CString strModuleName = dlg.m_strName;
		CString strModuleDesp = dlg.m_strDesp;
		if(m_pModel->LookUpMoudle(strModuleName,strModuleDesp)==1)
		{
			AfxMessageBox("模块名称/描述重复，请重新定义模块名称或描述!");
			return;
		}
		else
		{
			//创建该模块表格来存储，因为打开时需要从中读取信息
			BOOL  bOK= CreateBlockListTable(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName);
			if(!bOK) 
			{
				AfxMessageBox("错误：建立计算块列表存储表!");
				return;
			}
			bOK= CreateBlockPropTable(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName);
			if(!bOK) 
			{
				AfxMessageBox("错误：建立计算块属性存储表!");
				return;
			}
			//////////////////////////////////////////////////////
			CCalcModule *pModule = new CCalcModule(); //新建一个模块对象
			pModule->SetModuleName(strModuleName);
			pModule->SetModuleDesp(strModuleDesp);
			pModule->SetParentName(&(m_pModel->m_strModelName)); //写上属于哪个模块
			m_pModel->AddModule(pModule);

			//将模块名写入表中
			AddModuleInfoToDB(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName,strModuleDesp);  //写入模型表中

			CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	        if(pMain!=NULL) pMain->RefreshModelTree(m_pModel);
			UpdateAllViews(NULL); 
		}

	}
	
}

void CEPerfManageDoc::OnUpdateCfgAddModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
	
}

CEPerfManageView* CEPerfManageDoc::GetMainView()
{
	CEPerfManageView* pView=NULL;
   POSITION pos = GetFirstViewPosition();
   while(pos)
   {
	   CView* pView1 = GetNextView(pos);
	   if(pView1->IsKindOf(RUNTIME_CLASS(CEPerfManageView)))
	   {
		   pView = (CEPerfManageView*)pView1;
		   break;
	   }
   }
   return pView;
}
//根据块的类型添加到模块中;
//是否可以：先生成图形对象，然后根据图形对象再生成模块对象，那么图形对象需要
//保存模块对象的属性才可以，不仅仅保存位置信息、连接信息
//图形信息：类型、名称、位置、输入、输出
//////////////////////////////////////////////////////////////////////////////////////////////////
/*视图与实际对象的属性传递、数值传递机制
1、当生成时，要获得默认的输入输出个数，端口的信息
2.当双击时，需要配置其属性；
3.当显示数值是，需要从端口中获得数据值
4.当将图形删除时，是否删除计算对象？保存时，需要根据；
5.当复制对象时，是否连接也复制呢？
5、当读出时，仅建立图形对象吗？
====================================
最好是：读入时，读取属性，可形成一个内嵌的对象来解决
*/
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEPerfManageDoc::AddLibBlock(CString strBlockType)
{
	BOOL bRet=TRUE;
	CString strModule;
	/////////////////////////////////////////////
	//：先检查活动的模块对象视图，如果没有要提示打开
	CEPerfManageView* pView = GetMainView();
	CView* pViewActvie = pView->GetActiveView();
	CViewModuleFrm* pModuleView = NULL;
	if(pViewActvie!=NULL && pViewActvie->IsKindOf(RUNTIME_CLASS(CViewModuleFrm))) 
	{
		 pModuleView = (CViewModuleFrm*)pViewActvie;
		 strModule = pModuleView->m_strModuleName ;
	}
	else
	{
		AfxMessageBox("请打开并切换到要添加计算块的模块视图！");
		return FALSE;
	}
	/////////////////////////////////////////////
	//寻找一个名字来代替
	CCalcModule* pModule = m_pModel->FindModuleByName(strModule);
	CString strDefultName = pModule->FindCanUseBlockName(strBlockType);
	//if(pModule==NULL) return FALSE;

	//CCalcBlock* pBlock = pModule->CreateBlock(strBlockType,strDefultName,strDefultName);
	//if(pBlock!=NULL)	pModule->AddBlock(pBlock);

	///////////////////////////////////////////
    //在视图中画一个对象
	///////////////////////////////////////////
	pModuleView->OnButtonBlock(strBlockType,strDefultName,strBlockType);
//////////////////////////////////////////////////////////////////////////////////////////////////////
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain!=NULL) pMain->RefreshModelTree(m_pModel);
	//UpdateAllViews(NULL,HINT_UPDATE_MODULE); 

	return bRet;
}
//添加计算块
void CEPerfManageDoc::OnModuleAddblock() 
{
	//弹出对话框选择哪种类型的模块，并定义名称，名称用英文来定，或用中文定
	
}

void CEPerfManageDoc::OnUpdateModuleAddblock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}

/*void CEPerfManageDoc::OnViewClosepage() 
{
	// TODO: Add your command handler code here
	CEPerfManageView* pView = GetMainView();
	if(pView!=NULL) 
	//	pView->CloseActivePage();
	
}

void CEPerfManageDoc::OnUpdateViewClosepage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/
//打开一个模型


//保存模型
void CEPerfManageDoc::OnFileSavemodel() 
{
	// TODO: Add your command handler code here
	if(theApp.m_pView!=NULL)
	{
		theApp.m_pView->PostMessage(WM_TASKGUID,4);
	}
}
void CEPerfManageDoc::OnUpdateFileSavemodel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}
//打开模型列表表，看是否有重复，无重复
BOOL CEPerfManageDoc::AddModelInfoToDB(_ConnectionPtr &pCon,CString strModelName,CString strModelDesp)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable=_T("ModelList");
	strSQL.Format("INSERT INTO %s(model_name,model_desp)values(\'%s\',\'%s\')",
		strTable,strModelName,strModelDesp);
    try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		return FALSE;
	}	
	return TRUE;
}
BOOL CEPerfManageDoc::DelModuleInfoFromDB(_ConnectionPtr &pCon,CString strModelName,CString strModuleName)
{
	CString strTable;
	strTable.Format("%s_ModuleList",strModelName);//从模型列表中删除模块信息。
	CString strSQL;
	strSQL.Format("delete from %s where module_name='%s'",strTable,strModuleName);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	   return TRUE;
	}
   	catch(_com_error e)
	{
		return FALSE;
	}
	//删除模块表和属性
	strTable.Format("%s_%s_BlockList",strModelName,strModuleName);
	CDBOperator::DelTable(pCon,strTable);
	strTable.Format("%s_%s_BlockProp",strModelName,strModuleName);
	CDBOperator::DelTable(pCon,strTable);
}
BOOL CEPerfManageDoc::AddModuleInfoToDB(_ConnectionPtr &pCon,CString strModelName,CString strModuleName,CString strModuleDesp)
{
	int moduleCount=getModuleCount(pCon,strModelName);
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable;
    strTable.Format("%s_ModuleList",strModelName);
	strSQL.Format("INSERT INTO %s(module_name,module_desp,calcindex,GraphCx,GraphCy)values(\'%s\',\'%s\',\'%d\',\'%d\',\'%d\')",
		strTable,strModuleName,strModuleDesp,moduleCount,0,0);
    try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
    }
   	catch(_com_error e)
	{
		return FALSE;
	}	
	return TRUE;
}
int CEPerfManageDoc::getModuleCount(_ConnectionPtr &pCon,CString strModelName)
{
	int iRet = 0;
	CString strTable;
    strTable.Format("%s_ModuleList",strModelName);
  	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;

    CString strSQL;
    strSQL.Format("select * from %s",strTable);
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		//pRecord->Close() ;
		return 0;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	//块列表包括：名字、描述/类型
	while(!pRecord->adoEOF )
	{
		iRet++;
		pRecord->MoveNext();
	}
	pRecord->Close();
	////////////////////////////////////////////////////////////////////
	return iRet;
}
//建立模型的模块列表Table
//表名为：ModleName_ModuleList
//该表的字段为：module_desp，module_name,calcindex,GraphCx,GraphCy
//字段类型分别为：string,string,short,short,short
BOOL CEPerfManageDoc::CreateModuleListTable(_ConnectionPtr &pCon,CString strModelName)
{
	CStringArray Fields;
	CStringArray FieldTypes;
    
	Fields.Add(_T("ID"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("module_name"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("module_desp"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("calcindex"));
	FieldTypes.Add(_T("short"));

	Fields.Add(_T("GraphCx"));
	FieldTypes.Add(_T("short"));

	Fields.Add(_T("GraphCy"));
	FieldTypes.Add(_T("short"));

   CString strTable;
   strTable.Format("%s_ModuleList",strModelName);
	return CDBOperator::CreateTable(pCon,strTable,Fields,FieldTypes);
}

//建立记录模块信息的表
//blocklist,blockprop两个表

BOOL CEPerfManageDoc::CreateBlockListTable(_ConnectionPtr &pCon,CString strModelName,CString strModuleName)
{
	 CString strTable;
	 CStringArray Fields;
	 CStringArray FieldTypes;
    
    strTable.Format("%s_%s_BlockList",strModelName,strModuleName);
	
	Fields.Add(_T("ID"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("name"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("desp"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("type"));
	FieldTypes.Add(_T("string:100"));

	Fields.Add(_T("orderid"));
	FieldTypes.Add(_T("short"));

	Fields.Add(_T("left"));
	FieldTypes.Add(_T("double"));
	Fields.Add(_T("top"));
	FieldTypes.Add(_T("double"));
	Fields.Add(_T("right"));
	FieldTypes.Add(_T("double"));
	Fields.Add(_T("bottom"));
	FieldTypes.Add(_T("double"));

	Fields.Add(_T("InNum"));
	FieldTypes.Add(_T("short"));
	Fields.Add(_T("OutNum"));
	FieldTypes.Add(_T("short"));

	Fields.Add(_T("InNames"));
	FieldTypes.Add(_T("string:1024"));//转为备注

	Fields.Add(_T("InValueTypes"));
	FieldTypes.Add(_T("string:1024"));//转为备注

	Fields.Add(_T("OutNames"));
	FieldTypes.Add(_T("string:1024"));//转为备注

	Fields.Add(_T("OutValueTypes"));
	FieldTypes.Add(_T("string:1024"));//转为备注

	Fields.Add(_T("InputLinkStrs"));
	FieldTypes.Add(_T("string:1024"));//转为备注

	return CDBOperator::CreateTable(pCon,strTable,Fields,FieldTypes);
	////////////////////////////////////////////////////////////////////////////////////////////
}
BOOL CEPerfManageDoc::CreateBlockPropTable(_ConnectionPtr &pCon,CString strModelName,CString strModuleName)
{
	 CString strTable;
	 CStringArray Fields;
	 CStringArray FieldTypes;
    
	strTable.Format("%s_%s_BlockProp",strModelName,strModuleName);
	
	Fields.Add(_T("ID"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("block_name"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("prop_name"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("value_item1"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("value_item2"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("value_item3"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("value_item4"));
	FieldTypes.Add(_T("string:255"));

	Fields.Add(_T("value_item5"));
	FieldTypes.Add(_T("string:255"));

	return CDBOperator::CreateTable(pCon,strTable,Fields,FieldTypes);
}
//编译当前的模型，编译的过程需要检查模型的错误，但不运行；
//编译后，提示错误和警告，然后进行编辑修改，直到错误
/*
编译过程：
1、检查计算块的计算顺序，是否能保证计算过程的进行
2、计算的时候，顺序的考虑，第1，首先对源类型进行计算，然后根据模块的计算顺序
      自动扫描可以进行的计算模块，给定；
3、根据给定的模块顺序，和在模块内的计算号确定计算的顺序号，然后检查
*/
void CEPerfManageDoc::OnRunCompile() 
{
	 //模型对象中要建立所有的计算块对象，各个视图仅仅是其显示而已，本质在文件存储中
	 //建立从文件中的计算块对象,每个模块建立自身的计算块对象
	 m_pModel->ClearModuleBlocks();
	 m_pModel->LoadAllModuleBlocks(theApp.m_pConLocalModel); //重新加载所有的计算块到各模块中
	 m_pModel->AdjustCalcSequence();//包括
	 g_WorkSpace.RemoveAll();  //清除工作空间中的变量
	 m_pModel->InitforCalc();  //建立各计算块到内部和外部工作空间的变量映射关系公共变量池，可以查看变量的情况
     m_pModel->SetDebugOnOrOff(1);//调试运行
	 CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	 pMain->ClearOutput(0);
	 m_pModel->InitCompile(pMain,WM_MSG_MODELRUN_STATUS,&g_Log);//初始化编译
	 m_pModel->SetWriteLogOnOrOff(0);//日志输出到文件关闭。
	 m_pModel->RunCompile();
}

void CEPerfManageDoc::OnUpdateRunCompile(CCmdUI* pCmdUI) 
{
}
//调试计算
//每点击一次算一回,需要重新编译并
//每次根据模型的计算顺序逐个计算，并且要列出计算模块来，按键后进行计算，并在输出中，显示计算的结果信息
void CEPerfManageDoc::OnRunDebug() 
{
	//进行编译
	OnRunCompile();
    //设置模式
	m_pModel->SetRunMode(RUN_MODE_DEBUG);
	theApp.m_pView->PostMessage(WM_TASKGUID,2);
}

void CEPerfManageDoc::OnCfgModuleorder() 
{
	// TODO: Add your command handler code here
	CDlgCfgModuleOrder dlg;
	dlg.SetModuleList(this->m_pModel->m_ModuleList);
	if(IDOK==dlg.DoModal())
	{
		this->m_pModel->AdjustModuleOrder();
		UpdateCalcIndex(theApp.m_pConLocalModel);
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		if(pMain!=NULL) pMain->RefreshModelTree(m_pModel);
		UpdateAllViews(NULL);
	}
}
void CEPerfManageDoc::UpdateCalcIndex(_ConnectionPtr &pCon)//更新数据库的模块的计算次序值。
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//检索到所有类型
	CString strTable;
    strTable.Format("%s_ModuleList",this->m_pModel->m_strModelName);
	
	strSQL.Format("DELETE FROM %s",strTable);
	try	
	{
		pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		return;
	}
	POSITION pos = this->m_pModel->m_ModuleList.GetHeadPosition();
	while(pos)
	{
		CCalcModule *pModule=  (CCalcModule*)this->m_pModel->m_ModuleList.GetNext(pos);
		strSQL.Format("INSERT INTO %s(module_name,module_desp,calcindex,GraphCx,GraphCy)values(\'%s\',\'%s\',\'%d\',\'%d\',\'%d\')",
			strTable,pModule->m_Name,pModule->m_Desp,pModule->GetCalcOrderID(),0,0);
		try	
		{
			pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
		}
		catch(_com_error e)
		{
			return;
		}
	}
}
void CEPerfManageDoc::runRemoteMode() 
{
	CString sNodeName=m_pModel->m_strModelName;
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '1';	//协议号
	strcpy(lbuf+6,sNodeName);

	g_pSocketManage->Send(lbuf,*(int *)(lbuf+1));
	int ret = g_pSocketManage->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '1')
		{
			return;
		}
		//CString str(lbuf[6]);
		//CString sMsg = "返回结果是"+str;
		//AfxMessageBox(sMsg);
	}
}
void CEPerfManageDoc::stopRemoteMode() 
{
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '2';	//协议号
	strcpy(lbuf+6,sNodeName);

	g_pSocketManage->Send(lbuf,*(int *)(lbuf+1));
	int ret = g_pSocketManage->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '2')
		{
			return;
		}
	}
}
void CEPerfManageDoc::OnMagRun() 
{
	// TODO: Add your command handler code here
	//StartNode(m_pModel->m_strModelName);
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	runRemoteMode();
	m_iRemoteModeState=Running;
}

void CEPerfManageDoc::OnUpdateMagRun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//int state=GetNodeState(m_pModel->m_strModelName);
	//pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&((Stopped==state)||(Stopping==state)));
	pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&((Stopped==m_iRemoteModeState)||(Stopping==m_iRemoteModeState)));
}

void CEPerfManageDoc::OnMagStop() 
{
	// TODO: Add your command handler code here
	//StopNode(m_pModel->m_strModelName);
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	if(IDOK!=AfxMessageBox("确定要退出计算服务？",MB_OKCANCEL))
	{
		return;
	}
	//***停止刷新远程工作空间
	g_bToExit=true;
	CloseHandle(hThread);
	m_bTagGettingWS=false;
	//***停止刷新远程工作空间
	MagEndGetLog();//停止刷新日志

	stopRemoteMode();
	m_iRemoteModeState=Stopped;
}

void CEPerfManageDoc::OnUpdateMagStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//int state=GetNodeState(m_pModel->m_strModelName);
	//pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&((Running==state)||(Starting==state)||(Paused==state)||(StartFail==state)));
	pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&((Running==m_iRemoteModeState)||(Starting==m_iRemoteModeState)||(Paused==m_iRemoteModeState)||(StartFail==m_iRemoteModeState)));
}

void CEPerfManageDoc::OnMagPause() 
{
	// TODO: Add your command handler code here
	//PauseNode(m_pModel->m_strModelName);
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '3';	//协议号
	strcpy(lbuf+6,sNodeName);

	g_pSocketManage->Send(lbuf,*(int *)(lbuf+1));
	int ret = g_pSocketManage->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '3')
		{
			return;
		}
		//CString str(lbuf[6]);
		//CString sMsg = "返回结果是"+str;
		//AfxMessageBox(sMsg);
	}
	m_iRemoteModeState=Paused;
}

void CEPerfManageDoc::OnUpdateMagPause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//int state=GetNodeState(m_pModel->m_strModelName);
	//pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&(state==Running));
	pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&(m_iRemoteModeState==Running));
}

void CEPerfManageDoc::OnMagResume() 
{
	// TODO: Add your command handler code here
	//ResumeNode(m_pModel->m_strModelName);
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '4';	//协议号
	strcpy(lbuf+6,sNodeName);

	g_pSocketManage->Send(lbuf,*(int *)(lbuf+1));
	int ret = g_pSocketManage->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '4')
		{
			return;
		}
		//CString str(lbuf[6]);
		//CString sMsg = "返回结果是"+str;
		//AfxMessageBox(sMsg);
	}
	m_iRemoteModeState=Running;
}

void CEPerfManageDoc::OnUpdateMagResume(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//int nodeState=GetNodeState(m_pModel->m_strModelName);
	//pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&(nodeState==Paused));
	pCmdUI->Enable((m_pModel->m_iTagModeLocalOrRemote==1)&&(m_iRemoteModeState==Paused));
}
int CEPerfManageDoc::GetNodeState(CSocketManage *&pSocket,CString sNodeName)
{
	int retState=-1;
	char lbuf[1024];	
	memset(lbuf,0,1024);
	if(pSocket==NULL)
		return retState;

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '5';	//协议号
	strcpy(lbuf+6,sNodeName);

	pSocket->Send(lbuf,*(int *)(lbuf+1));
	int ret = pSocket->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return retState;
		}
		if(lbuf[5] != '5')
		{
			return retState;
		}
		CString str(lbuf[6]);
		//CString sMsg = "返回结果是"+str;
		//AfxMessageBox(sMsg);
		retState=atoi(str);
	}
	return retState;
}

void CEPerfManageDoc::OnFileUploadmodel() 
{
	// TODO: Add your command handler code here
	BOOL bHased = CDBOperator::CheckRecordExisted(theApp.m_pConRemoteModel,_T("ModelList"),_T("model_name"),m_pModel->m_strModelName);
	CString str;
	int int_selOK=1;
	if(bHased)
	{
		str.Format("远程已存在名称为:'%s'的模型，是否上传并覆盖？",m_pModel->m_strModelName);
		int_selOK=AfxMessageBox(str,MB_OKCANCEL);	
	}
	if(int_selOK==1)
	{
		CString strIP,strPort;//保留远程模型的部署信息IP和Port
		ModelData modelDataRemote;//删除远程
		modelDataRemote.ReadDate(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataRemote.GetIPAndPort(strIP,strPort);//保留IP和Port
		modelDataRemote.DelTheModelTables(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		modelDataRemote.DelModelInModelListTable(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
	
		ModelData modelDataObj;
		modelDataObj.ReadDate(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		modelDataObj.CreatTheModelTablesInMySql(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataObj.SetIPAndPort(strIP,strPort);//设置保留IP和Port
		modelDataObj.WriteData(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		
		//上载运行控制数据
		m_pMCalControl->SetConPtr(theApp.m_pConRemoteModel);
		m_pMCalControl->SaveAllCtrlDataToDB();
		m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);

		AfxMessageBox("上载成功！");
	}
}

void CEPerfManageDoc::OnUpdateFileUploadmodel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}

void CEPerfManageDoc::OnUpdateCfgAddBlock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain==NULL)
	{
		pCmdUI->Enable(false);
		return;
	}

	CString strAddType;
	strAddType.Empty();
	CTreeCtrl* pWndTree = (CTreeCtrl*)&(pMain->m_wndClassView.m_wndClassView);

	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
	if(hTreeItem==NULL)
	{
		pCmdUI->Enable(false);
		return;
	}
 	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	if(pWS->m_iItemType ==0)
	{
        pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}
void CEPerfManageDoc::OnCfgAddBlock() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain==NULL) return;

	CString strAddType;
	strAddType.Empty();
	CTreeCtrl* pWndTree = (CTreeCtrl*)&(pMain->m_wndClassView.m_wndClassView);

	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
 	CTreeItemObj* pWS = (CTreeItemObj*) pWndTree->GetItemData (hTreeItem);
	if(pWS->m_iItemType ==0)
	{
           strAddType = pWS->m_strBlockType; 
	}
	else
	{
		return;
	}
	//////////////////////////////////////////////
	CView* pView = pMain->GetActiveView();
	if(pView==NULL) return;
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)pView->GetDocument();
	if(pDoc!=NULL)
	{
			pDoc->AddLibBlock(strAddType);
	}
}

void CEPerfManageDoc::OnCfgModeRunControl() 
{
	// TODO: Add your command handler code here
	if(m_pMCalControl!=NULL)
	{
		CDlgModeRunCtrl dlg;
		dlg.SetModelCalcControl(m_pMCalControl);
		dlg.DoModal();
	}
}

void CEPerfManageDoc::OnUpdateCfgModeRunControl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}

void CEPerfManageDoc::OnMagStartGetLog() 
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->ActiveOutPutPage(2);
	// TODO: Add your command handler code here
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	if(g_pSocketLogServ != NULL)
	{
		if(g_pSocketLogServ->IsConnect())
		{
			char lbuf[1024];	
			memset(lbuf,0,1024);

			lbuf[0] = '*';	//安全传输符号
			*(int *)(lbuf+1) = 6;
			lbuf[5] = '9';	//协议号
			g_pSocketLogServ->Send(lbuf,6);
			return;
		}
		else
		{
			g_ServSocketExit(g_pSocketLogServ);
		}
	}
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '7';	//协议号
	strcpy(lbuf+6,sNodeName);

	
	g_pSocketManage->Send(lbuf,*(int *)(lbuf+1));
	int ret = g_pSocketManage->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '7')
		{
			return;
		}
		CString str(lbuf+6);
		CString sMsg = "返回端口是"+str;
		//AfxMessageBox(sMsg);
	}
	

	//下面真正给CalcServ发取日志通知
	g_SocketServPort = ::atoi(lbuf+6);

	g_pSocketLogServ = g_ServSocketInit(m_pModel->m_strModelIP,g_SocketServPort);
	if(!g_pSocketLogServ)
	{
		AfxMessageBox("连接该计算模型失败，可能模型未启动或网络相关故障!");
		return;
	}

	//发送请求
	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6;
	lbuf[5] = '9';	//协议号
	g_pSocketLogServ->Send(lbuf,6);
	m_bTagGettingLog=true;

}

void CEPerfManageDoc::OnUpdateMagStartGetLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		pCmdUI->Enable(false);
		return;
	}
	//CSocketServ* pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,atoi(m_pModel->m_strModelIP));
	if(g_pSocketLogServ==NULL)
	{
		pCmdUI->Enable(true);
	}
	//else if(g_pSocketLogServ->IsConnect())
	//{
		pCmdUI->Enable(!m_bTagGettingLog);
	//}
	//else
	//	pCmdUI->Enable(false);
}
void CEPerfManageDoc::MagEndGetLog()
{
	if(g_pSocketLogServ != NULL)
	{
		if(g_pSocketLogServ->IsConnect())
		{
			char lbuf[1024];	
			memset(lbuf,0,1024);

			lbuf[0] = '*';	//安全传输符号
			*(int *)(lbuf+1) = 6;
			lbuf[5] = '0';	//协议号
			g_pSocketLogServ->Send(lbuf,6);
		}
		g_ServSocketExit(g_pSocketLogServ);
		m_bTagGettingLog=false;
	}
}
void CEPerfManageDoc::OnMagEndGetLog() 
{
	// TODO: Add your command handler code here
	MagEndGetLog();
}

void CEPerfManageDoc::OnUpdateMagEndGetLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		pCmdUI->Enable(false);
		return;
	}
	//CSocketServ* pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,atoi(m_pModel->m_strModelIP));
	if(g_pSocketLogServ==NULL)
	{
		pCmdUI->Enable(false);
	}
	//else if(g_pSocketLogServ->IsConnect())
	//{
		pCmdUI->Enable(m_bTagGettingLog);	
	//}
	//else
	//	pCmdUI->Enable(false);
}
bool g_RefreshWSpaceOnce(void *pThreadData)
{
	CCalcModel *m_pModel=(CCalcModel*)pThreadData;
	CString sNodeName=m_pModel->m_strModelName;
	char lbuf[1024];	
	memset(lbuf,0,1024);
	//下面真正从CalcServ中取数据
	g_SocketServPort = ::atoi(lbuf+6);
	CSocketServ* pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,::atoi(m_pModel->m_strPort));
	if(!pSockServ)
	{
		AfxMessageBox("连接该计算模型失败，可能模型未启动或网络相关故障!");
		return false;
	}
	//发送请求
	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6;
	lbuf[5] = '8';	//协议号
	strcpy(lbuf+6,sNodeName);
	pSockServ->Send(lbuf,6);
	
	char lbufHead[6];
	memset(lbufHead,0,6);

	int ret3 = pSockServ->Receive(lbufHead,6);
	if(ret3>0)
	{
		if(lbufHead[0] != '*')
		{
			return false;
		}
		if(lbufHead[5] != '8')
		{
			return false;
		}
	}
	else
		return false;
	CArray<char,char> bufArr;
	g_ReceivePacketsToCharArr(pSockServ,lbufHead,6,bufArr);
	g_PutCharArrToWSpace(bufArr);
	bufArr.RemoveAll();
	
	m_pModel->GetBlockInputValueFromGlobalWS();

	g_ServSocketExit(pSockServ);
	
	theApp.m_pTreeItemObj->m_strName = "工作空间视图";
	theApp.m_pTreeItemObj->m_iItemType = 3;
	theApp.m_pView->PostMessage(WM_TASKGUID,30);
	return true;
}
unsigned __stdcall ThreadRefreshWSpace(void* pThreadData)
{
	while(TRUE)
	{
		if(g_bToExit)		//退出信号
			break;
		int nRet = ::WaitForSingleObject(g_RunEvent,INFINITE);
		if(nRet == WAIT_FAILED)
		{
			break;
		}
		else if(nRet == WAIT_OBJECT_0)	
		{
			bool ret=g_RefreshWSpaceOnce(pThreadData);
			if(false==ret)//如果失败则停止刷新工作空间和日志
			{
				CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
				g_bToExit=true;
				CloseHandle(hThread);
				pDoc->m_bTagGettingWS=false;
				pDoc->MagEndGetLog();
				break;
			}
			//DoWork();
			::Sleep(5000);	//代表工作量
		}
		else
		{
			break;
		}
	}
	return 1;
}
void CEPerfManageDoc::OnMagRefreshws() 
{
	// TODO: Add your command handler code here
	//GetData(&g_WorkSpace);

	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}	
	g_RefreshWSpaceOnce(m_pModel);
	// TODO: Add your control notification handler code here
	
}

void CEPerfManageDoc::OnUpdateMagRefreshws(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		pCmdUI->Enable(false);
		return;
	}
	if(g_pSocketWorkSpaceServ==NULL)
	{
		pCmdUI->Enable(true);
	}
	else if(g_pSocketWorkSpaceServ->IsConnect())
	{
		pCmdUI->Enable(true);
	}
	else
		pCmdUI->Enable(false);
	
}
void CEPerfManageDoc::OnMagRefreshWSStart() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->ActiveOutPutPage(2);
	// TODO: Add your command handler code here
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("连接管理工具失败，可能未启动或网络相关故障！");
		return;
	}
	//g_pSocketManage->Detach();
	g_bToExit=false;
    unsigned threadID;
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadRefreshWSpace, m_pModel, 0, &threadID);
		if (hThread > 0)
		{
			m_bTagGettingWS=true;
		}
		else
		{
			AfxMessageBox("启动刷新失败!");
			return ;
		}
	}
}

void CEPerfManageDoc::OnUpdateMagRefreshWSStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		pCmdUI->Enable(false);
		return;
	}
	if(g_pSocketWorkSpaceServ==NULL)
	{
		pCmdUI->Enable(true);
	}
	//else if(g_pSocketWorkSpaceServ->IsConnect())
	//{
		pCmdUI->Enable(!m_bTagGettingWS);
	//}
	//else
	//	pCmdUI->Enable(false);
}

void CEPerfManageDoc::OnMagRefreshWSEnd() 
{
	// TODO: Add your command handler code here
	g_bToExit=true;
	CloseHandle(hThread);
	m_bTagGettingWS=false;
	if(m_pModel==NULL)
		return;
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		AfxMessageBox("该模型为本地模型，请打开远程模型再进行操作！");
		return;
	}
}

void CEPerfManageDoc::OnUpdateMagRefreshWSEnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pModel->m_iTagModeLocalOrRemote!=1)
	{
		pCmdUI->Enable(false);
		return;
	}
	if(g_pSocketWorkSpaceServ==NULL)
	{
		pCmdUI->Enable(false);
	}
	//else if(g_pSocketWorkSpaceServ->IsConnect())
	//{
		pCmdUI->Enable(m_bTagGettingWS);	
	//}
	//else
	//	pCmdUI->Enable(false);
}

void CEPerfManageDoc::OnMagModelSyn() 
{
	// TODO: Add your command handler code here
	BOOL bHased = CDBOperator::CheckRecordExisted(theApp.m_pConRemoteModel,_T("ModelList"),_T("model_name"),m_pModel->m_strModelName);
	CString str;
	int int_selOK=1;
	int retDlg=-1;
	if(bHased)
	{
		CDlgModelSynNote dlg;
		dlg.DoModal();
		retDlg=dlg.m_iTag;
	}
	if((retDlg==1)||(retDlg==2))//2:同步并重启，1:只同步，0：取消
	{
		CString strIP,strPort;//保留远程模型的部署信息IP和Port
		ModelData modelDataRemote;//删除远程
		modelDataRemote.ReadDate(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataRemote.GetIPAndPort(strIP,strPort);
		modelDataRemote.DelTheModelTables(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		modelDataRemote.DelModelInModelListTable(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		
		ModelData modelDataObj;
		modelDataObj.ReadDate(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		modelDataObj.CreatTheModelTablesInMySql(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataObj.SetIPAndPort(strIP,strPort);
		modelDataObj.WriteData(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		
		//上载运行控制数据
		m_pMCalControl->SetConPtr(theApp.m_pConRemoteModel);
		m_pMCalControl->SaveAllCtrlDataToDB();
		m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
		if(retDlg==2) //2:同步并重启
		{
			//***停止刷新远程工作空间
			g_bToExit=true;
			CloseHandle(hThread);
			m_bTagGettingWS=false;
			//***停止刷新远程工作空间
			MagEndGetLog();//停止刷新日志

			stopRemoteMode();
			Sleep(500);
			runRemoteMode();
		}
		AfxMessageBox("同步成功！");
	}
}

void CEPerfManageDoc::OnUpdateMagModelSyn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// TODO: Add your command handler code here
	pCmdUI->Enable(m_pModel->m_iTagModeLocalOrRemote==1);
}

void CEPerfManageDoc::OnFileDelModel() 
{
	// TODO: Add your command handler code here
	if((!theApp.m_bModelLocalConOK)&&(theApp.m_bModelRemoteConOK))
	{
		AfxMessageBox("与模型数据的连接未建立成功，不能打开模型");
		return;
	}
	CDlgDelModel dlg;
	dlg.DoModal();
}

void CEPerfManageDoc::OnCfgDelModule() 
{
	// TODO: Add your command handler code here

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain==NULL) 
	{
		AfxMessageBox("当前无法删除模型！");
		return;
	}

	HTREEITEM hTreeItem = (pMain->m_wndModelView.m_wndClassView).GetSelectedItem();
	if(hTreeItem==NULL)
	{
		return;
	}
	CTreeItemObj* pWS = (CTreeItemObj*) (pMain->m_wndModelView.m_wndClassView).GetItemData (hTreeItem);
	if(pWS==NULL)
	{
		AfxMessageBox("请选择要删除的模块!");
		return;
	}
	if(pWS->m_iItemType==1) //如果模块被点击
	{
		if(IDOK==AfxMessageBox("确定要删除选定模块？",MB_OKCANCEL))
		{
			((CEPerfManageView*)(theApp.m_pView))->CloseUserTabByName(pWS->m_strName);
			m_pModel->DelModuleByName(pWS->m_strName);
			DelModuleInfoFromDB(theApp.m_pConLocalModel,this->m_pModel->m_strModelName,pWS->m_strName);
			
			pMain->RefreshModelTree(m_pModel);
			UpdateAllViews(NULL);
		}
	}
	else
	{
		AfxMessageBox("请选择要删除的模块!");
		return;
	}
}

void CEPerfManageDoc::OnUpdateCfgDelModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain==NULL) 
	{
		pCmdUI->Enable(false);
	}
	else
	{
		HTREEITEM hTreeItem = (pMain->m_wndModelView.m_wndClassView).GetSelectedItem();
		if(hTreeItem==NULL)
		{
			pCmdUI->Enable(false);
		}
		else
		{
			CTreeItemObj* pWS = (CTreeItemObj*) (pMain->m_wndModelView.m_wndClassView).GetItemData (hTreeItem);
			if(pWS==NULL)
			{
				pCmdUI->Enable(false);
			}
			else
			{
				if(pWS->m_iItemType==1) //如果模块被点击
				{
					pCmdUI->Enable(true);
				}
				else
					pCmdUI->Enable(false);

			}
		}
	}
}

//启动本地计算-------------------------------------------------
//运行方式：
//(1)  实时运行：
//(2)  历史计算: 采用模拟方式，模拟实时，可先读取历史数据，然后按照模拟实时的运行
//(3)  历史计算：采用非实时方式，也可先缓冲读取历史数据。
//(4)  计算一次：按时刻计算（当前、历史）；时间段：选择历史时间段

void CEPerfManageDoc::OnRunLocalStart() 
{
	OnRunCompile();

	CDlgLocalRunSetting  dlg;
	dlg.m_nCalcMode = m_pLocalThreadPara->CalcMode;
	dlg.m_nCycleT =  m_pLocalThreadPara->intCycle;
	
	dlg.m_StartDate = m_pLocalThreadPara->startTime;
	dlg.m_StartTime = m_pLocalThreadPara->startTime;
	dlg.m_EndDate = m_pLocalThreadPara->endTime;
	dlg.m_EndTime = m_pLocalThreadPara->endTime;
	dlg.m_nInterval = m_pLocalThreadPara->TimeInterval;

	if(dlg.DoModal()==IDOK)
	{
		m_pLocalThreadPara->CalcMode = dlg.m_nCalcMode;
		m_pLocalThreadPara->intCycle = dlg.m_nCycleT;

		m_pLocalThreadPara->startTime = CTime(dlg.m_StartDate.GetYear(),dlg.m_StartDate.GetMonth(),dlg.m_StartDate.GetDay(),dlg.m_StartTime.GetHour(),dlg.m_StartTime.GetMinute(),dlg.m_StartTime.GetSecond());
		m_pLocalThreadPara->endTime = CTime(dlg.m_EndDate.GetYear(),dlg.m_EndDate.GetMonth(),dlg.m_EndDate.GetDay(),dlg.m_EndTime.GetHour(),dlg.m_EndTime.GetMinute(),dlg.m_EndTime.GetSecond());
		m_pLocalThreadPara->TimeInterval = dlg.m_nInterval;
	}
	else return;
	// 给线程参数赋值
	m_pLocalThreadPara->m_pModel = m_pModel;
	m_pLocalThreadPara->m_hWnd = ((CMainFrame*)AfxGetMainWnd())->m_hWnd;
    //-启动线程-----------------------------------------------------------------------------------------------------------------------
	DWORD ThreadID;
	DWORD code;
	if(!GetExitCodeThread(m_hLocalThread,&code)||(code!=STILL_ACTIVE))
	{
		switch(m_pLocalThreadPara->CalcMode)
		{
		case 0:
		case 1:
			{
				m_hLocalThread=CreateThread(NULL,0,TRunInCycle,m_pLocalThreadPara,0,&ThreadID);//创建并开始线程
				break;
			}
		case 2:
			{
				m_hLocalThread=CreateThread(NULL,0,TRunUserSetting,m_pLocalThreadPara,0,&ThreadID);//创建并开始线程
				break;
			}
		}
	}
	m_bInLocalRun = TRUE;
   ((CMainFrame*)AfxGetMainWnd())->ClearOutput(0);
}

void CEPerfManageDoc::OnUpdateRunLocalStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bInLocalRun);
	
}
//停止本地计算--------------------------------------------------
void CEPerfManageDoc::OnRunLocalStop() 
{
	// TODO: Add your command handler code here
	DWORD code;
	 if(GetExitCodeThread(m_hLocalThread,&code))
	 {
		if(code==STILL_ACTIVE)								                	//如果当前线程还活动
		{
			TerminateThread(m_hLocalThread,0);						//前些终止线程
			CloseHandle(m_hLocalThread);								//销毁线程句柄
		}
	 }
	m_bInLocalRun = FALSE;
	m_hLocalThread=NULL;
}

void CEPerfManageDoc::OnUpdateRunLocalStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bInLocalRun);
}

void CEPerfManageDoc::OnCfgFindBlock() 
{
	// TODO: Add your command handler code here
	if(pDlgFindBlock==NULL)
	{
		pDlgFindBlock=new CDlgCfgFindBlock;
		pDlgFindBlock->Create(IDD_CFG_FIND_BLOCK);
	}
	pDlgFindBlock->ShowWindow(SW_SHOW);
}

void CEPerfManageDoc::OnUpdateCfgFindBlock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}

void CEPerfManageDoc::OnMagPortValueShow() 
{
	// TODO: Add your command handler code here
	CEPerfManageView* pMainView = GetMainView();
	CBCGPTabWnd &wndTabs=pMainView->GetTabControl();
	
	int allTabsNum=wndTabs.GetTabsNum();
	for(int i=0;i<allTabsNum;i++)
	{
		CView *pView=(CView*)wndTabs.GetTabWnd(i);
		if(pView!=NULL && pView->IsKindOf(RUNTIME_CLASS(CViewModuleFrm))) 
		{
			CViewModuleFrm* pModuleView=(CViewModuleFrm*)pView;
			pModuleView->ShowPortValue();
		}
	}
}
//打开所有模块
void CEPerfManageDoc::OnFileOpenAllModule() 
{
	// TODO: Add your command handler code here
	int iAllRecordCount=m_pModel->GetModuleCount();
	int iItem=0;
	if(iAllRecordCount==0)
		return;
	CDlgProgress ProgDlg;
	MSG msg;
	ProgDlg.Create(IDD_DLGPROGRESS, NULL);
	ProgDlg.ShowWindow(SW_SHOW);
	ProgDlg.CenterWindow();
	ProgDlg.UpdateData(FALSE);
	ProgDlg.m_bCancel = FALSE;
	CString strOldModelName=theApp.m_pTreeItemObj->m_strName;
	POSITION pos = m_pModel->m_ModuleList.GetHeadPosition();
	while(pos)
	{
		if(ProgDlg.m_bCancel) 
		{
			ProgDlg.PostMessage(WM_CLOSE, 0, 0);
			break;
		}
		CCalcModule* pModule=  (CCalcModule*)m_pModel->m_ModuleList.GetNext(pos);
		theApp.m_pTreeItemObj->m_strName = pModule->m_Name;
		theApp.m_pView->SendMessage(WM_TASKGUID,11);
		float persent =(float)((float)iItem/(float)iAllRecordCount)*100;
		ProgDlg.SendMessage(WM_MSG_PROGESS,(float)persent,0);
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!ProgDlg.IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		iItem++;
	}
	theApp.m_pTreeItemObj->m_strName=strOldModelName;
	ProgDlg.ShowWindow(SW_HIDE);
	ProgDlg.SendMessage(WM_CLOSE, 0, 0);
	if(!ProgDlg.m_bCancel)
	{
		AfxMessageBox("已打开所有模块！");
	}
}

void CEPerfManageDoc::OnUpdateFileOpenAllModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}

void CEPerfManageDoc::OnFileCloseAllModule() 
{
	// TODO: Add your command handler code here
	if(theApp.m_pView!=NULL)
	{
		theApp.m_pView->SendMessage(WM_TASKGUID,0);
	}
}

void CEPerfManageDoc::OnUpdateFileCloseAllModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMCalControl!=NULL);
}
