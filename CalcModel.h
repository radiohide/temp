// CalcModel.h: interface for the CCalcModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_)
#define AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_
//http://www.baidu.com/s?wd=%E5%9C%A8%E7%BA%BF%E7%BF%BB%E8%AF%91&rsv_bp=0&rsv_spt=3&ie=utf-8&rsv_sug3=1&rsv_sug1=1&rsv_sug4=19&oq=%E5%9C%A8%E7%BA%BF&rsp=0&f=3&rsv_sug5=0&rsv_sug=0&rsv_sug2=0&inputT=3750
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//����ģ���࣬��ģ��Ӧ���ǿ�����̬����һ�������ɸ��ּ��㣻
/*
����һ���������ռ䣬���ɺʹ����������
����һ��ģ�ͱ����ռ䣬���ɺʹ��������
����ģ�͵ı������ñ��������ǲ�������������������������������
��������ģ���ɶ��ģ����ɣ�ÿ��ģ���ɶ������飨�������������ϵ�Ŀ飩��ɣ�

ģ�͵Ľ������̣�
��1������ģ�͵����������ģ�ͼ�����Ҫ��Щ�����������㡢��������
��2������ģ�ͼ����ģ�飺����Щģ����ɣ�
��3������ÿ��ģ��ļ���顣ÿ������飬����������������������ģ�ͱ��������ռ�WS,��������ŵ�WS����
�γɹ���

���罫���ܼ����Ϊ��
��1������ȡ��Ԥ����ģ�飺��ģ�齫������ݱ���룬Ȼ����
��1��ú�ʴ���ģ�飺��ģ�ͱ����ռ�
*/

