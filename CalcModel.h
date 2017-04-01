// CalcModel.h: interface for the CCalcModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_)
#define AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_
//http://www.baidu.com/s?wd=%E5%9C%A8%E7%BA%BF%E7%BF%BB%E8%AF%91&rsv_bp=0&rsv_spt=3&ie=utf-8&rsv_sug3=1&rsv_sug1=1&rsv_sug4=19&oq=%E5%9C%A8%E7%BA%BF&rsp=0&f=3&rsv_sug5=0&rsv_sug=0&rsv_sug2=0&inputT=3750
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//计算模型类，该模型应该是可以组态，是一个可容纳各种计算；
/*
包含一个测点变量空间，容纳和处理测点变量；
包含一个模型变量空间，容纳和处理变量；
增加模型的变量，该变量可以是测点变量，计算变量（依赖与测点变量）；
整个计算模型由多个模块组成，每个模块由多个计算块（具有输入输出关系的块）组成；

模型的建立过程：
（1）建立模型的输入变量：模型计算需要哪些输入变量（测点、常量）；
（2）建立模型计算的模块：由哪些模块组成；
（3）建立每个模块的计算块。每个计算块，有输入和输出，输入来自于模型变量工作空间WS,块的输出则放到WS中以
形成共享。

例如将性能计算分为：
（1）测点读取与预处理模块：该模块将测点数据表读入，然后建立
（1）煤质处理模块：由模型变量空间
*/

