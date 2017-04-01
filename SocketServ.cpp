// SocketServ.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "SocketServ.h"
#include "MainFrm.h"
#include "PortTransferData.h"
#include "EPerfManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketServ

CSocketServ::CSocketServ()
{
}

CSocketServ::~CSocketServ()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketServ, CSocket)
	//{{AFX_MSG_MAP(CSocketServ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketServ member functions
extern char g_sNewLog[256];	//新日志
extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
#define WM_ADD_LOG			WM_USER+102
void g_PutPacketsToCharArr(char *pBuf,int count,CArray<char,char> &bufArr)
{
	for(int i=0;i<count;i++)
	{
		bufArr.Add(*(pBuf+i));
	}
}
BOOL g_ReceivePacketsToCharArr(CSocketServ *pSockServ,char *pFirst,int iFirstCount,CArray<char,char> &bufArr)
{
	int retReceive=0;
	//放入头
	g_PutPacketsToCharArr(pFirst,iFirstCount,bufArr);
	//循环放入数据
	int nObjectCount = *(int*)(pFirst+1);
	char bufDataHead[4];
	for(int i=0;i<nObjectCount;i++)
	{
		memset(bufDataHead,0,4);
		retReceive = pSockServ->Receive(bufDataHead,4);
		if(retReceive>0)
		{
			g_PutPacketsToCharArr(bufDataHead,4,bufArr);//放入数据头
			
			int oneDataSize=*(int*)(bufDataHead);
			if(oneDataSize>100000)//大于100000字节认为接受的包有误
			{
				bufArr.RemoveAll();
				return false;
			}
			if(oneDataSize<0)
			{
				bufArr.RemoveAll();
				return false;
			}
			
			char *pBufDataBody=new char[oneDataSize];
			memset(pBufDataBody,0,oneDataSize);
			retReceive = pSockServ->Receive(pBufDataBody,oneDataSize);
			if(retReceive>0)
			{
				g_PutPacketsToCharArr(pBufDataBody,oneDataSize,bufArr);//放入数据体
			}
			else
			{
				bufArr.RemoveAll();
				return FALSE;
			}
			if(pBufDataBody!=NULL)
			{
				delete pBufDataBody;
				pBufDataBody=NULL;
			}
		}
		else
		{
			bufArr.RemoveAll();
			return FALSE;
		}
	}
	//接收尾巴
	char bufDataTail[1];
	memset(bufDataTail,0,1);
	retReceive = pSockServ->Receive(bufDataTail,1);
	if(retReceive>0)
	{
		if(bufDataTail[0]!='#')
		{
			bufArr.RemoveAll();
			return FALSE;
		}
	}
	else
	{
		bufArr.RemoveAll();
		return FALSE;
	}
	return TRUE;
}
void g_PutCharArrToPCharByIndex(const CArray<char,char> &bufArr,int startIndex,int count,char *pBuf)
{
	for(int i=0;i<count;i++)
	{
		*(pBuf+i)=bufArr[startIndex+i];
	}
}
void g_PutCharArrToWSpace(const CArray<char,char> &bufArr)
{
	if(bufArr.GetSize()<6)
		return;
	char bufObjCount[4];
	memset(bufObjCount,0,4);
	g_PutCharArrToPCharByIndex(bufArr,1,4,bufObjCount);
	int nObjectCount = *(int*)(bufObjCount);

	char bufDataSize[4];
	int tag=6;//第一个数据包size的开始位是6
	for(int i=0;i<nObjectCount;i++)
	{
		memset(bufDataSize,0,4);
		g_PutCharArrToPCharByIndex(bufArr,tag,4,bufDataSize);
		tag+=4;
		int nDataSize = *(int*)(bufDataSize);
		char *pBufData=new char[nDataSize];
		memset(pBufData,0,nDataSize);
		g_PutCharArrToPCharByIndex(bufArr,tag,nDataSize,pBufData);
		tag+=nDataSize;
		CPortTransferData transferData;
		memcpy(&transferData,pBufData,nDataSize);

		CString strKey=transferData.m_cKey;
		CCalcPort *pPortData=g_WorkSpace[strKey];			
		transferData.CopyValueToPortObj(pPortData);
		if(pBufData!=NULL)
		{
			delete pBufData;
			pBufData=NULL;
		}
	}
}
void CSocketServ::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char lbufHead[6];
	memset(lbufHead,0,6);

	int ret = Receive(lbufHead,6);
	if(ret > 0)
	{
		if(lbufHead[0] != '*')
		{
			return;
		}
		if(lbufHead[5] == '9')
		{
			char lbufBody[1024-6];
			memset(lbufBody,0,1024-6);
			ret = Receive(lbufBody,1024-6);
			if(ret>0)
			{
				strcpy(g_sNewLog,lbufBody);
				CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
				pMain->SendMessage(WM_MSG_MODELRUN_LOG, 0, (LPARAM)g_sNewLog);
			}
			return;
		}
		else if(lbufHead[5] == '8')
		{
			CArray<char,char> bufArr;
			g_ReceivePacketsToCharArr(this,lbufHead,6,bufArr);
			g_PutCharArrToWSpace(bufArr);
			bufArr.RemoveAll();
			CEPerfManageDoc *pDoc = (CEPerfManageDoc *)theApp.m_pView->GetDocument();
			pDoc->m_pModel->GetBlockInputValueFromGlobalWS();
		}
		else
			return;
		//OnLogArive();
	}	
	theApp.m_pTreeItemObj->m_strName = "实时测点视图";
	theApp.m_pTreeItemObj->m_iItemType = 1;
	theApp.m_pView->SendMessage(WM_TASKGUID,30);

	theApp.m_pTreeItemObj->m_strName = "关系测点视图";
	theApp.m_pTreeItemObj->m_iItemType = 2;
	theApp.m_pView->SendMessage(WM_TASKGUID,30);

	theApp.m_pTreeItemObj->m_strName = "工作空间视图";
	theApp.m_pTreeItemObj->m_iItemType = 3;
	theApp.m_pView->SendMessage(WM_TASKGUID,30);
	CSocket::OnReceive(nErrorCode);
}

BOOL CSocketServ::IsConnect()
{
	BOOL RET = true;
	char lbuf[1024];	
	memset(lbuf,0,1024);

	lbuf[0] = '*';	//安全传输符号
	*(int *)(lbuf+1) = 6;
	lbuf[5] = 'A';	//协议号 心跳包

	int ret = Send(lbuf,1024);

	do
	{
		ret = Receive(lbuf,1024);
		if(ret > 0)
		{
			if(lbuf[0] != '*')
			{
				RET = false;
				break;
			}
			//CString str(lbuf+6);
			//AfxMessageBox(str);
			if(lbuf[5] != 'A')
			{
				RET = false;
				break;
			}
		}
		else
			RET = false;
	}
	while(0);
	return RET;
}
