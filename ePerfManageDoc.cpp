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
extern CLog g_Log;//ȫ�ֵ������־����;

extern DWORD WINAPI TRunInCycle(LPVOID pthread);
extern DWORD WINAPI TRunUserSetting(LPVOID pthread);

enum RunCommand{Start=1,Stop,Pause,Resume,Kill};
enum RunState{Unknown=-1,Running=1,Starting,Stopped,Stopping,Paused,StartFail};

extern BOOL g_ReceivePacketsToCharArr(CSocketServ *pSockServ,char *pFirst,int iFirstCount,CArray<char,char> &bufArr);
extern void g_PutCharArrToWSpace(const CArray<char,char> &bufArr);

BOOL g_bToExit = false;	//�˳��ź�
HANDLE g_RunEvent;	//����ʱ��
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
		while(pos1)//ɾ����ͬKey��Ԫ����ͬ��Ԫ�ء�
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
		AfxMessageBox("��ģ�����ݵ�����δ�����ɹ������ܴ�ģ��");
		return;
	}
	CDlgDepModel dlg;
	dlg.DoModal();
}

void CEPerfManageDoc::OnFileOpenModel()
{
   if((!theApp.m_bModelLocalConOK)&&(theApp.m_bModelRemoteConOK))
   {
	   AfxMessageBox("��ģ�����ݵ�����δ�����ɹ������ܴ�ģ��");
	   return;
   }
  
   ///////////////////////////////////////////////////////////////
   CDlgOpenModel dlg;
   if(dlg.DoModal()==IDOK)
   {
       ////////////////
	   //�����ѡ���ģ�����ֺ��������ݴˣ��ɴ�ģ�͵�ģ���б��Լ���Ӧ��
	   //�õ��ĵ�PDOCָ��
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
        //��ȡģ�Ͳ��γɶ���
	   if(m_pModel->m_iTagModeLocalOrRemote==0)
	   {
		   m_pModel->ReadModel(theApp.m_pConLocalModel); //��ȡ����
		   m_pModel->EstablishSourceRDBConnect(theApp.m_pConLocalModel);//���ؿ��õ�����Դ
		   m_pModel->ClearModuleBlocks();
		   m_pModel->LoadAllModuleBlocks(theApp.m_pConLocalModel); 
		   m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
		   m_pMCalControl->ReadCtrlData();
		   CString str;
		   str.Format("VeStore-DisCalc--%s--����",m_pModel->m_strModelName);
		   ((CMainFrame*)AfxGetMainWnd())->SetWindowText(str);
	   }
	   else
	   {
		   ModelData modelDataLocal;//ɾ������ģ��
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
		   
		   m_pModel->ReadModel(theApp.m_pConRemoteModel); //��ȡԶ��
		   m_pModel->EstablishSourceRDBConnect(theApp.m_pConRemoteModel);//���ؿ��õ�����Դ
		   m_pModel->ClearModuleBlocks();
		   m_pModel->LoadAllModuleBlocks(theApp.m_pConRemoteModel); 

		   CString str;
		   str.Format("VeStore-DisCalc--%s--Զ��",m_pModel->m_strModelName);
		   ((CMainFrame*)AfxGetMainWnd())->SetWindowText(str);
		   	   
		   g_SocketManageExit();
		   if(!g_SocketManageInit(m_pModel->m_strModelIP,11650))
		   {
				AfxMessageBox("Զ�̹������޷�����,���޷�����Զ�̿��ƣ�");
		   }
		   m_iRemoteModeState=GetNodeState(g_pSocketManage,m_pModel->m_strModelName);
	   }
	   m_pModel->AdjustCalcSequence();//����
	   g_WorkSpace.RemoveAll();  //��������ռ��еı���//to do
	   m_pModel->InitforCalc();  //����������鵽�ڲ����ⲿ�����ռ�ı���ӳ���ϵ���������أ����Բ鿴���������
	   m_pModel->SetDebugOnOrOff(1);//��������
       //����������ʾ
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
���������������δ洢���洢����������
�м������룬ÿ������
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
	//�½�һ������ģ�ͣ���Ҫ��յ�ǰ��ģ����Ϣ��
	//����ģ�͵����ƺ�������Ȼ���Ϊһ���µ�ģ��
	CDlgNewModel dlg;
	if(dlg.DoModal()==IDOK)
	{
		//�����û��ͬ����ģ��,��û�еĻ����뵽
		//�����û�У�����У�����Ҫ�޸�Ϊ��һ������
		BOOL bHased = CDBOperator::CheckRecordExisted(theApp.m_pConLocalModel,_T("ModelList"),_T("model_name"),dlg.m_strName);
		if(bHased)
		{
			AfxMessageBox("�Ѿ���һ��ͬ����ģ�ʹ��ڣ���ѡ����������.");
			return;
		}
		theApp.m_pView->PostMessage(WM_TASKGUID,0);//�رյ�ǰ��ģ��
		m_pModel->m_iTagModeLocalOrRemote=0;//���Ϊ����ģ��
		//��ģ�ͱ��м����ģ�����ƣ�������
		BOOL bOK = AddModelInfoToDB(theApp.m_pConLocalModel,dlg.m_strName,dlg.m_strDesp);  //д��ģ�ͱ���
		//����ģ�ͼ�¼ģ���,
		bOK =  CreateModuleListTable(theApp.m_pConLocalModel,dlg.m_strName);
		if(!bOK) 
		{
			AfxMessageBox("���󣺲��ܴ�����Ӧ�洢��.");
			return;
		}
		
		m_pModel->ResetModelContent();
		m_pModel->m_strModelDesp = dlg.m_strDesp;
		m_pModel->m_strModelName = dlg.m_strName;
		
		if(m_pMCalControl==NULL)//�������п������ݲ�����
		{
			m_pMCalControl= new ModelCalcControl(m_pModel->m_strModelName);
			m_pMCalControl->SetDefaulControlData();//����Ĭ�Ͽ��Ʒ�ʽ
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
//�ڵ�ǰ��ģ���£����һ��ģ��
void CEPerfManageDoc::OnCfgAddModule() 
{
	//����ģ����
	CDlgAddModule dlg;
	if(dlg.DoModal()==IDOK)
	{
	   //�����Ƿ���ģ�����������
		CString strModuleName = dlg.m_strName;
		CString strModuleDesp = dlg.m_strDesp;
		if(m_pModel->LookUpMoudle(strModuleName,strModuleDesp)==1)
		{
			AfxMessageBox("ģ������/�����ظ��������¶���ģ�����ƻ�����!");
			return;
		}
		else
		{
			//������ģ�������洢����Ϊ��ʱ��Ҫ���ж�ȡ��Ϣ
			BOOL  bOK= CreateBlockListTable(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName);
			if(!bOK) 
			{
				AfxMessageBox("���󣺽���������б�洢��!");
				return;
			}
			bOK= CreateBlockPropTable(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName);
			if(!bOK) 
			{
				AfxMessageBox("���󣺽�����������Դ洢��!");
				return;
			}
			//////////////////////////////////////////////////////
			CCalcModule *pModule = new CCalcModule(); //�½�һ��ģ�����
			pModule->SetModuleName(strModuleName);
			pModule->SetModuleDesp(strModuleDesp);
			pModule->SetParentName(&(m_pModel->m_strModelName)); //д�������ĸ�ģ��
			m_pModel->AddModule(pModule);

			//��ģ����д�����
			AddModuleInfoToDB(theApp.m_pConLocalModel,m_pModel->m_strModelName,strModuleName,strModuleDesp);  //д��ģ�ͱ���

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
//���ݿ��������ӵ�ģ����;
//�Ƿ���ԣ�������ͼ�ζ���Ȼ�����ͼ�ζ���������ģ�������ôͼ�ζ�����Ҫ
//����ģ���������Բſ��ԣ�����������λ����Ϣ��������Ϣ
//ͼ����Ϣ�����͡����ơ�λ�á����롢���
//////////////////////////////////////////////////////////////////////////////////////////////////
/*��ͼ��ʵ�ʶ�������Դ��ݡ���ֵ���ݻ���
1��������ʱ��Ҫ���Ĭ�ϵ���������������˿ڵ���Ϣ
2.��˫��ʱ����Ҫ���������ԣ�
3.����ʾ��ֵ�ǣ���Ҫ�Ӷ˿��л������ֵ
4.����ͼ��ɾ��ʱ���Ƿ�ɾ��������󣿱���ʱ����Ҫ���ݣ�
5.�����ƶ���ʱ���Ƿ�����Ҳ�����أ�
5��������ʱ��������ͼ�ζ�����
====================================
����ǣ�����ʱ����ȡ���ԣ����γ�һ����Ƕ�Ķ��������
*/
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEPerfManageDoc::AddLibBlock(CString strBlockType)
{
	BOOL bRet=TRUE;
	CString strModule;
	/////////////////////////////////////////////
	//���ȼ����ģ�������ͼ�����û��Ҫ��ʾ��
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
		AfxMessageBox("��򿪲��л���Ҫ��Ӽ�����ģ����ͼ��");
		return FALSE;
	}
	/////////////////////////////////////////////
	//Ѱ��һ������������
	CCalcModule* pModule = m_pModel->FindModuleByName(strModule);
	CString strDefultName = pModule->FindCanUseBlockName(strBlockType);
	//if(pModule==NULL) return FALSE;

	//CCalcBlock* pBlock = pModule->CreateBlock(strBlockType,strDefultName,strDefultName);
	//if(pBlock!=NULL)	pModule->AddBlock(pBlock);

	///////////////////////////////////////////
    //����ͼ�л�һ������
	///////////////////////////////////////////
	pModuleView->OnButtonBlock(strBlockType,strDefultName,strBlockType);
//////////////////////////////////////////////////////////////////////////////////////////////////////
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain!=NULL) pMain->RefreshModelTree(m_pModel);
	//UpdateAllViews(NULL,HINT_UPDATE_MODULE); 

	return bRet;
}
//��Ӽ����
void CEPerfManageDoc::OnModuleAddblock() 
{
	//�����Ի���ѡ���������͵�ģ�飬���������ƣ�������Ӣ���������������Ķ�
	
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
//��һ��ģ��


//����ģ��
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
//��ģ���б�����Ƿ����ظ������ظ�
BOOL CEPerfManageDoc::AddModelInfoToDB(_ConnectionPtr &pCon,CString strModelName,CString strModelDesp)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//��������������
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
	strTable.Format("%s_ModuleList",strModelName);//��ģ���б���ɾ��ģ����Ϣ��
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
	//ɾ��ģ��������
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
	//��������������
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
	//���б���������֡�����/����
	while(!pRecord->adoEOF )
	{
		iRet++;
		pRecord->MoveNext();
	}
	pRecord->Close();
	////////////////////////////////////////////////////////////////////
	return iRet;
}
//����ģ�͵�ģ���б�Table
//����Ϊ��ModleName_ModuleList
//�ñ���ֶ�Ϊ��module_desp��module_name,calcindex,GraphCx,GraphCy
//�ֶ����ͷֱ�Ϊ��string,string,short,short,short
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

//������¼ģ����Ϣ�ı�
//blocklist,blockprop������

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
	FieldTypes.Add(_T("string:1024"));//תΪ��ע

	Fields.Add(_T("InValueTypes"));
	FieldTypes.Add(_T("string:1024"));//תΪ��ע

	Fields.Add(_T("OutNames"));
	FieldTypes.Add(_T("string:1024"));//תΪ��ע

	Fields.Add(_T("OutValueTypes"));
	FieldTypes.Add(_T("string:1024"));//תΪ��ע

	Fields.Add(_T("InputLinkStrs"));
	FieldTypes.Add(_T("string:1024"));//תΪ��ע

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
//���뵱ǰ��ģ�ͣ�����Ĺ�����Ҫ���ģ�͵Ĵ��󣬵������У�
//�������ʾ����;��棬Ȼ����б༭�޸ģ�ֱ������
/*
������̣�
1���������ļ���˳���Ƿ��ܱ�֤������̵Ľ���
2�������ʱ��˳��Ŀ��ǣ���1�����ȶ�Դ���ͽ��м��㣬Ȼ�����ģ��ļ���˳��
      �Զ�ɨ����Խ��еļ���ģ�飬������
3�����ݸ�����ģ��˳�򣬺���ģ���ڵļ����ȷ�������˳��ţ�Ȼ����
*/
void CEPerfManageDoc::OnRunCompile() 
{
	 //ģ�Ͷ�����Ҫ�������еļ������󣬸�����ͼ����������ʾ���ѣ��������ļ��洢��
	 //�������ļ��еļ�������,ÿ��ģ�齨������ļ�������
	 m_pModel->ClearModuleBlocks();
	 m_pModel->LoadAllModuleBlocks(theApp.m_pConLocalModel); //���¼������еļ���鵽��ģ����
	 m_pModel->AdjustCalcSequence();//����
	 g_WorkSpace.RemoveAll();  //��������ռ��еı���
	 m_pModel->InitforCalc();  //����������鵽�ڲ����ⲿ�����ռ�ı���ӳ���ϵ���������أ����Բ鿴���������
     m_pModel->SetDebugOnOrOff(1);//��������
	 CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	 pMain->ClearOutput(0);
	 m_pModel->InitCompile(pMain,WM_MSG_MODELRUN_STATUS,&g_Log);//��ʼ������
	 m_pModel->SetWriteLogOnOrOff(0);//��־������ļ��رա�
	 m_pModel->RunCompile();
}

void CEPerfManageDoc::OnUpdateRunCompile(CCmdUI* pCmdUI) 
{
}
//���Լ���
//ÿ���һ����һ��,��Ҫ���±��벢
//ÿ�θ���ģ�͵ļ���˳��������㣬����Ҫ�г�����ģ��������������м��㣬��������У���ʾ����Ľ����Ϣ
void CEPerfManageDoc::OnRunDebug() 
{
	//���б���
	OnRunCompile();
    //����ģʽ
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
void CEPerfManageDoc::UpdateCalcIndex(_ConnectionPtr &pCon)//�������ݿ��ģ��ļ������ֵ��
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
	_variant_t RecordsAffected;
	//////////////////////////////////////////////////////
	//��������������
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

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '1';	//Э���
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
		//CString sMsg = "���ؽ����"+str;
		//AfxMessageBox(sMsg);
	}
}
void CEPerfManageDoc::stopRemoteMode() 
{
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '2';	//Э���
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
		return;
	}
	if(IDOK!=AfxMessageBox("ȷ��Ҫ�˳��������",MB_OKCANCEL))
	{
		return;
	}
	//***ֹͣˢ��Զ�̹����ռ�
	g_bToExit=true;
	CloseHandle(hThread);
	m_bTagGettingWS=false;
	//***ֹͣˢ��Զ�̹����ռ�
	MagEndGetLog();//ֹͣˢ����־

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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '3';	//Э���
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
		//CString sMsg = "���ؽ����"+str;
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	// TODO: Add your control notification handler code here
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '4';	//Э���
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
		//CString sMsg = "���ؽ����"+str;
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

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '5';	//Э���
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
		//CString sMsg = "���ؽ����"+str;
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
		str.Format("Զ���Ѵ�������Ϊ:'%s'��ģ�ͣ��Ƿ��ϴ������ǣ�",m_pModel->m_strModelName);
		int_selOK=AfxMessageBox(str,MB_OKCANCEL);	
	}
	if(int_selOK==1)
	{
		CString strIP,strPort;//����Զ��ģ�͵Ĳ�����ϢIP��Port
		ModelData modelDataRemote;//ɾ��Զ��
		modelDataRemote.ReadDate(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataRemote.GetIPAndPort(strIP,strPort);//����IP��Port
		modelDataRemote.DelTheModelTables(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		modelDataRemote.DelModelInModelListTable(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
	
		ModelData modelDataObj;
		modelDataObj.ReadDate(theApp.m_pConLocalModel,m_pModel->m_strModelName);
		modelDataObj.CreatTheModelTablesInMySql(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		if(bHased)
			modelDataObj.SetIPAndPort(strIP,strPort);//���ñ���IP��Port
		modelDataObj.WriteData(theApp.m_pConRemoteModel,m_pModel->m_strModelName);
		
		//�������п�������
		m_pMCalControl->SetConPtr(theApp.m_pConRemoteModel);
		m_pMCalControl->SaveAllCtrlDataToDB();
		m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);

		AfxMessageBox("���سɹ���");
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
		return;
	}
	CString sNodeName=m_pModel->m_strModelName;
	if(g_pSocketLogServ != NULL)
	{
		if(g_pSocketLogServ->IsConnect())
		{
			char lbuf[1024];	
			memset(lbuf,0,1024);

			lbuf[0] = '*';	//��ȫ�������
			*(int *)(lbuf+1) = 6;
			lbuf[5] = '9';	//Э���
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

	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6+sNodeName.GetLength();
	lbuf[5] = '7';	//Э���
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
		CString sMsg = "���ض˿���"+str;
		//AfxMessageBox(sMsg);
	}
	

	//����������CalcServ��ȡ��־֪ͨ
	g_SocketServPort = ::atoi(lbuf+6);

	g_pSocketLogServ = g_ServSocketInit(m_pModel->m_strModelIP,g_SocketServPort);
	if(!g_pSocketLogServ)
	{
		AfxMessageBox("���Ӹü���ģ��ʧ�ܣ�����ģ��δ������������ع���!");
		return;
	}

	//��������
	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6;
	lbuf[5] = '9';	//Э���
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

			lbuf[0] = '*';	//��ȫ�������
			*(int *)(lbuf+1) = 6;
			lbuf[5] = '0';	//Э���
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
	//����������CalcServ��ȡ����
	g_SocketServPort = ::atoi(lbuf+6);
	CSocketServ* pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,::atoi(m_pModel->m_strPort));
	if(!pSockServ)
	{
		AfxMessageBox("���Ӹü���ģ��ʧ�ܣ�����ģ��δ������������ع���!");
		return false;
	}
	//��������
	lbuf[0] = '*';	//��ȫ�������
	*(int *)(lbuf+1) = 6;
	lbuf[5] = '8';	//Э���
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
	
	theApp.m_pTreeItemObj->m_strName = "�����ռ���ͼ";
	theApp.m_pTreeItemObj->m_iItemType = 3;
	theApp.m_pView->PostMessage(WM_TASKGUID,30);
	return true;
}
unsigned __stdcall ThreadRefreshWSpace(void* pThreadData)
{
	while(TRUE)
	{
		if(g_bToExit)		//�˳��ź�
			break;
		int nRet = ::WaitForSingleObject(g_RunEvent,INFINITE);
		if(nRet == WAIT_FAILED)
		{
			break;
		}
		else if(nRet == WAIT_OBJECT_0)	
		{
			bool ret=g_RefreshWSpaceOnce(pThreadData);
			if(false==ret)//���ʧ����ֹͣˢ�¹����ռ����־
			{
				CEPerfManageDoc *pDoc = (CEPerfManageDoc*)theApp.m_pView->GetDocument();
				g_bToExit=true;
				CloseHandle(hThread);
				pDoc->m_bTagGettingWS=false;
				pDoc->MagEndGetLog();
				break;
			}
			//DoWork();
			::Sleep(5000);	//��������
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
		return;
	}
	if(!g_pSocketManage->IsConnect())
	{
		AfxMessageBox("���ӹ�����ʧ�ܣ�����δ������������ع��ϣ�");
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
			AfxMessageBox("����ˢ��ʧ��!");
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
		AfxMessageBox("��ģ��Ϊ����ģ�ͣ����Զ��ģ���ٽ��в�����");
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
	if((retDlg==1)||(retDlg==2))//2:ͬ����������1:ֻͬ����0��ȡ��
	{
		CString strIP,strPort;//����Զ��ģ�͵Ĳ�����ϢIP��Port
		ModelData modelDataRemote;//ɾ��Զ��
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
		
		//�������п�������
		m_pMCalControl->SetConPtr(theApp.m_pConRemoteModel);
		m_pMCalControl->SaveAllCtrlDataToDB();
		m_pMCalControl->SetConPtr(theApp.m_pConLocalModel);
		if(retDlg==2) //2:ͬ��������
		{
			//***ֹͣˢ��Զ�̹����ռ�
			g_bToExit=true;
			CloseHandle(hThread);
			m_bTagGettingWS=false;
			//***ֹͣˢ��Զ�̹����ռ�
			MagEndGetLog();//ֹͣˢ����־

			stopRemoteMode();
			Sleep(500);
			runRemoteMode();
		}
		AfxMessageBox("ͬ���ɹ���");
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
		AfxMessageBox("��ģ�����ݵ�����δ�����ɹ������ܴ�ģ��");
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
		AfxMessageBox("��ǰ�޷�ɾ��ģ�ͣ�");
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
		AfxMessageBox("��ѡ��Ҫɾ����ģ��!");
		return;
	}
	if(pWS->m_iItemType==1) //���ģ�鱻���
	{
		if(IDOK==AfxMessageBox("ȷ��Ҫɾ��ѡ��ģ�飿",MB_OKCANCEL))
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
		AfxMessageBox("��ѡ��Ҫɾ����ģ��!");
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
				if(pWS->m_iItemType==1) //���ģ�鱻���
				{
					pCmdUI->Enable(true);
				}
				else
					pCmdUI->Enable(false);

			}
		}
	}
}

//�������ؼ���-------------------------------------------------
//���з�ʽ��
//(1)  ʵʱ���У�
//(2)  ��ʷ����: ����ģ�ⷽʽ��ģ��ʵʱ�����ȶ�ȡ��ʷ���ݣ�Ȼ����ģ��ʵʱ������
//(3)  ��ʷ���㣺���÷�ʵʱ��ʽ��Ҳ���Ȼ����ȡ��ʷ���ݡ�
//(4)  ����һ�Σ���ʱ�̼��㣨��ǰ����ʷ����ʱ��Σ�ѡ����ʷʱ���

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
	// ���̲߳�����ֵ
	m_pLocalThreadPara->m_pModel = m_pModel;
	m_pLocalThreadPara->m_hWnd = ((CMainFrame*)AfxGetMainWnd())->m_hWnd;
    //-�����߳�-----------------------------------------------------------------------------------------------------------------------
	DWORD ThreadID;
	DWORD code;
	if(!GetExitCodeThread(m_hLocalThread,&code)||(code!=STILL_ACTIVE))
	{
		switch(m_pLocalThreadPara->CalcMode)
		{
		case 0:
		case 1:
			{
				m_hLocalThread=CreateThread(NULL,0,TRunInCycle,m_pLocalThreadPara,0,&ThreadID);//��������ʼ�߳�
				break;
			}
		case 2:
			{
				m_hLocalThread=CreateThread(NULL,0,TRunUserSetting,m_pLocalThreadPara,0,&ThreadID);//��������ʼ�߳�
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
//ֹͣ���ؼ���--------------------------------------------------
void CEPerfManageDoc::OnRunLocalStop() 
{
	// TODO: Add your command handler code here
	DWORD code;
	 if(GetExitCodeThread(m_hLocalThread,&code))
	 {
		if(code==STILL_ACTIVE)								                	//�����ǰ�̻߳��
		{
			TerminateThread(m_hLocalThread,0);						//ǰЩ��ֹ�߳�
			CloseHandle(m_hLocalThread);								//�����߳̾��
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
//������ģ��
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
		AfxMessageBox("�Ѵ�����ģ�飡");
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
