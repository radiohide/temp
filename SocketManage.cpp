// SocketManage.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "SocketManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketManage

CSocketManage::CSocketManage()
{
}

CSocketManage::~CSocketManage()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketManage, CSocket)
	//{{AFX_MSG_MAP(CSocketManage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketManage member functions

void CSocketManage::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char lbuf[1024];	
	memset(lbuf,0,1024);

	int ret = Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != '6')
		{
			//AfxMessageBox("hehhe,here");
			return;
		}

		CString str(lbuf+7);
		CString sMsg;	sMsg.Format("%s状态发生变化,变为%d",str,lbuf[6]);
		AfxMessageBox(sMsg);

		//OnStateChange();
		

	}
	
	CSocket::OnReceive(nErrorCode);
}
BOOL CSocketManage::IsConnect()
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