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

	CRITICAL_SECTION CriticMem;					//�����ڴ����Ļ������
	CRITICAL_SECTION CriticFileName;			//���ڶ��ļ��������Ļ������
	PER_IO_OPERATION_DATA* m_IoFreePoint;		//��������ͷָ��
	int m_nCurrentIoBuffNum;					//��ǰ���������нڵ������
	char m_pLogPath[1024];
	HANDLE m_hFile;				//�ļ�ָ��
	DWORD  m_position;			//�ļ�λ��
	HANDLE m_CompletionPort;	//IO��ɶ˿�
	HANDLE m_CloseEvent;		//�ر��¼�
	bool m_IsClose;				//�رձ���
	int m_nSaveDays;				//���������
	char m_nProccessName[48];
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nFileNo;
};
