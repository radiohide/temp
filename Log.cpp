#include "StdAfx.h"
#include ".\log.h"


CLog::CLog(void)
{
	InitializeCriticalSection(&CriticMem);
	InitializeCriticalSection(&CriticFileName);
	m_CloseEvent = CreateEvent(NULL, true, false, NULL);
	m_IsClose = false;
	m_IoFreePoint = NULL;
	m_nSaveDays = SAVEDAYS;
	m_nFileNo=0;
}

CLog::~CLog(void)
{
	m_IsClose = true;
	CloseHandle(m_CompletionPort);
	CloseHandle(m_hFile);

	EnterCriticalSection(&CriticMem);

	PER_IO_OPERATION_DATA* pTempPoint = NULL;
	while(NULL != m_IoFreePoint)
	{
		pTempPoint = m_IoFreePoint;
		m_IoFreePoint = m_IoFreePoint->next;
		delete pTempPoint;
		pTempPoint = NULL;
	}

	LeaveCriticalSection(&CriticMem);

	DeleteCriticalSection(&CriticMem);
	DeleteCriticalSection(&CriticFileName);

	WaitForSingleObject(m_CloseEvent, 2000);
	Sleep(50);
	CloseHandle(m_CloseEvent);
	
	//CloseHandle(m_CompletionPort);
}



bool CLog::InitLog(char* pDataBasePath, char* pDataBaseName, int nSaveDays)
{
	strcpy(m_nProccessName,pDataBaseName);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	m_nSaveDays = nSaveDays;
	sprintf(m_pLogPath, "%s\\Log\0", pDataBasePath);
	//建立Log文件夹
	int n=CreateDirectory(m_pLogPath, NULL);
	if(n==0)
		int err=GetLastError();
	CTime Date;
	Date = CTime::GetCurrentTime();
	m_nYear = Date.GetYear();
	m_nMonth = Date.GetMonth();
	m_nDay = Date.GetDay();
	char pPath[1024];
	sprintf(pPath, "%s\\%d\0", m_pLogPath, m_nYear);
	CreateDirectory(pPath, NULL);
	sprintf(pPath, "%s\\%s\0", pPath, pDataBaseName);
	CreateDirectory(pPath, NULL);
	char pFullPath[1024];
	int nindex =  m_nMonth*100 + m_nDay;
	sprintf(pFullPath, "%s\\%d.txt", pPath, nindex);
	printf("%s\n",pFullPath);
	m_hFile = ::CreateFile(pFullPath,
							GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							NULL);

	if (m_hFile ==  INVALID_HANDLE_VALUE)
	{
		int e=::GetLastError();
		printf("Error Create File,err is %d\n",e);
		return false;
	}
	if(NULL == (m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)))
	{
		printf("Create CompleteIo1  Failed\n");
		return false;
	}
    if(NULL == (CreateIoCompletionPort((HANDLE)m_hFile, m_CompletionPort, (DWORD)m_hFile, 0)))
	{
		printf("Create CompleteIo2  Failed\n");
		return false;
	}
	if (NULL == (ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)))
	{
		printf("Create Thread Failed\n");
		return false;
	}
	CloseHandle(ThreadHandle);
	//预先分配IO空间
	m_IoFreePoint = NULL;
	PER_IO_OPERATION_DATA* pTempPoint = NULL;
	m_nCurrentIoBuffNum = 0;
	for(int i=0; i<IONUM; i++)
	{
		if(NULL == m_IoFreePoint)
		{
			m_IoFreePoint = new PER_IO_OPERATION_DATA;
			m_IoFreePoint->next = NULL;
			m_nCurrentIoBuffNum++;
		}
		else
		{
			pTempPoint = new PER_IO_OPERATION_DATA;
			pTempPoint->next =  m_IoFreePoint;
			m_IoFreePoint = pTempPoint;
			m_nCurrentIoBuffNum++;
		}
	}
	DWORD nHighOffset = 0;
	m_position = ::GetFileSize(m_hFile, &nHighOffset);
	return true;
}


