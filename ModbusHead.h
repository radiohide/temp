#ifndef _RTDB_DEFINE_H__
#define _RTDB_DEFINE_H__

#include <afxsock.h>		// MFC socket extensions
#define RTDB_TAGTYPE_LENGTH 8		//���״̬��󳤶�


#define RTDB_RETURNSTATUS_SUCCESS   0        //��ѯ�ɹ�
#define RTDB_RETURNSTATUS_NETECP_SEND    -1       //�����쳣sendʧ��
#define RTDB_RETURNSTATUS_NETECP_RECV    -2       //�����쳣recvʧ��
#define RTDB_RETURNSTATUS_SYSECP    -3       //ϵͳ�쳣
#define RTDB_RETURNSTATUS_OTHERECP   -4		 //�����쳣
#define RTDB_RETURNSTATUS_TYPEERR    -5       //�������Ͳ�ƥ��

//************************************
//�������ݽṹ�壨������
//************************************
typedef struct PLCInsertData_bool_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//д�������ͣ�BOOL
	int       add;                              //��ʼ��ַ
	int		  funcode;                          //������
	bool      value;		                    //д����¼��ֵ
}PLCInsertData_bool;
//************************************
//�������ݽṹ�壨������
//************************************
typedef struct PLCInsertData_long_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//д�������ͣ�DINT
	int       add;                              //��ʼ��ַ
	int		  funcode;                          //������
	long      value;		                    //д����¼��ֵ
}PLCInsertData_long;
//************************************
//�������ݽṹ�壨������
//************************************
typedef struct PLCInsertData_float_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//д�������ͣ�REAL
	int       add;                              //��ʼ��ַ
	int		  funcode;                          //������
	float     value;		                    //д����¼��ֵ
}PLCInsertData_float;
//�����м�¼��������ݽṹ��
//************************************
typedef struct PLCGetData_struct{
	char	  type[RTDB_TAGTYPE_LENGTH];		//д����״̬��BOOL INT REAL
	int       add;           //
	int		  funcode;
}PLCGetData;
#endif
