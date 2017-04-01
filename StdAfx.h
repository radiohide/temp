// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2814181D_5D3F_428C_8E1A_C832120B5AA3__INCLUDED_)
#define AFX_STDAFX_H__2814181D_5D3F_428C_8E1A_C832120B5AA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "process.h"
#include "BCGCBProInc.h"			// BCGControlBar Pro
#include   <afxtempl.h>
#include "math.h"

/*
#import "c:\program files\common files\system\ado\msado15.dll"\
//在xp3开发环境下，用以上路径加载dll，产生的exe可在xp3和win7下使用。
//但是该代码在win7下直接编译的exe在xp3下会报错，所以将xp3下的文件拷贝至代码根路径，则编译的exe可在xp3和win7下应用。
*/
#import "msado15.dll"\
rename_namespace("ADO")\
rename("EOF","adoEOF")
//#import "c:\program files\common files\system\ado\msado15.dll" \
//	no_namespace \
//	rename ("EOF", "adoEOF")  

/*
#import "c:\Program Files\Common Files\system\ado\msadox.dll" \
*/
#import "msadox.dll" \
rename_namespace( "ADOX") 
using   namespace   ADO;

#include "AFXSOCK.H"
//#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")



#define WM_TASKGUID		WM_USER+200 
#define WM_TASKGUID_CLOSE_CURRENT_TABVIEW	WM_USER+201
#define WM_TASKGUID_CLOSE_ALL_TABVIEW	WM_USER+202
#define WM_MSG_MODELRUN_STATUS	WM_USER + 0x0100
#define WM_MSG_MODELRUN_LOG	WM_USER + 0x0101
#define WM_MSG_PROGESS	WM_USER + 0x0105

#define WM_CLICK_ITER_FORMULALIST WM_USER+1000

// string message allocator for posting messages between windows...
static char* AllocBuffer(CString strMsg)
{
	int nLen = strMsg.GetLength();
	char *pBuffer = new char[nLen+1]; 
	
	strcpy(pBuffer,(const char*)strMsg);

	ASSERT(pBuffer != NULL);
	return pBuffer;
}


enum VALUE_TYPE_ENUM
{
	VALUE_DOUBLE=0, //数据值或过程值（时间、值、状态）
	VALUE_RTVALUE_SERIES, //数据值序列或过程值序列
	VALUE_DOUBLE_SERIES,  //2013-7-17，宣布不再使用
	VALUE_RTVALUE, //过程值
	
	VALUE_STAT,//统计值
	VALUE_STAT_SERIES//统计值序列
};
enum RT_DATA_PREMETHOD
{
	PRE_GETMEAN=0, //取均值
	PRE_GETMIDDLE,  //中值
	PRE_GETMAX,
	PRE_GETMIN
};

////////////////////////////////////////////////////////////
//模型控制结构影响和作用：
//作为模型的一个成员，对于VALUE类的源，
//在model进行计算时，若为实时运算，则RT类源数据取值,则读取实时值.
//在进行计算控制的时候，采取的如下的写法：
/*
//模型实时运行，每个一定间隔将触发一次计算
    CCalcModel  model;
	//对于实时值块的计算，仅给定类型=0
	model.CalcControl.GivenTime_MomentType =0;
	//对于序列值块的计算，需确定每次计算的时间段确定方法：每次需要根据实时时刻T0进行推算
	（1）-h  到t0 ：每隔5分钟，计算从当前往前推
	（2）根据时刻T0,计算
	【t1,t2】
*/
//模型开始执行的运行模式

/////////////////////////////////////////////////
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_MODULE      1
#define HINT_UPDATE_BLOCK       2

typedef struct BLOCK_NODEINFO{
	 BOOL bValid;
     BOOL bInput; //是否为输入
	 BOOL bOutput;//是否为输出
	 int  nPortIndex; //是第几个
	 CString m_PortID; 
	 CString m_PortDesp;//
}BLOCK_NODEINFO;

const COLORREF BLOCK_TITLE_BKCOLOR = RGB(0,90,245);//RGB(214,238,255);
const COLORREF BLOCK_TITLE_FTCOLOR = RGB(255,255,255);
const COLORREF BLOCK_BODY_BKCOLOR = RGB(214,238,255);//RGB(250,248,243);//RGB(136,196,255);
const COLORREF BLOCK_BODY_FTCOLOR = RGB(0,0,0);
const COLORREF BLOCK_INPUT_FTCOLOR = RGB(0,0,0);
const COLORREF BLOCK_INPUT_BKCOLOR[] = {RGB(128,128,128),RGB(0,255,0)};
const COLORREF BLOCK_OUPUT_BKCOLOR[] ={RGB(128,128,128),RGB(0,255,255)};
////////////////////////////////////////////自定义

#define		TAGNAME_MAX				48				// 最长点名
#define		FUNC_MAX				100				// 最大公式个数
#define		PARM_MAX				32				// 最大公式个数

#define		RECCOUNTH_MAX			8000			// 最大来源点个数，历史
#define		RECCOUNTR_MAX			2000			// 最大来源点个数，实时

