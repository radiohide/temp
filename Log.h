#pragma once

#define DATA_BUFSIZE 512
#define IONUM 20
#define SAVEDAYS 31
#define LOGFILELENGTH 20*1024*1024
enum IOCode
{
	IORead,
	IOWrite
};

struct PER_IO_OPERATION_DATA
{
	OVERLAPPED   Overlapped;
	CHAR		 Buf[DATA_BUFSIZE];
	IOCode		 CmdCode;
	DWORD        nNumberOfBytesToSend;
	DWORD        nNumberOfBytesSent;
	PER_IO_OPERATION_DATA* next;

};

class CLog
{
public:

	CLog(void);
	~CLog(void);
	bool InitLog(char* pDataBasePath, char* pDataBaseName, int nSaveDays);
	bool WriteFileData(CString strLogNote);
	bool SetSaveDays(int nDays);
	int GetSaveDays();
	static DWORD WINAPI ServerWorkerThread(LPVOID file);
private:

	PER_IO_OPERATION_DATA*  NewLogIOBuff();
	void FreeLogIOBuff(PER_IO_OPERATION_DATA*& pIoBuff);

	CRITICAL_SECTION CriticMem;					//用于内存分配的互斥操作
	CRITICAL_SECTION CriticFileName;			//用于对文件名变量的互斥操作
	PER_IO_OPERATION_DATA* m_IoFreePoint;		//自由链表头指针
	int m_nCurrentIoBuffNum;					//当前自由链表中节点的数量
	char m_pLogPath[1024];
	HANDLE m_hFile;				//文件指针
	DWORD  m_position;			//文件位置
	HANDLE m_CompletionPort;	//IO完成端口
	HANDLE m_CloseEvent;		//关闭事件
	bool m_IsClose;				//关闭变量
	int m_nSaveDays;				//保存的天数
	char m_nProccessName[48];
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nFileNo;
};
