#ifndef _RTDB_DEFINE_H__
#define _RTDB_DEFINE_H__

#include <afxsock.h>		// MFC socket extensions
#define RTDB_TAGTYPE_LENGTH 8		//测点状态最大长度


#define RTDB_RETURNSTATUS_SUCCESS   0        //查询成功
#define RTDB_RETURNSTATUS_NETECP_SEND    -1       //网络异常send失败
#define RTDB_RETURNSTATUS_NETECP_RECV    -2       //网络异常recv失败
#define RTDB_RETURNSTATUS_SYSECP    -3       //系统异常
#define RTDB_RETURNSTATUS_OTHERECP   -4		 //其他异常
#define RTDB_RETURNSTATUS_TYPEERR    -5       //数据类型不匹配

//************************************
//插入数据结构体（单个）
//************************************
typedef struct PLCInsertData_bool_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//写入点的类型，BOOL
	int       add;                              //开始地址
	int		  funcode;                          //功能码
	bool      value;		                    //写入点记录的值
}PLCInsertData_bool;
//************************************
//插入数据结构体（单个）
//************************************
typedef struct PLCInsertData_long_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//写入点的类型，DINT
	int       add;                              //开始地址
	int		  funcode;                          //功能码
	long      value;		                    //写入点记录的值
}PLCInsertData_long;
//************************************
//插入数据结构体（单个）
//************************************
typedef struct PLCInsertData_float_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//写入点的类型，REAL
	int       add;                              //开始地址
	int		  funcode;                          //功能码
	float     value;		                    //写入点记录的值
}PLCInsertData_float;
//给已有记录点插入数据结构体
//************************************
typedef struct PLCGetData_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//写入点的状态，BOOL INT REAL
	int       add;           //
	int		  funcode;
}PLCGetData;
#endif
