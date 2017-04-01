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
//��xp3���������£�������·������dll��������exe����xp3��win7��ʹ�á�
//���Ǹô�����win7��ֱ�ӱ����exe��xp3�»ᱨ�����Խ�xp3�µ��ļ������������·����������exe����xp3��win7��Ӧ�á�
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
	VALUE_DOUBLE=0, //����ֵ�����ֵ��ʱ�䡢ֵ��״̬��
	VALUE_RTVALUE_SERIES, //����ֵ���л����ֵ����
	VALUE_DOUBLE_SERIES,  //2013-7-17����������ʹ��
	VALUE_RTVALUE, //����ֵ
	
	VALUE_STAT,//ͳ��ֵ
	VALUE_STAT_SERIES//ͳ��ֵ����
};
enum RT_DATA_PREMETHOD
{
	PRE_GETMEAN=0, //ȡ��ֵ
	PRE_GETMIDDLE,  //��ֵ
	PRE_GETMAX,
	PRE_GETMIN
};

////////////////////////////////////////////////////////////
//ģ�Ϳ��ƽṹӰ������ã�
//��Ϊģ�͵�һ����Ա������VALUE���Դ��
//��model���м���ʱ����Ϊʵʱ���㣬��RT��Դ����ȡֵ,���ȡʵʱֵ.
//�ڽ��м�����Ƶ�ʱ�򣬲�ȡ�����µ�д����
/*
//ģ��ʵʱ���У�ÿ��һ�����������һ�μ���
    CCalcModel  model;
	//����ʵʱֵ��ļ��㣬����������=0
	model.CalcControl.GivenTime_MomentType =0;
	//��������ֵ��ļ��㣬��ȷ��ÿ�μ����ʱ���ȷ��������ÿ����Ҫ����ʵʱʱ��T0��������
	��1��-h  ��t0 ��ÿ��5���ӣ�����ӵ�ǰ��ǰ��
	��2������ʱ��T0,����
	��t1,t2��
*/
//ģ�Ϳ�ʼִ�е�����ģʽ

/////////////////////////////////////////////////
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_MODULE      1
#define HINT_UPDATE_BLOCK       2

typedef struct BLOCK_NODEINFO{
	 BOOL bValid;
     BOOL bInput; //�Ƿ�Ϊ����
	 BOOL bOutput;//�Ƿ�Ϊ���
	 int  nPortIndex; //�ǵڼ���
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
////////////////////////////////////////////�Զ���

#define		TAGNAME_MAX				48				// �����
#define		FUNC_MAX				100				// ���ʽ����
#define		PARM_MAX				32				// ���ʽ����

#define		RECCOUNTH_MAX			8000			// �����Դ���������ʷ
#define		RECCOUNTR_MAX			2000			// �����Դ�������ʵʱ

typedef struct tagTAGINFO{
	double			dValue;							// ��ֵ
	time_t			tTime;							// ��ʱ��
	DWORD			wState;							// ��״̬
	char			cName[TAGNAME_MAX];				// ����
} TAGINFO, *PTAGINFO;
typedef TAGINFO	(*RTFUNC)(TAGINFO* pVar, int);

//***************************************
//������ݽṹ��
//***************************************
typedef struct TagData_struct{
	double			value;		//���ص����ֵ
	long    		time;		//���ص��ʱ�䣬��
	int 			status;		//���ص��״̬
}TagData;
struct cpRTVALUE
{
	double			dValue;		//���ص����ֵ
	long    		ltime;		//���ص��ʱ�䣬��
	int 			status;		//���ص��״̬
};
#define RTDB_TAGNAME_LENGTH 48		//��������󳤶�
//**************************************
//��ѯ��ʷ��¼������Ľṹ
//**************************************
typedef struct ReadHiDataRequest_struct{
	long stTime;				//��ѯ���¼�Ŀ�ʼʱ�䣬��
	long enTime;				//��ѯ���¼�Ľ���ʱ�䣬��
	long tPeriod;			    //��ѯ��ֵ��ʱ��������
	int	 reqType;				//��ѯ���������ͣ�ԭʼ=0�����=1����С=2��ƽ��=3,����=4��
	char pointName[RTDB_TAGNAME_LENGTH];	    //�������������
}ReadHiDataRequest;

//************************************
//�����м�¼��������ݽṹ��
//************************************
typedef struct InsertData_struct{
	int		  type;			//Ϊ0��Ϊ��������Ϊ1��Ϊ������
	int		  status;		//д����¼��״̬
	double    value;		//д����¼��ֵ
	long 	  time;			//д����¼��ʱ�䣬��
	char      pointName[RTDB_TAGNAME_LENGTH];	//д������������
}InsertData;

/////////////////////
typedef struct RelvData_struct{
	char   id[10];
	char   type[4];
	char   status[4];
	char   time[20];
	char   value[20];
}RelvData;

//**************StatData ͳ�ƽ���ṹ��****************
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
//****************CXListCtrl�ؼ�����********
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
//****************CXListCtrl�ؼ�����********


//--�������
#define IF		 1
#define ELSE	 2
//--�����
#define VALD	201//--������(double)
#define PAR		202//--�����
#define VALWAND	203//--������(WORD�����á��߼��롱����)
#define VALWOR	204//--������(WORD�����á��߼��򡱲���)
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
#define	FUNC	401//����


//#include "globe_Fuc.h"
#include "Log.h"

//RTDBDefine.h��
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