bool CLog::WriteFileData(CString strLogNote)
{
	static nIndex = 0;
	COleDateTime DelFileDate;
	COleDateTimeSpan RelativeDays;
	HANDLE hfile;
	int nError = 0;
	EnterCriticalSection(&CriticFileName);
	CTime Date;
	Date = CTime::GetCurrentTime();
	int nDay=Date.GetDay();
	TRACE("nDay=%d\n",nDay);
	//用于删除文件的变量申明
	int nY = 0;
	int nFileNo = 0;
	int nTempSaveDays = m_nSaveDays;
	bool bDelContinue = true;
	//判断是否翻年
	if(m_nYear != Date.GetYear())
	{
		m_nYear = Date.GetYear();
		m_nFileNo=0;
		char pPath[1024];
		sprintf(pPath, "%s\\%d\\%s\0", m_pLogPath, m_nYear,m_nProccessName);
		CreateDirectory(pPath, NULL);			//创建年的目录
		m_nMonth = Date.GetMonth();
		m_nDay = Date.GetDay();
		
		char pFullPath[1024];
		m_position = 0;
		nIndex = m_nMonth * 100 + m_nDay;
		sprintf(pFullPath, "%s\\%d.txt\0", pPath, nIndex);
		hfile = ::CreateFile(pFullPath,
							 GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
							 NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							 NULL);
		 if(NULL == (CreateIoCompletionPort((HANDLE)hfile, m_CompletionPort, (DWORD)hfile, 0)))
		 {
			printf("Create CompleteIo2  Failed\n");
		 }
		 CloseHandle(m_hFile);
		 m_hFile = hfile;
		 //删除超出保留天数的文件
		 while(1)
		 {
			DelFileDate.SetDateTime(m_nYear, m_nMonth, m_nDay, 0, 0, 0);
			RelativeDays.SetDateTimeSpan(nTempSaveDays + 1, 0, 0, 0);
			DelFileDate = DelFileDate - RelativeDays;
			nY = DelFileDate.GetYear();
			nFileNo = DelFileDate.GetMonth() *100 + DelFileDate.GetDay();
			sprintf(pFullPath, "%s\\%d\\%s\\%d.txt", m_pLogPath, nY, m_nProccessName,nFileNo);
			try
			{
				CFile::Remove(pFullPath);
			}
			catch(CFileException* pEx)
			{
				pEx->Delete();
				bDelContinue = false;
			}
			if(false == bDelContinue)
			{
				break;
			}
			else
			{
				nTempSaveDays++;
			}
		 }
	}
	else if(m_nDay != Date.GetDay())	//查看是否翻天
	{
		m_nFileNo=0;
		m_nMonth = Date.GetMonth();
		m_nDay = Date.GetDay();
		m_position = 0;
		nIndex = m_nMonth * 100 + m_nDay;
		char pFullPath[256];
		sprintf(pFullPath, "%s\\%d\\%s\\%d.txt", m_pLogPath, m_nYear, m_nProccessName,nIndex);
		hfile = ::CreateFile(pFullPath,
							 GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
							 NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							 NULL);
		 if(NULL == (CreateIoCompletionPort((HANDLE)hfile, m_CompletionPort, (DWORD)hfile, 0)))
		 {
			printf("Create CompleteIo2  Failed\n");
		 }
		 CloseHandle(m_hFile);
		 m_hFile = hfile;
		 //删除超过记录天数的文件
		 while(1)
		 {
			DelFileDate.SetDateTime(m_nYear, m_nMonth, m_nDay, 0, 0, 0);
			RelativeDays.SetDateTimeSpan(nTempSaveDays + 1, 0, 0, 0);
			DelFileDate = DelFileDate - RelativeDays;
			nY = DelFileDate.GetYear();
			nFileNo = DelFileDate.GetMonth() *100 + DelFileDate.GetDay();
			sprintf(pFullPath, "%s\\%d\\%s\\%d.txt", m_pLogPath, nY, m_nProccessName,nFileNo);
			try
			{
				CFile::Remove(pFullPath);
			}
			catch(CFileException* pEx)
			{
				pEx->Delete();
				bDelContinue = false;
			}
			if(false == bDelContinue)
			{
				break;
			}
			else
			{
				nTempSaveDays++;
			}
		 }
	}
	
	LeaveCriticalSection(&CriticFileName);
	
	//分配空间，拷贝记录
	PER_IO_OPERATION_DATA* PerIoData;
	PerIoData = NewLogIOBuff();
	if (PerIoData == NULL)
	{
		return false;
	}
	ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
	ZeroMemory(PerIoData->Buf, DATA_BUFSIZE);
	CString strLog;
	strLog.Format("%d-%d-%d %d:%d:%d   ", m_nYear, m_nMonth, m_nDay, Date.GetHour(), Date.GetMinute(), Date.GetSecond());
	strLog = strLog + strLogNote + "\r\n";
	int nLen = strLog.GetLength();
	memcpy(PerIoData->Buf, strLog, nLen);
	PerIoData->CmdCode              = IOWrite;
	PerIoData->nNumberOfBytesToSend = nLen;
	PerIoData->nNumberOfBytesSent   = 0;
//add by malingzhi
	///////////////////
	if(m_position>LOGFILELENGTH)
	{
		char pPath[1024];
		sprintf(pPath, "%s\\%d\\%s\0", m_pLogPath, m_nYear,m_nProccessName);
		CreateDirectory(pPath, NULL);			//创建年的目录
		m_nMonth = Date.GetMonth();
		m_nDay = Date.GetDay();
		
		char pFullPath[1024];
		m_position = 0;
		nIndex = m_nMonth * 100 + m_nDay;
		sprintf(pFullPath, "%s\\%d_%d.txt\0", pPath, nIndex,m_nFileNo++);
		hfile = ::CreateFile(pFullPath,
							 GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
							 NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							 NULL);
		 if(NULL == (CreateIoCompletionPort((HANDLE)hfile, m_CompletionPort, (DWORD)hfile, 0)))
		 {
			printf("Create CompleteIo2  Failed\n");
		 }
		 CloseHandle(m_hFile);
		 m_hFile = hfile;
		 m_position=0;
	}
	///////////

	//异步把数据存入文件
	PerIoData->Overlapped.Offset  = m_position;
	m_position = m_position + nLen;
	if (!WriteFile(m_hFile, PerIoData->Buf, nLen, NULL, (OVERLAPPED*)&(PerIoData->Overlapped)))
	{
		nError = GetLastError();
		if (ERROR_IO_PENDING != nError)
		{
			printf("write file err,%d\n",nError);
			return false;
		}
	}
//	printf("write success\n");
	return true;
}

