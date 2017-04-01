
#ifndef g_Data_zxg
#define g_Data_zxg

#include <vector>
#define		TAGNAME_MAX				48				// �����
#define		FUNC_MAX				100				// ���ʽ����
typedef struct unitTagData_struct{
	char   id[10];
	char   unit[10];
	char   sets[10];
	char   name[50];
	char   capacity[4];
	char   type[50];
	char   PatLib_StaTime[18];
	char   PatLib_LasTime[18];
	char   PatLib_VerNo[15];
}unitTagData;
typedef struct logTagData_struct{
	char   logID[24];
	char   donetime[48];
	char   logbegintime[48];
	char   logendtime[48];
}logTagData;
typedef struct pointTagData_struct{
	char   varID[20];
	char   varname[50];
	char   vardes[50];
	char   varunit[5];
	char   varformulation[100];
	char   vartype[10];
	std::vector<CString> strings_points;//���������ö�Ӧ���ֵ�����
    std::vector<CString> pointcycle;//���������ö�Ӧ���ֵ�����

	char   category_value[50];//	char   classifyattribute[50];
	char   category_endpoint[45];//	char   clusterbegin[45];
	char   direction[45];
	char   fixsep[45];
	char   relationsep[45];
	char   clustersource[45];
	//char   data_source[1024];
	std::vector<CString> data_source;//���������ò�����Դ
	std::vector<CString> tag_beinvolved;//�������Ƿ������ϴ�ı�� 1���� 0 ������
}pointTagData;
typedef struct ReadHiDataRequest_struct{
	long stTime;				//��ѯ���¼�Ŀ�ʼʱ�䣬��
	long enTime;				//��ѯ���¼�Ľ���ʱ�䣬��
	long tPeriod;			    //��ѯ��ֵ��ʱ��������
	int	 reqType;				//��ѯ���������ͣ�ԭʼ=0�����=1����С=2��ƽ��=3,����=4��
	char pointName[48];	    //�������������
}ReadHiDataRequest;

typedef struct TagData_struct{
	double			value;		//���ص����ֵ
	long    		time;		//���ص��ʱ�䣬��
	int 			status;		//���ص��״̬
}TagData;

typedef struct fresh_badTagData_struct{
	char		index[10];		//���
	char		begintime_bad[48];		//��ʱ��εĿ�ʼ
	char		endtime_bad[48];		//��ʱ��εĽ���
	char		timeflag[48];           //��ʱ��������
}freshbadTagData; 
typedef struct fresh_goodTagData_struct{
	char		index[10];		//���
	char		begintime_good[48];		//��ʱ��εĿ�ʼ
	char		endtime_good[48];		//��ʱ��εĽ���
	char		timeflag[48];           //��ʱ��������
}freshgoodTagData; 

typedef struct hist_badTagData_struct{
	char		index[10];		//���
	char		begintime_bad[48];		//��ʱ��εĿ�ʼ
	char		endtime_bad[48];		//��ʱ��εĽ���
	char		timeflag[48];           //��ʱ��������
}histbadTagData; 

typedef struct hist_goodTagData_struct{
	char		index[10];		//���
	char		begintime_good[48];		//��ʱ��εĿ�ʼ
	char		endtime_good[48];		//��ʱ��εĽ���
	char		timeflag[48];           //��ʱ��������
}histgoodTagData;

typedef struct ordercorresponding_TagData_struct{
	int		Rnum;		//�к�
	int		Ordernum;		//��ͬ�������ͬ�����е�˳��ֵ
	int		Listnum;		//�б��е�����
}ordercorrespondingTagData;

typedef struct dataCleanGlobal_TagData_struct{
	int		rate;		//����(��Ȼ��)
}dataCleanGlobalTagData;

typedef struct seps_TagData_struct{
	int		sep_begin;		//ʱ��ο�ʼ
	int		sep_end;		//ʱ��ν���
	int		sep_flag;		//ʱ�䣨δ�ã�
}jiangesTagData;

typedef struct Group_TagData_struct{
	char		groupID[10];		//��ID
	char		groupname[50];		//����
	char		groupdes[50];		//������
	char		grouptype[50];      //������
}GroupTagData; 
typedef struct VarsGroup_TagData_struct{
	char		id[10];		        //���
	char		groupid[50];		//����ID
	char		varname[50];		//����
}VarsGroupTagData; 