typedef struct tagTAGINFO{
	double			dValue;							// 点值
	time_t			tTime;							// 点时标
	DWORD			wState;							// 点状态
	char			cName[TAGNAME_MAX];				// 点名
} TAGINFO, *PTAGINFO;
typedef TAGINFO	(*RTFUNC)(TAGINFO* pVar, int);

//***************************************
//测点数据结构体
//***************************************
typedef struct TagData_struct{
	double			value;		//返回点的数值
	long    		time;		//返回点的时间，秒
	int 			status;		//返回点的状态
}TagData;
struct cpRTVALUE
{
	double			dValue;		//返回点的数值
	long    		ltime;		//返回点的时间，秒
	int 			status;		//返回点的状态
};
#define RTDB_TAGNAME_LENGTH 48		//测点点名最大长度
//**************************************
//查询历史记录请求包的结构
//**************************************
typedef struct ReadHiDataRequest_struct{
	long stTime;				//查询点记录的开始时间，秒
	long enTime;				//查询点记录的结束时间，秒
	long tPeriod;			    //查询点值的时间间隔，秒
	int	 reqType;				//查询的数据类型（原始=0，最大=1，最小=2，平均=3,快照=4）
	char pointName[RTDB_TAGNAME_LENGTH];	    //请求的完整点名
}ReadHiDataRequest;

//************************************
//给已有记录点插入数据结构体
//************************************
typedef struct InsertData_struct{
	int		  type;			//为0则为开关量，为1则为摸拟量
	int		  status;		//写入点记录的状态
	double    value;		//写入点记录的值
	long 	  time;			//写入点记录的时间，秒
	char      pointName[RTDB_TAGNAME_LENGTH];	//写入点的完整点名
}InsertData;

/////////////////////
typedef struct RelvData_struct{
	char   id[10];
	char   type[4];
	char   status[4];
	char   time[20];
	char   value[20];
}RelvData;

//**************StatData 统计结果结构体****************
typedef struct StatData_struct{
	long lSTime;
	long lETime;
	long lInsertTime;
	double dMin;
	double dMax;
	double dValue;
	short iType;
	int iCount;
}StatData;
//*****************************************************
//****************CXListCtrl控件所加********
#ifndef XLISTCTRLLIB_EXPORTS//+
	#define XLISTCTRLLIB_EXPORTS//+
#endif//+
#define XLISTCTRLLIB_NOAUTOLIB//+

#define ENABLE_XLISTCTRL_TRACE//+

#ifdef ENABLE_XLISTCTRL_TRACE//+
	#define XLISTCTRL_TRACE TRACE//+
#else//+
	#define XLISTCTRL_TRACE __noop//+
#endif//+
//****************CXListCtrl控件所加********


//--基本语句
#define IF		 1
#define ELSE	 2
//--运算符
#define VALD	201//--数据项(double)
#define PAR		202//--测点项
#define VALWAND	203//--数据项(WORD，需用“逻辑与”操作)
#define VALWOR	204//--数据项(WORD，需用“逻辑或”操作)
#define	SM		301//<
#define	EQ		302//==
#define	LA		303//>
#define SE		304//<=
#define LE		305//>=
#define NE		306//!=
#define OR		307//||
#define AND		308//&&
#define ADD		310//+
#define MIN		311//-
#define MUL		312//*
#define DEV		313//--/
#define IV		314//^
#define NOT		315//!
#define SIN		320
#define COS		321
#define EXP		322
#define SQRT	323//sqrt
#define LOG		324
#define TAN		325
#define CTAN	326//ctan
#define ASIN	327//asin
#define ACOS	328//acos
#define ATAN	329//atan
#define ABS		334//fabs
#define	FUNC	401//函数


//#include "globe_Fuc.h"
#include "Log.h"

//RTDBDefine.h用
typedef int(*pGetRTDataByTagName)(const char* tagName,TagData * pTagData );
typedef int (*pGetRTDataByBatch)(char* tagNames[],TagData * pTagData,long nCount);
typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
typedef int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
typedef int (*pGetAggregationDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * pTagData);
typedef int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
typedef int (*pAppendRTTagDataByTagName)(InsertData *pInsertData);
typedef int (*pAppendRTTagDataByBatch)(InsertData *pInsertData,int nCount);
typedef int (*pInitConnect)(char* strParameters[],long nCount);
typedef int (*pReleaseConnect)();
typedef int (*pRTDBFreePointer)(void *p);
//////////////////////////////////////////////////////////////////////////////////////////////
CString gGetExeFileDir();
BOOL gIsTableExist(_ConnectionPtr& pConnection, CString strTableName);
BOOL gIsTableExistInMDB(_ConnectionPtr& pConnection, CString strTableName);
//BOOL gEstablishMDBConnectPtr(_ConnectionPtr &pCon,CString strFileName);
//void gCloseConnectPrt(_ConnectionPtr &pCon);
//////////////////////////////////////////////////////////////////////////////////////////////
void g_ReadRTValue(long ltime,int timetype,const char * tagName,double* pvalue,long* pltime);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2814181D_5D3F_428C_8E1A_C832120B5AA3__INCLUDED_)