bool CLog::SetSaveDays(int nDays)
{
	if (nDays<0)
	{
		return false;
	}
	EnterCriticalSection(&CriticFileName);
	m_nSaveDays = nDays;
	LeaveCriticalSection(&CriticFileName);
	return true;
}

int CLog::GetSaveDays()
{
	int nDays;
	EnterCriticalSection(&CriticFileName);
	nDays = m_nSaveDays;
	LeaveCriticalSection(&CriticFileName);
	return nDays;
}

PER_IO_OPERATION_DATA* CLog::NewLogIOBuff()
{
	//static nnn = 0;
	PER_IO_OPERATION_DATA* pTempPoint = NULL;
	
	EnterCriticalSection(&CriticMem);

	if(NULL != m_IoFreePoint)
	{
		pTempPoint = m_IoFreePoint;
		m_IoFreePoint = m_IoFreePoint->next;
		pTempPoint->next = NULL;
		m_nCurrentIoBuffNum--;
	}
	else
	{
		pTempPoint = new PER_IO_OPERATION_DATA;
		pTempPoint->next = NULL;
	}

	LeaveCriticalSection(&CriticMem);

	//TRACE("New--%d\n", nnn++);
	return pTempPoint;
}

void CLog::FreeLogIOBuff(PER_IO_OPERATION_DATA*& pIoBuff)
{
	//static nnn = 0;
	
	EnterCriticalSection(&CriticMem);
	
	if(IONUM > m_nCurrentIoBuffNum)
	{
		pIoBuff->next = m_IoFreePoint;
		m_IoFreePoint = pIoBuff;
		m_nCurrentIoBuffNum++;
		//TRACE("Back--%d\n", nnn++);
	}
	else
	{
		delete pIoBuff;
		pIoBuff = NULL;
		//TRACE("Del--%d\n", nnn++);
	}

	LeaveCriticalSection(&CriticMem);
}

DWORD WINAPI CLog::ServerWorkerThread(LPVOID file)
{
	CLog* Iofile = (CLog*) file;
	//static nnn = 0;
	HANDLE CompletionPort = (HANDLE) Iofile->m_CompletionPort;
	while (true)
	{
		DWORD BytesTransferred  = 0;
		DWORD CompleteKey       = 0;
		PER_IO_OPERATION_DATA* PerIoData;
		int ret = GetQueuedCompletionStatus(CompletionPort,
											&BytesTransferred,
											(LPDWORD)&CompleteKey,
											(LPOVERLAPPED *)&PerIoData,
											1000);
		if (ret == 0)
		{
			// error handling 
		}
		else if(PerIoData->CmdCode == IOWrite)
		{
			//TRACE("End--%d\n", nnn++);
			Iofile->FreeLogIOBuff(PerIoData);
			continue;
		}
		if(true == Iofile->m_IsClose)
		{
			SetEvent(Iofile->m_CloseEvent);
			return 1;
		}
	}
}