typedef struct ConditionClass_Fresh_TagData_struct{
	char		ID[48];		            //����ID
	char		Condition_code[128];		//��������
	char		Record_num[48];		    //��¼����
	char		Carry_situation[48];    //�������
    char		Coal_situation[48];		//ú�����
	char		Env_Tem_situation[48];  //�����¶����
	char        Ear_RID[48];//ʱ�������RID
	char        Late_RID[48];//ʱ�������RID
}ConditionClass_FreshTagData; 

typedef struct StableRecord_Fresh_TagData_struct{
	char		RID[48];		            //RID
	char		Stable_begin[48];		 //��̬��ʼʱ���ǩ
	char		Stable_end[48];		    //��̬����ʱ���ǩ
	char		Stable_longth[48];     //��̬ʱ��γ���
    char		Stable_flag[48];				//��̬��Ч��־
	char		Condition_Code[128];				//��������
}StableRecord_FreshTagData; 

typedef struct StableRecord_Hist_TagData_struct{
	char		RID[48];		            //RID
	char		Stable_begin[48];		 //��̬��ʼʱ���ǩ
	char		Stable_end[48];		    //��̬����ʱ���ǩ
	char		Stable_longth[48];     //��̬ʱ��γ���
    char		Stable_flag[48];				//��̬��Ч��־
	char		Condition_Code[128];				//��������
}StableRecord_HistTagData;

typedef struct Data_TagData_struct{
	char		RID[48];		            //RID
	char		avg[48];			//��������ģ�����ķ���ֵ������ģ�������ص���ƽ��ֵ���������������ص���1����0��������ֵ
	char		mixvalue[48];		//��Сֵ
	char		mixtime[48];		//��Сֵʱ��
    char		maxvalue[48];		//���ֵ
	char		maxtime[48];		//���ֵʱ��
	char		mean_square[48];	//������
	char		stablevalue[48];	//�ȶ���
	
	char        frequency[48];      //������Ƶ��
}DataTagData; 

typedef struct basicgroupclass_fresh_TagData_struct{
	char		RID[48];		   
    std::vector<CString> SortStr;//����������GroupX_ClassMark
}basicgroupclass_freshTagData;

typedef struct basicgroupclass_hist_TagData_struct{
	char		RID[48];		   
    std::vector<CString> SortStr;//����������GroupX_ClassMark
}basicgroupclass_histTagData;

// T_UnitX_Group[X]_Class_Hist��
typedef struct groupX_class_hist_TagData_struct{
	char		ClassSerial_id[48];	//�����ID	   
	char		class_mark[48];     //������
	char		condition_code[128]; //������������
	char		record_numble[48];  //������¼����
    std::vector<CString> point_eigenvalues;//������һ���е���������ֵ
}groupX_class_histTagData;

typedef struct groupX_class_fresh_TagData_struct{
	char		ClassSerial_id[48];	//�����ID	   
	char		class_mark[48];     //������
	char		condition_code[128]; //������������
	char		record_numble[48];  //������¼����
    std::vector<CString> point_eigenvalues;//������һ���е���������ֵ
}groupX_class_freshTagData;

typedef struct dataloadinfo_TagData_struct{
	char		ID[10];	//����	   
	char		load_starttime[15];     //���ؿ�ʼʱ��
	char		load_endtime[15]; //���ؽ���ʱ��
	char		load_operatetime[15];     //����ʱ��
	char		load_operator[8]; //������
	char		load_remark[200]; //��ע
}dataloadinfoTagData;

typedef struct tagTAGINFO{
	double			dValue;							// ��ֵ
	time_t			tTime;							// ��ʱ��
	DWORD			wState;							// ��״̬
	char			cName[TAGNAME_MAX];				// ����
} TAGINFO, *PTAGINFO;
typedef struct ComGroup_TagData_struct{
	char		ComgroupID[148];		//
	char		Comgroupname[148];	//
	char		Comgroupdes[148];	//
}ComGroupTagData;
typedef struct ComVarsGroup_TagData_struct{
	char		ComID[48];		//
	char		ComgroupID[48];//
	char		Comvarname[48];//
}ComVarsGroupTagData; 
typedef TAGINFO	(*RTFUNC)(TAGINFO* pVar, int);
#endif