//这是一个计算模型,包括一个模块的列表，模块的计算顺序可以调整。
//计算还可以进行流程控制，即根据模块的输出来控制后续的计算过程
//例如包括：异常判断模块，计算顺序为1；输出状态：模型层列表；
/*
模型计算的控制方法：
	 //计算的时候，必须首先确定计算的顺序，顺序确定有两种方法
	 //m_pModel->Calc();
	//当模型要计算的时候需要满足的前提条件是：
	//1. 实时计算（常用）：实时算则间隔一段时间后进行计算，各模块如何算取决于它们的功能
	//2. 定时计算（统计）：每天什么时候算，每周，每月
	//3.计算一次：按当前时间计算一次，按历史时刻计算一次
	//4.条件计算：满足某个条件进行计算，模型本身可以包含一个条件判断，首先进行条件的计算满足后；
	//5.用户触发计算：模型运行后，相当于一个服务，计算的结果
	//6.用历史数据进行类实时计算；
	//7. 历史数据段进行计算多段（20天，每天算一次，自动计算完毕），给定时间段，然后进行计算
	//系统运行是根据其设定的计算方式进行的，当进入到调试状态后。
	//如何控制计算，模型载入后，其运行方式参数也载入，也就确定了其运行的模式，由外部启动和控制；
	//有一个控制计算的模块，根据模型的计算模式
1）模型的计算模式：实时、定时、（有条件的实时、有条件的定时）
2）模型计算的模式参数
模式1：实时（有条件的实时）
-------------------------------------------------------
由外部控制计算的启动，但内部首先计算的是条件模块，该模块是一个固定的模块，最终的模块需要输出一个信号（固定名称），
决定是否执行后续的计算。然后进入下一个计算周期。
a）对于实时读取数据的源模块，当时间到时，进行计算，计算可读取实时的值、实时时刻往前推一个时间段的平均值。
b）对于读取历史数据段的源模块，当时间到时，时间为当前时间t，定义其读取行为：当前t往前推DT时间段的序列。
模式2：定时（有条件的定时）
--------------------------------------------------------、
由外部控制定时，定时到的时候首先是进行条件的计算，同上。然后进入等待下一个定时。
模式3：历史模式（类实时）--------------------------------------------------------
外部控制读取历史的时间段，计算的目标是计算每个历史时刻的数据值。外部给出历史的各个时刻，在每个时刻均计算。
模式4：历史模式（时间段，计算一次或按间隔的多次）
外部控制时间段的起点和终点，源模块读取的是一段时间的数据，是数据序列,然后对数据序列进行处理的模块依次计算，
对于实时源模块来说，定义其读取行为：是计算时间段内起点、终点、均值。
/////////////////////////////////////////////////////////////////////////////////////////////////////////
调用的示例：
实时方式：StartRTProcess();  
任务方式：StartTaskProcess();
历史的类实时方式：启动的方法1：手动选择时间段，按时间间隔给模型赋值（历史时刻）,控制外部。
                                  启动方法2：定时启动，如每天1时计算，每月最后一天计算该月，每值结束算该值。
历史的时间段方式：

启动方式：
（1）自动方式: 启动模型运行后，即启动一个实时线程，定时判断是哪种计算条件的满足（如）
1.1 自动启动后进行实时计算，需要设定：时间间隔
1.2 自动启动后进行历史计算
  >> 计划任务的时刻是否到达？
  >>定时时钟是否达到？
  退出条件：模型停止标识为真；无（一直计算）；运行时间长度限制；
  暂停机制：暂停
  (2) 手动方式，系统启动后，利用用户界面设计（可以有很多设计），满足用户交互式计算
////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#include "CalcModule.h"
#include "CalcBlock.h"
///////////////////////////////////////////////
enum MODEL_RUN_MODE
{
	RUN_MODE_DEBUG=0, //	调试
    RUN_MODE_REALTIME, //实时运行
	RUN_MODE_SCHEDULETASK, //计划任务
	RUN_MODE_INTERACTIVE  //交互式，需要人参与
};
enum MODEL_RUN_TIMEMODE
{
	RUN_TIMEMODE_SPAN=0, //时段计算
	RUN_TIMEMODE_MOMENT //	时刻
    
};
enum MODEL_RUN_TASKMODE
{
	RUN_TASKMODE_EVERYDAY=0,
	RUN_TASKMODE_EVERYWEEK,
	RUN_TASKMODE_EVERYMONTH
};
enum MODEL_RUN_STATE //模型的
{
	RUN_STATE_RUNNING=0,
	RUN_STATE_STOPPED,
	RUN_STATE_PAUSE
};
//外部给模型计算时的控制参数，模型计算时根据控制参数进行计算
typedef struct CalcControl_struct{
	
	int  RunMode; // 实时、任务、交互
	//关于实时的参数
	int  RealCalc_Cycle; //实时周期
	//关于计划任务的参数
	int  TaskFrequence; //频率，every day,every week, every montch
	int  TaskDate;   //每天时无效，每周时为周几，每月时为日期号或最后一天
	CTime  TaskTime; //任务的时间，每天的几点
	//关于交互式的计算
    //---------------------------------------------------------------------
	//运行时的时间模式,当实时时不用考虑，当计划任务时，要考虑自动计算的问题，种类为：
	//如每天8点计算一次，时间规则为：定时实时（计算），算前一天从0点到24点（类型为间隔/、时段）
	//每月开始，第1天1点，计算上个月1号到月末、按天计算、按小时计算
	int  Task_ForwardTimeCount ; //前多少个时间单位，前一天，前一个月，
    
	int       HistSpan_TimeMode; //指明按时刻、时段
	long    HistSpan_TimeInterval; //单位为秒，指明时刻和时段划分间隔；0：整段，不划分，其他为时间的间隔时间 
	//关于历史时间的控制参数：时间段，时刻计算/时段方式
	CTime    HistSpan_lTimeBegin; //给定要计算的时间段起点
	CTime    HistSpan_lTimeEnd;   //给定要计算的时间段终点
	
   
}Model_CalcControl;
//每次模型计算需要设定其时间属性
typedef struct CalcModel_GlobalTime_struct{
	CTime tmCurHistBegin; //当前计算的时间段的开始 
	CTime tmCurHistEnd;  //当前计算的时间段结束
	
	
}Model_CalcGlobalTimeVars;
///////////////////////////////////////////////
class CCalcModel : public CObject  
{
public:
	CCalcModel();
	virtual ~CCalcModel();
	void AddModule(CCalcModule* pModule);
	int LookUpMoudle(CString name,CString desp);//需要名字和描述都不重复

	//
	void CheckBlockCalcIntegrity(CStringArray& LogStrings);
	void AdjustModuleOrder();//根据模块的次序值，调整模块的顺序
	void AdjustCalcSequence(); //根据给予的号码，调整计算的顺序
	void InitforCalc();
	void Calc();
	void  ResetCalc();//重复计算时候要进行复位
	void  WriteDataToRTDB();//写数据到实时历史库
    Model_CalcControl m_CalcControlPara;
	Model_CalcGlobalTimeVars  m_GlobalTimeVars; //模型当前的时间属性，可以被任何计算块所引用

	int m_iBackCalcTag;//补算标识
	long m_lDataTime;//模型处理数据的时间点
	long m_lDataSTime;//模型处理数据的时间段起点
	long m_lDataETime;//模型处理数据的时间段终点
	CString m_strModelName;
	CString m_strModelDesp;
	CString m_strModelIP;
	CString m_strPort;
	int m_iTagModeLocalOrRemote;//0:local;1:remote

	//关系数据源连接
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
	
	CArray<int,int> m_ArrSourceRDBTagCon;//1:连接。0:未连接
	CStringArray m_ArrSourceRDBName;
	CStringArray m_ArrSourceRDBType;//类型
	
	CStringArray m_ArrConDBIP;//连接数据库IP
	CStringArray m_ArrConDBName;//连接数据库名称
	CStringArray m_ArrConDBUser;//连接数据库用户名
	CStringArray m_ArrConDBPass;//连接数据库密码

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
	void SetWriteLogOnOrOff(int iOnOrOff);//设置日志开关
	void SetTraceOnOrOff(int iOnOrOff);//设置追踪开关
	void SetDebugOnOrOff(int iOnOrOff);//设置调试开关
	void RunCompile();
	void InitCompile(CWnd *pWnd,int iMsg,CLog *pLog);//初始化编译信息
	void ResetStopCalc();//停止标志复位//for stop
	void StopCalc();//for stop
	void InitControlPara();
	BOOL ExecuteBlock(CString strModuleName,CString strBlockName);
	int GetRunMode();
	void SetRunMode(int mode);
	void  SetRunState(int state);
	int GetRunState();
	
	void ResetModelContent();
	void ClearModuleBlocks(); //清除模块的所有计算块，但不清除模块本身

	CCalcModule* GetModuleByIndex(int index);
	CCalcModule* FindModuleByName(CString strModule);

	CCalcBlock* FindBlockByName(CString strBlock);
	CCalcBlock* FindBlockByName(CString strModule,CString strBlockName);
	//读取的数据库类型
	BOOL ReadModel(_ConnectionPtr &pCon); //读取得到模型包括的模块
	BOOL ReadModelDesp(_ConnectionPtr &pCon);//根据模型的名字读取模型的描述
	BOOL LoadAllModuleBlocks(_ConnectionPtr &pCon); //读取得到模型中各模块的计算块
	CModuleList m_ModuleList;
private:
	 CStringArray s_BlockTypeArray;
	 CStringArray s_BlockIDArray;

	 int m_iModelWarningCount;//警告个数
	 int m_iModelErrorCount;//错误个数
	 CWnd *m_pWnd;//获得编译和运行过程信息输出窗口的句柄
	 int m_iMsg;//获得编译和运行过程信息输出的消息编号
	 CLog *m_pLog;//用来输出日志
	 int m_iLogOnOrOff;//日志开或关1:开，0：关
	 int m_iTraceOn;//追踪开关1:开，0:关
	 int m_iDebugOn;//调试开关1:开，0:关
//**************实时历史库接口****************
public:
     BOOL m_bLoadRTDBDll;//是否加载了RTDB接口
	 BOOL m_bConRTDB;//是否建立了与实时历史库连接
	 BOOL m_bInitLoadAndConRTDB;//进行过实时历史库dll加载和连接的建立
	 HINSTANCE  hDLL;
public:
	 BOOL LoadRTDBDll();
	 BOOL InitRTDBConnect(HINSTANCE hDLL);
	 void LoadRTDBDllAndCon();
//**************实时历史库接口****************
protected:
	int   m_RunState; //运行状态：运行、暂停、停止
private:
	int m_iStopTag;//0:运行；1:停止//for stop

};

#endif // !defined(AFX_CALCMODEL_H__F4B6DAEF_3CFC_4C6D_9DB8_B8FCDB544479__INCLUDED_)