//����һ������ģ��,����һ��ģ����б�ģ��ļ���˳����Ե�����
//���㻹���Խ������̿��ƣ�������ģ�����������ƺ����ļ������
//����������쳣�ж�ģ�飬����˳��Ϊ1�����״̬��ģ�Ͳ��б�
/*
ģ�ͼ���Ŀ��Ʒ�����
	 //�����ʱ�򣬱�������ȷ�������˳��˳��ȷ�������ַ���
	 //m_pModel->Calc();
	//��ģ��Ҫ�����ʱ����Ҫ�����ǰ�������ǣ�
	//1. ʵʱ���㣨���ã���ʵʱ������һ��ʱ�����м��㣬��ģ�������ȡ�������ǵĹ���
	//2. ��ʱ���㣨ͳ�ƣ���ÿ��ʲôʱ���㣬ÿ�ܣ�ÿ��
	//3.����һ�Σ�����ǰʱ�����һ�Σ�����ʷʱ�̼���һ��
	//4.�������㣺����ĳ���������м��㣬ģ�ͱ�����԰���һ�������жϣ����Ƚ��������ļ��������
	//5.�û��������㣺ģ�����к��൱��һ�����񣬼���Ľ��
	//6.����ʷ���ݽ�����ʵʱ���㣻
	//7. ��ʷ���ݶν��м����Σ�20�죬ÿ����һ�Σ��Զ�������ϣ�������ʱ��Σ�Ȼ����м���
	//ϵͳ�����Ǹ������趨�ļ��㷽ʽ���еģ������뵽����״̬��
	//��ο��Ƽ��㣬ģ������������з�ʽ����Ҳ���룬Ҳ��ȷ���������е�ģʽ�����ⲿ�����Ϳ��ƣ�
	//��һ�����Ƽ����ģ�飬����ģ�͵ļ���ģʽ
1��ģ�͵ļ���ģʽ��ʵʱ����ʱ������������ʵʱ���������Ķ�ʱ��
2��ģ�ͼ����ģʽ����
ģʽ1��ʵʱ����������ʵʱ��
-------------------------------------------------------
���ⲿ���Ƽ�������������ڲ����ȼ����������ģ�飬��ģ����һ���̶���ģ�飬���յ�ģ����Ҫ���һ���źţ��̶����ƣ���
�����Ƿ�ִ�к����ļ��㡣Ȼ�������һ���������ڡ�
a������ʵʱ��ȡ���ݵ�Դģ�飬��ʱ�䵽ʱ�����м��㣬����ɶ�ȡʵʱ��ֵ��ʵʱʱ����ǰ��һ��ʱ��ε�ƽ��ֵ��
b�����ڶ�ȡ��ʷ���ݶε�Դģ�飬��ʱ�䵽ʱ��ʱ��Ϊ��ǰʱ��t���������ȡ��Ϊ����ǰt��ǰ��DTʱ��ε����С�
ģʽ2����ʱ���������Ķ�ʱ��
--------------------------------------------------------��
���ⲿ���ƶ�ʱ����ʱ����ʱ�������ǽ��������ļ��㣬ͬ�ϡ�Ȼ�����ȴ���һ����ʱ��
ģʽ3����ʷģʽ����ʵʱ��--------------------------------------------------------
�ⲿ���ƶ�ȡ��ʷ��ʱ��Σ������Ŀ���Ǽ���ÿ����ʷʱ�̵�����ֵ���ⲿ������ʷ�ĸ���ʱ�̣���ÿ��ʱ�̾����㡣
ģʽ4����ʷģʽ��ʱ��Σ�����һ�λ򰴼���Ķ�Σ�
�ⲿ����ʱ��ε������յ㣬Դģ���ȡ����һ��ʱ������ݣ�����������,Ȼ����������н��д����ģ�����μ��㣬
����ʵʱԴģ����˵���������ȡ��Ϊ���Ǽ���ʱ�������㡢�յ㡢��ֵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////
���õ�ʾ����
ʵʱ��ʽ��StartRTProcess();  
����ʽ��StartTaskProcess();
��ʷ����ʵʱ��ʽ�������ķ���1���ֶ�ѡ��ʱ��Σ���ʱ������ģ�͸�ֵ����ʷʱ�̣�,�����ⲿ��
                                  ��������2����ʱ��������ÿ��1ʱ���㣬ÿ�����һ�������£�ÿֵ�������ֵ��
��ʷ��ʱ��η�ʽ��

������ʽ��
��1���Զ���ʽ: ����ģ�����к󣬼�����һ��ʵʱ�̣߳���ʱ�ж������ּ������������㣨�磩
1.1 �Զ����������ʵʱ���㣬��Ҫ�趨��ʱ����
1.2 �Զ������������ʷ����
  >> �ƻ������ʱ���Ƿ񵽴
  >>��ʱʱ���Ƿ�ﵽ��
  �˳�������ģ��ֹͣ��ʶΪ�棻�ޣ�һֱ���㣩������ʱ�䳤�����ƣ�
  ��ͣ���ƣ���ͣ
  (2) �ֶ���ʽ��ϵͳ�����������û�������ƣ������кܶ���ƣ��������û�����ʽ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#include "CalcModule.h"
#include "CalcBlock.h"
///////////////////////////////////////////////
enum MODEL_RUN_MODE
{
	RUN_MODE_DEBUG=0, //	����
    RUN_MODE_REALTIME, //ʵʱ����
	RUN_MODE_SCHEDULETASK, //�ƻ�����
	RUN_MODE_INTERACTIVE  //����ʽ����Ҫ�˲���
};
enum MODEL_RUN_TIMEMODE
{
	RUN_TIMEMODE_SPAN=0, //ʱ�μ���
	RUN_TIMEMODE_MOMENT //	ʱ��
    
};
enum MODEL_RUN_TASKMODE
{
	RUN_TASKMODE_EVERYDAY=0,
	RUN_TASKMODE_EVERYWEEK,
	RUN_TASKMODE_EVERYMONTH
};
enum MODEL_RUN_STATE //ģ�͵�
{
	RUN_STATE_RUNNING=0,
	RUN_STATE_STOPPED,
	RUN_STATE_PAUSE
};
//�ⲿ��ģ�ͼ���ʱ�Ŀ��Ʋ�����ģ�ͼ���ʱ���ݿ��Ʋ������м���
typedef struct CalcControl_struct{
	
	int  RunMode; // ʵʱ�����񡢽���
	//����ʵʱ�Ĳ���
	int  RealCalc_Cycle; //ʵʱ����
	//���ڼƻ�����Ĳ���
	int  TaskFrequence; //Ƶ�ʣ�every day,every week, every montch
	int  TaskDate;   //ÿ��ʱ��Ч��ÿ��ʱΪ�ܼ���ÿ��ʱΪ���ںŻ����һ��
	CTime  TaskTime; //�����ʱ�䣬ÿ��ļ���
	//���ڽ���ʽ�ļ���
    //---------------------------------------------------------------------
	//����ʱ��ʱ��ģʽ,��ʵʱʱ���ÿ��ǣ����ƻ�����ʱ��Ҫ�����Զ���������⣬����Ϊ��
	//��ÿ��8�����һ�Σ�ʱ�����Ϊ����ʱʵʱ�����㣩����ǰһ���0�㵽24�㣨����Ϊ���/��ʱ�Σ�
	//ÿ�¿�ʼ����1��1�㣬�����ϸ���1�ŵ���ĩ��������㡢��Сʱ����
	int  Task_ForwardTimeCount ; //ǰ���ٸ�ʱ�䵥λ��ǰһ�죬ǰһ���£�
    
	int       HistSpan_TimeMode; //ָ����ʱ�̡�ʱ��
	long    HistSpan_TimeInterval; //��λΪ�룬ָ��ʱ�̺�ʱ�λ��ּ����0�����Σ������֣�����Ϊʱ��ļ��ʱ�� 
	//������ʷʱ��Ŀ��Ʋ�����ʱ��Σ�ʱ�̼���/ʱ�η�ʽ
	CTime    HistSpan_lTimeBegin; //����Ҫ�����ʱ������
	CTime    HistSpan_lTimeEnd;   //����Ҫ�����ʱ����յ�
	
   
}Model_CalcControl;
//ÿ��ģ�ͼ�����Ҫ�趨��ʱ������
typedef struct CalcModel_GlobalTime_struct{
	CTime tmCurHistBegin; //��ǰ�����ʱ��εĿ�ʼ 
	CTime tmCurHistEnd;  //��ǰ�����ʱ��ν���
	
	
}Model_CalcGlobalTimeVars;
///////////////////////////////////////////////
class CCalcModel : public CObject  
{
public:
	CCalcModel();
	virtual ~CCalcModel();
	void AddModule(CCalcModule* pModule);
	int LookUpMoudle(CString name,CString desp);//��Ҫ���ֺ����������ظ�

	//
	void CheckBlockCalcIntegrity(CStringArray& LogStrings);
	void AdjustModuleOrder();//����ģ��Ĵ���ֵ������ģ���˳��
	void AdjustCalcSequence(); //���ݸ���ĺ��룬���������˳��
	void InitforCalc();
	void Calc();
	void  ResetCalc();//�ظ�����ʱ��Ҫ���и�λ
	void  WriteDataToRTDB();//д���ݵ�ʵʱ��ʷ��
    Model_CalcControl m_CalcControlPara;
	Model_CalcGlobalTimeVars  m_GlobalTimeVars; //ģ�͵�ǰ��ʱ�����ԣ����Ա��κμ����������

	int m_iBackCalcTag;//�����ʶ
	long m_lDataTime;//ģ�ʹ������ݵ�ʱ���
	long m_lDataSTime;//ģ�ʹ������ݵ�ʱ������
	long m_lDataETime;//ģ�ʹ������ݵ�ʱ����յ�
	CString m_strModelName;
	CString m_strModelDesp;
	CString m_strModelIP;
	CString m_strPort;
	int m_iTagModeLocalOrRemote;//0:local;1:remote

	//��ϵ����Դ����
	_ConnectionPtr m_pConRDB1;
	_ConnectionPtr m_pConRDB2;
	_ConnectionPtr m_pConRDB3;
	_ConnectionPtr m_pConRDB4;
	_ConnectionPtr m_pConRDB5;
	_ConnectionPtr m_pConRDB6;
	_ConnectionPtr m_pConRDB7;
	_ConnectionPtr m_pConRDB8;
	_ConnectionPtr m_pConRDB9;
	_ConnectionPtr m_pConRDB10;
	_ConnectionPtr m_pConRDB11;
	_ConnectionPtr m_pConRDB12;
	_ConnectionPtr m_pConRDB13;
	_ConnectionPtr m_pConRDB14;
	_ConnectionPtr m_pConRDB15;
	_ConnectionPtr m_pConRDB16;
	_ConnectionPtr m_pConRDB17;
	_ConnectionPtr m_pConRDB18;
	_ConnectionPtr m_pConRDB19;
	_ConnectionPtr m_pConRDB20;
	_ConnectionPtr m_pConRDB21;
	_ConnectionPtr m_pConRDB22;
	_ConnectionPtr m_pConRDB23;
	_ConnectionPtr m_pConRDB24;
	_ConnectionPtr m_pConRDB25;
	_ConnectionPtr m_pConRDB26;
	_ConnectionPtr m_pConRDB27;
	_ConnectionPtr m_pConRDB28;
	_ConnectionPtr m_pConRDB29;
	_ConnectionPtr m_pConRDB30;
	_ConnectionPtr m_pConRDB31;
	_ConnectionPtr m_pConRDB32;
	_ConnectionPtr m_pConRDB33;
	_ConnectionPtr m_pConRDB34;
	_ConnectionPtr m_pConRDB35;
	_ConnectionPtr m_pConRDB36;
	_ConnectionPtr m_pConRDB37;
	_ConnectionPtr m_pConRDB38;
	_ConnectionPtr m_pConRDB39;
	_ConnectionPtr m_pConRDB40;
	_ConnectionPtr m_pConRDB41;
	_ConnectionPtr m_pConRDB42;
	_ConnectionPtr m_pConRDB43;
	_ConnectionPtr m_pConRDB44;
	_ConnectionPtr m_pConRDB45;
	_ConnectionPtr m_pConRDB46;
	_ConnectionPtr m_pConRDB47;
	_ConnectionPtr m_pConRDB48;
	_ConnectionPtr m_pConRDB49;
	_ConnectionPtr m_pConRDB50;
	_ConnectionPtr m_pConRDB51;
	_ConnectionPtr m_pConRDB52;
	_ConnectionPtr m_pConRDB53;
	_ConnectionPtr m_pConRDB54;
	_ConnectionPtr m_pConRDB55;
	_ConnectionPtr m_pConRDB56;
	_ConnectionPtr m_pConRDB57;
	_ConnectionPtr m_pConRDB58;
	_ConnectionPtr m_pConRDB59;
	_ConnectionPtr m_pConRDB60;
	_ConnectionPtr m_pConRDB61;
	_ConnectionPtr m_pConRDB62;
	_ConnectionPtr m_pConRDB63;
	_ConnectionPtr m_pConRDB64;
	_ConnectionPtr m_pConRDB65;
	_ConnectionPtr m_pConRDB66;
	_ConnectionPtr m_pConRDB67;
	_ConnectionPtr m_pConRDB68;
	_ConnectionPtr m_pConRDB69;
	_ConnectionPtr m_pConRDB70;
	_ConnectionPtr m_pConRDB71;
	_ConnectionPtr m_pConRDB72;
	_ConnectionPtr m_pConRDB73;
	_ConnectionPtr m_pConRDB74;
	_ConnectionPtr m_pConRDB75;
	_ConnectionPtr m_pConRDB76;
	_ConnectionPtr m_pConRDB77;
	_ConnectionPtr m_pConRDB78;
	_ConnectionPtr m_pConRDB79;
	_ConnectionPtr m_pConRDB80;
	_ConnectionPtr m_pConRDB81;
	_ConnectionPtr m_pConRDB82;
	_ConnectionPtr m_pConRDB83;
	_ConnectionPtr m_pConRDB84;
	_ConnectionPtr m_pConRDB85;
	_ConnectionPtr m_pConRDB86;
	_ConnectionPtr m_pConRDB87;
	_ConnectionPtr m_pConRDB88;
	_ConnectionPtr m_pConRDB89;
	_ConnectionPtr m_pConRDB90;
	_ConnectionPtr m_pConRDB91;
	_ConnectionPtr m_pConRDB92;
	_ConnectionPtr m_pConRDB93;
	_ConnectionPtr m_pConRDB94;
	_ConnectionPtr m_pConRDB95;
	_ConnectionPtr m_pConRDB96;
	_ConnectionPtr m_pConRDB97;
	_ConnectionPtr m_pConRDB98;
	_ConnectionPtr m_pConRDB99;
	_ConnectionPtr m_pConRDB100;
	
	CArray<int,int> m_ArrSourceRDBTagCon;//1:���ӡ�0:δ����
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//����
	
	CStringArray m_ArrConDBIP;//�������ݿ�IP
	CStringArray m_ArrConDBName;//�������ݿ�����
	CStringArray m_ArrConDBUser;//�������ݿ��û���
	CStringArray m_ArrConDBPass;//�������ݿ�����

	InsertData *m_pInSertRtData;
	InsertData *m_pInSertRtDataBack;//debug
	int m_iWCounter;
	int m_iWAllCounter;
public:
	int   TestRDBCon(_ConnectionPtr &pCon);
	void EstablishSourceRDBConnect(_ConnectionPtr &pCon);
	void CloseAllSourceRDBCon();
	_ConnectionPtr& GetConPtrRefByIndex(int index);
	_ConnectionPtr& GetSourceRDBByIndex(int index,int &beCon,CString &dbType);
	_ConnectionPtr& GetSourceRDBByDesp(CString strDesp,int &tagBeCon,CString &dbType);
private:
	void closeConnectPrt(_ConnectionPtr &pCon);
	void  printDebugData(InsertData *pData,int count);//debug
public:
	void DelModuleByName(CString strName);
	void GetDataTheTime(long &lTime);
	void GetDateSETime(long &lSTime,long &lETime);
	void SetCalcDataTime(long lCalcTime,long lDataSTime,long lDataETime);
	int GetBlockCount();
	int GetModuleCount();
	void GetBlockInputValueFromGlobalWS();
	void SetWriteLogOnOrOff(int iOnOrOff);//������־����
	void SetTraceOnOrOff(int iOnOrOff);//����׷�ٿ���
	void SetDebugOnOrOff(int iOnOrOff);//���õ��Կ���
	void RunCompile();
	void InitCompile(CWnd *pWnd,int iMsg,CLog *pLog);//��ʼ��������Ϣ
	void ResetStopCalc();//ֹͣ��־��λ//for stop
	void StopCalc();//for stop
	void InitControlPara();
	BOOL ExecuteBlock(CString strModuleName,CString strBlockName);
	int GetRunMode();
	void SetRunMode(int mode);
	void  SetRunState(int state);
	int GetRunState();
	
	void ResetModelContent();
	void ClearModuleBlocks(); //���ģ������м���飬�������ģ�鱾��

	CCalcModule* GetModuleByIndex(int index);
	CCalcModule* FindModuleByName(CString strModule);

	CCalcBlock* FindBlockByName(CString strBlock);
	CCalcBlock* FindBlockByName(CString strModule,CString strBlockName);
	//��ȡ�����ݿ�����
	BOOL ReadModel(_ConnectionPtr &pCon); //��ȡ�õ�ģ�Ͱ�����ģ��
	BOOL ReadModelDesp(_ConnectionPtr &pCon);//����ģ�͵����ֶ�ȡģ�͵�����
	BOOL LoadAllModuleBlocks(_ConnectionPtr &pCon); //��ȡ�õ�ģ���и�ģ��ļ����
	CModuleList m_ModuleList;
private:
	 CStringArray s_BlockTypeArray;
	 CStringArray s_BlockIDArray;

	 int m_iModelWarningCount;//�������
	 int m_iModelErrorCount;//�������
	 CWnd *m_pWnd;//��ñ�������й�����Ϣ������ڵľ��
	 int m_iMsg;//��ñ�������й�����Ϣ�������Ϣ���
	 CLog *m_pLog;//���������־
	 int m_iLogOnOrOff;//��־�����1:����0����
	 int m_iTraceOn;//׷�ٿ���1:����0:��
	 int m_iDebugOn;//���Կ���1:����0:��
//**************ʵʱ��ʷ��ӿ�****************
public:
     BOOL m_bLoadRTDBDll;//�Ƿ������RTDB�ӿ�
	 BOOL m_bConRTDB;//�Ƿ�������ʵʱ��ʷ������
	 BOOL m_bInitLoadAndConRTDB;//���й�ʵʱ��ʷ��dll���غ����ӵĽ���
	 HINSTANCE  hDLL;
public:
	 BOOL LoadRTDBDll();
	 BOOL InitRTDBConnect(HINSTANCE hDLL);
	 void LoadRTDBDllAndCon();
//**************ʵʱ��ʷ��ӿ�****************
protected:
	int   m_RunState; //����״̬�����С���ͣ��ֹͣ
private:
	int m_iStopTag;//0:���У�1:ֹͣ//for stop

};

#endif // !defined(AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_)
